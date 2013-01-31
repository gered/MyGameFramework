#include "../../debug.h"

#include "keyframemeshfile.h"

#include "keyframe.h"
#include "keyframemeshtriangle.h"
#include "../meshfile.h"
#include "../../file/file.h"
#include "../../math/vector2.h"
#include "../../math/vector3.h"
#include "../../support/animationsequence.h"

KeyframeMeshFile::KeyframeMeshFile(File *file)
	: MeshFile(file)
{
	STACK_TRACE;
	m_numFrames = 0;
	m_numTexCoords = 0;
	m_numTriangles = 0;
	m_numVerticesPerFrame = 0;
	m_frames = NULL;
	m_texCoords = NULL;
	m_triangles = NULL;
	Load();
}

KeyframeMeshFile::~KeyframeMeshFile()
{
	STACK_TRACE;
	for (uint32_t i = 0; i < m_numFrames; ++i)
		SAFE_DELETE(m_frames[i]);
	SAFE_DELETE_ARRAY(m_frames);
	SAFE_DELETE_ARRAY(m_texCoords);
	SAFE_DELETE_ARRAY(m_triangles);
	m_animations.clear();
}

void KeyframeMeshFile::Load()
{
	STACK_TRACE;
	ChunkDescriptor *keyframesDesc = GetChunkDesc("KFR");
	ChunkDescriptor *texCoordsDesc = GetChunkDesc("TXT");
	ChunkDescriptor *trianglesDesc = GetChunkDesc("KTR");
	ChunkDescriptor *animationsDesc = GetChunkDesc("ANI");
	ASSERT(keyframesDesc != NULL);
	ASSERT(texCoordsDesc != NULL);
	ASSERT(trianglesDesc != NULL);

	File *file = GetFile();

	// keyframes
	file->Seek(keyframesDesc->start, FILESEEK_BEGINNING);
	m_numFrames = file->ReadInt();
	uint32_t numVertices = file->ReadInt();
	m_frames = new Keyframe*[m_numFrames];
	ASSERT(m_frames != NULL);
	for (uint32_t i = 0; i < m_numFrames; ++i)
	{
		Keyframe *frame = new Keyframe(numVertices);
		ASSERT(frame != NULL);
		Vector3 *vertices = frame->GetVertices();
		Vector3 *normals = frame->GetNormals();

		// vertices
		for (uint32_t j = 0; j < numVertices; ++j)
		{
			vertices[j].x = file->ReadFloat();
			vertices[j].y = file->ReadFloat();
			vertices[j].z = file->ReadFloat();
		}

		// normals
		for (uint32_t j = 0; j < numVertices; ++j)
		{
			normals[j].x = file->ReadFloat();
			normals[j].y = file->ReadFloat();
			normals[j].z = file->ReadFloat();
		}

		m_frames[i] = frame;
	}

	// texture coordinates
	file->Seek(texCoordsDesc->start, FILESEEK_BEGINNING);
	m_numTexCoords = file->ReadInt();
	m_texCoords = new Vector2[m_numTexCoords];
	ASSERT(m_texCoords != NULL);
	for (uint32_t i = 0; i < m_numTexCoords; ++i)
	{
		m_texCoords[i].x = file->ReadFloat();
		m_texCoords[i].y = file->ReadFloat();
	}

	// triangles
	file->Seek(trianglesDesc->start, FILESEEK_BEGINNING);
	m_numTriangles = file->ReadInt();
	m_triangles = new KeyframeMeshTriangle[m_numTriangles];
	ASSERT(m_triangles != NULL);
	for (uint32_t i = 0; i < m_numTriangles; ++i)
	{
		m_triangles[i].vertices[0] = file->ReadInt();
		m_triangles[i].vertices[1] = file->ReadInt();
		m_triangles[i].vertices[2] = file->ReadInt();

		m_triangles[i].textureCoords[0] = file->ReadInt();
		m_triangles[i].textureCoords[1] = file->ReadInt();
		m_triangles[i].textureCoords[2] = file->ReadInt();
	}

	// for the rendering loop
	m_numVerticesPerFrame = m_numTriangles * 3;

	// animations
	if (animationsDesc != NULL)
	{
		file->Seek(animationsDesc->start, FILESEEK_BEGINNING);
		uint32_t numAnimations = file->ReadInt();
		for (uint32_t i = 0; i < numAnimations; ++i)
		{
			AnimationSequence sequence;
			stl::string name;
			
			file->ReadString(name);
			sequence.start = file->ReadInt();
			sequence.stop = file->ReadInt();

			m_animations[name] = sequence;
		}
	}
}
