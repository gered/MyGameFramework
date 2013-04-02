#include "../../debug.h"

#include "../../common.h"
#include "joint.h"
#include "jointkeyframe.h"
#include "jointvertexmapping.h"
#include "skeletalmesh.h"
#include "skeletalmeshfile.h"
#include "skeletalmeshsubset.h"
#include "skeletalmeshtriangle.h"
#include "../../file/file.h"
#include "../../graphics/indexbuffer.h"
#include "../../graphics/vertexbuffer.h"
#include "../../math/quaternion.h"
#include "../../math/vector3.h"
#include "../../support/animationsequence.h"
#include <stl/string.h>
#include <stl/vector.h>

SkeletalMeshFile::SkeletalMeshFile(File *file)
	: MeshFile(file)
{
}

SkeletalMeshFile::~SkeletalMeshFile()
{
}

SkeletalMesh* SkeletalMeshFile::CreateMesh()
{
	ChunkDescriptor *verticesDesc = GetChunkDesc("VTX");
	ChunkDescriptor *normalsDesc = GetChunkDesc("NRL");
	ChunkDescriptor *texCoordsDesc = GetChunkDesc("TXT");
	ChunkDescriptor *trianglesDesc = GetChunkDesc("TRI");
	ChunkDescriptor *groupsDesc = GetChunkDesc("GRP");
	ChunkDescriptor *jointsDesc = GetChunkDesc("JNT");
	ChunkDescriptor *jointsToVerticesDesc = GetChunkDesc("JTV");
	ChunkDescriptor *jointFramesDesc = GetChunkDesc("JKF");
	ChunkDescriptor *animationsDesc = GetChunkDesc("ANI");
	ASSERT(verticesDesc != NULL);
	ASSERT(trianglesDesc != NULL);
	ASSERT(groupsDesc != NULL);
	ASSERT(jointsDesc != NULL);
	ASSERT(jointsToVerticesDesc != NULL);
	
	BOOL hasNormals = (normalsDesc != NULL ? TRUE : FALSE);
	BOOL hasTexCoords = (texCoordsDesc != NULL ? TRUE : FALSE);
	
	File *file = GetFile();
	SkeletalMesh *mesh = new SkeletalMesh();
	
	// read number of vertices first
	// NOTE: we assume # of vertices == # of normals == # of tex coords
	file->Seek(verticesDesc->start, FILESEEK_BEGINNING);
	mesh->m_numVertices = file->ReadUnsignedInt();
	
	// lol, lazy way out
	stl::vector<VERTEX_ATTRIBS> attribs;
	attribs.reserve(4);
	attribs.push_back(VERTEX_F1);
	attribs.push_back(VERTEX_POS_3D);
	if (hasNormals)
		attribs.push_back(VERTEX_NORMAL);
	if (hasTexCoords)
		attribs.push_back(VERTEX_TEXCOORD);
	
	mesh->m_vertexBuffer = new VertexBuffer();
	mesh->m_vertexBuffer->Initialize(&attribs[0], attribs.size(), mesh->m_numVertices, BUFFEROBJECT_USAGE_STATIC);
	
	// read vertices
	mesh->m_vertices = new Vector3[mesh->m_numVertices];
	for (uint32_t i = 0; i < mesh->m_numVertices; ++i)
	{
		mesh->m_vertices[i].x = file->ReadFloat();
		mesh->m_vertices[i].y = file->ReadFloat();
		mesh->m_vertices[i].z = file->ReadFloat();
		mesh->m_vertexBuffer->SetPosition3(i, mesh->m_vertices[i]);
	}
	
	// normals
	if (hasNormals)
	{
		file->Seek(normalsDesc->start, FILESEEK_BEGINNING);
		uint32_t numNormals = file->ReadUnsignedInt();
		ASSERT(numNormals == mesh->m_numVertices);
		for (uint32_t i = 0; i < numNormals; ++i)
		{
			Vector3 normal;
			normal.x = file->ReadFloat();
			normal.y = file->ReadFloat();
			normal.z = file->ReadFloat();
			mesh->m_vertexBuffer->SetNormal(i, normal);
		}
	}
	
	// texture coords
	if (hasTexCoords)
	{
		file->Seek(texCoordsDesc->start, FILESEEK_BEGINNING);
		uint32_t numTexCoords = file->ReadUnsignedInt();
		ASSERT(numTexCoords == mesh->m_numVertices);
		for (uint32_t i = 0; i < numTexCoords; ++i)
		{
			Vector2 texCoord;
			texCoord.x = file->ReadFloat();
			texCoord.y = file->ReadFloat();
			mesh->m_vertexBuffer->SetTexCoord(i, texCoord);
		}
	}
	
	// groups / subsets
	file->Seek(groupsDesc->start, FILESEEK_BEGINNING);
	mesh->m_numSubsets = file->ReadUnsignedInt();
	mesh->m_subsets = new SkeletalMeshSubset[mesh->m_numSubsets];
	for (uint32_t i = 0; i < mesh->m_numSubsets; ++i)
	{
		stl::string name;
		stl::string texture;
		BOOL alpha;
		uint32_t numTriangles;

		file->ReadString(name);
		file->ReadString(texture);
		alpha = file->ReadChar() == 0 ? FALSE : TRUE;
		numTriangles = file->ReadUnsignedInt();
		
		mesh->m_subsets[i].Create(name, numTriangles, alpha);
		mesh->m_subsets[i].GetIndices()->MoveToStart();
	}
	
	// triangles
	file->Seek(trianglesDesc->start, FILESEEK_BEGINNING);
	uint32_t numTriangles = file->ReadUnsignedInt();
	for (uint32_t i = 0; i < numTriangles; ++i)
	{
		uint32_t v1 = file->ReadUnsignedInt();
		uint32_t v2 = file->ReadUnsignedInt();
		uint32_t v3 = file->ReadUnsignedInt();
		uint32_t subsetIndex = file->ReadUnsignedInt();
		
		SkeletalMeshSubset *subset = &mesh->m_subsets[subsetIndex];
		subset->GetIndices()->SetCurrent((uint16_t)v1);
		subset->GetIndices()->MoveNext();
		subset->GetIndices()->SetCurrent((uint16_t)v2);
		subset->GetIndices()->MoveNext();
		subset->GetIndices()->SetCurrent((uint16_t)v3);
		subset->GetIndices()->MoveNext();
	}
	
	// joints
	file->Seek(jointsDesc->start, FILESEEK_BEGINNING);
	mesh->m_numJoints = file->ReadUnsignedInt();
	mesh->m_joints = new Joint[mesh->m_numJoints];
	for (uint32_t i = 0; i < mesh->m_numJoints; ++i)
	{
		file->ReadString(mesh->m_joints[i].name);
		int32_t parentIndex = file->ReadInt();
		mesh->m_joints[i].parentIndex = parentIndex;
		if (parentIndex >= 0)
			mesh->m_joints[i].parent = &mesh->m_joints[parentIndex];
		
		Vector3 initialPosition;
		initialPosition.x = file->ReadFloat();
		initialPosition.y = file->ReadFloat();
		initialPosition.z = file->ReadFloat();
		
		float rotationX = file->ReadFloat();
		float rotationY = file->ReadFloat();
		float rotationZ = file->ReadFloat();
		Quaternion initialRotation = Quaternion::CreateFromEulerAngles(rotationX, rotationY, rotationZ);
		
		mesh->m_joints[i].relative = 
			Matrix4x4::CreateTranslation(initialPosition.x, initialPosition.y, initialPosition.z) *
			initialRotation.ToMatrix();
	}
	
	// build absolute transformation matrices for all the joints we just loaded
	for (uint32_t i = 0; i < mesh->m_numJoints; ++i)
	{
		Joint *joint = &mesh->m_joints[i];
		if (joint->parent != NULL)
		{
			joint->absolute = joint->parent->absolute;
			joint->absolute = joint->absolute * joint->relative;
		}
		else
			joint->absolute = joint->relative;
	}
	
	// set up inverse absolute transforms for each joint in preparation
	// for transforming all the vertices
	Matrix4x4 *inverseJointTransforms = new Matrix4x4[mesh->m_numJoints];
	for (uint32_t i = 0; i < mesh->m_numJoints; ++i)
		inverseJointTransforms[i] = Matrix4x4::Inverse(mesh->m_joints[i].absolute);
	
	// joint-to-vertex mappings
	file->Seek(jointsToVerticesDesc->start, FILESEEK_BEGINNING);
	uint32_t numMappings = file->ReadUnsignedInt();
	ASSERT(numMappings == mesh->m_numVertices);
	mesh->m_jointMappings = new JointVertexMapping[numMappings];
	for (uint32_t i = 0; i < numMappings; ++i)
	{
		uint32_t jointIndex = file->ReadUnsignedInt();
		mesh->m_jointMappings[i].jointIndex = jointIndex;
		mesh->m_jointMappings[i].weight = file->ReadFloat();

		// set joint index for this vertex in the VBO
		mesh->m_vertexBuffer->Set1f(0, i, (float)jointIndex);
		
		// transform vertex by it's joint's inverse absolute transform
		Vector3 vertex = mesh->m_vertexBuffer->GetPosition3(i);
		vertex = Matrix4x4::Transform(inverseJointTransforms[jointIndex], vertex);
		mesh->m_vertexBuffer->SetPosition3(i, vertex);
		
		// transform normal by it's joint's inverse absolute transform
		if (hasNormals)
		{
			Vector3 normal = mesh->m_vertexBuffer->GetNormal(i);
			normal = Matrix4x4::TransformUsingRotationOnly(inverseJointTransforms[jointIndex], normal);
			mesh->m_vertexBuffer->SetNormal(i, normal);
		}
	}
	
	// joint keyframes
	if (jointFramesDesc != NULL)
	{
		file->Seek(jointFramesDesc->start, FILESEEK_BEGINNING);
		mesh->m_numFrames = file->ReadUnsignedInt();
		
		// allocate memory for the frames for each joint
		for (uint32_t i = 0; i < mesh->m_numJoints; ++i)
			mesh->m_joints[i].frames = new JointKeyFrame[mesh->m_numFrames];
		
		for (uint32_t i = 0; i < mesh->m_numFrames; ++i)
		{
			for (uint32_t j = 0; j < mesh->m_numJoints; ++j)
			{
				mesh->m_joints[j].frames[i].position.x = file->ReadFloat();
				mesh->m_joints[j].frames[i].position.y = file->ReadFloat();
				mesh->m_joints[j].frames[i].position.z = file->ReadFloat();
				
				float rotationX = file->ReadFloat();
				float rotationY = file->ReadFloat();
				float rotationZ = file->ReadFloat();
				mesh->m_joints[j].frames[i].rotation = Quaternion::CreateFromEulerAngles(rotationX, rotationY, rotationZ);
			}
		}
	}

	// animations
	if (animationsDesc != NULL)
	{
		file->Seek(animationsDesc->start, FILESEEK_BEGINNING);
		int32_t numAnimations = file->ReadInt();
		for (int32_t i = 0; i < numAnimations; ++i)
		{
			AnimationSequence sequence;
			stl::string name;
			
			file->ReadString(name);
			sequence.start = file->ReadInt();
			sequence.stop = file->ReadInt();
			
			mesh->m_animations[name] = sequence;
		}
	}
	
	mesh->FindAndSetRootJointIndex();

	return mesh;
}
