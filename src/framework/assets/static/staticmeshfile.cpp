#include "../../debug.h"

#include "staticmeshfile.h"

#include "staticmeshtriangle.h"
#include "../../file/file.h"
#include "../../math/vector3.h"
#include "../../math/vector2.h"

StaticMeshFile::StaticMeshFile(File *file)
	: MeshFile(file)
{
	STACK_TRACE;
	m_vertices = NULL;
	m_normals = NULL;
	m_texCoords = NULL;
	m_materials = NULL;
	m_triangles = NULL;
	m_subMeshes = NULL;
	m_numVertices = 0;
	m_numNormals = 0;
	m_numTexCoords = 0;
	m_numMaterials = 0;
	m_numTriangles = 0;
	m_numSubMeshes = 0;
	Load();
}

StaticMeshFile::~StaticMeshFile()
{
	STACK_TRACE;
	SAFE_DELETE_ARRAY(m_vertices);
	SAFE_DELETE_ARRAY(m_normals);
	SAFE_DELETE_ARRAY(m_texCoords);
	SAFE_DELETE_ARRAY(m_materials);
	SAFE_DELETE_ARRAY(m_triangles);
	SAFE_DELETE_ARRAY(m_subMeshes);
}

void StaticMeshFile::Load()
{
	STACK_TRACE;
	ChunkDescriptor *verticesDesc = GetChunkDesc("VTX");
	ChunkDescriptor *normalsDesc = GetChunkDesc("NRL");
	ChunkDescriptor *texCoordsDesc = GetChunkDesc("TXT");
	ChunkDescriptor *materialsDesc = GetChunkDesc("MTL");
	ChunkDescriptor *trianglesDesc = GetChunkDesc("TRI");
	ChunkDescriptor *subMeshesDesc = GetChunkDesc("GRP");
	ASSERT(verticesDesc != NULL);
	ASSERT(materialsDesc != NULL);
	ASSERT(trianglesDesc != NULL);
	ASSERT(subMeshesDesc != NULL);

	File *file = GetFile();

	// vertices
	file->Seek(verticesDesc->start, FILESEEK_BEGINNING);
	m_numVertices = file->ReadUnsignedInt();
	m_vertices = new Vector3[m_numVertices];
	ASSERT(m_vertices != NULL);
	for (uint32_t i = 0; i < m_numVertices; ++i)
	{
		m_vertices[i].x = file->ReadFloat();
		m_vertices[i].y = file->ReadFloat();
		m_vertices[i].z = file->ReadFloat();
	}

	// normals
	if (normalsDesc != NULL)
	{
		file->Seek(normalsDesc->start, FILESEEK_BEGINNING);
		m_numNormals = file->ReadUnsignedInt();
		m_normals = new Vector3[m_numNormals];
		ASSERT(m_normals != NULL);
		for (uint32_t i = 0; i < m_numNormals; ++i)
		{
			m_normals[i].x = file->ReadFloat();
			m_normals[i].y = file->ReadFloat();
			m_normals[i].z = file->ReadFloat();
		}
	}

	// texture coordinates
	if (texCoordsDesc != NULL)
	{
		file->Seek(texCoordsDesc->start, FILESEEK_BEGINNING);
		m_numTexCoords = file->ReadUnsignedInt();
		m_texCoords = new Vector2[m_numTexCoords];
		ASSERT(m_texCoords != NULL);
		for (uint32_t i = 0; i < m_numTexCoords; ++i)
		{
			m_texCoords[i].x = file->ReadFloat();
			m_texCoords[i].y = file->ReadFloat();
		}
	}

	// materials chunk
	file->Seek(materialsDesc->start, FILESEEK_BEGINNING);
	m_numMaterials = file->ReadUnsignedInt();
	m_materials = new StaticMeshFileMaterial[m_numMaterials];
	ASSERT(m_materials != NULL);
	for (uint32_t i = 0; i < m_numMaterials; ++i)
	{
		StaticMeshFileMaterial *material = &m_materials[i];

		file->ReadString(material->name);
		file->ReadString(material->textureFile);

		// TODO: read and store the rest of the material somewhere. skip for now
		file->Seek(sizeof(float) * 14, FILESEEK_CURRENT);
	}

	// triangles
	file->Seek(trianglesDesc->start, FILESEEK_BEGINNING);
	m_numTriangles = file->ReadUnsignedInt();
	m_triangles = new StaticMeshTriangle[m_numTriangles];
	ASSERT(m_triangles != NULL);
	for (uint32_t i = 0; i < m_numTriangles; ++i)
	{
		m_triangles[i].vertices[0] = file->ReadUnsignedInt();
		m_triangles[i].vertices[1] = file->ReadUnsignedInt();
		m_triangles[i].vertices[2] = file->ReadUnsignedInt();

		m_triangles[i].subMeshIndex = file->ReadUnsignedInt();
	}

	// sub meshes chunk
	file->Seek(subMeshesDesc->start, FILESEEK_BEGINNING);
	m_numSubMeshes = file->ReadUnsignedInt();
	m_subMeshes = new StaticMeshFileSubMesh[m_numSubMeshes];
	ASSERT(m_subMeshes != NULL);
	for (uint32_t i = 0; i < m_numSubMeshes; ++i)
	{
		StaticMeshFileSubMesh *subMesh = &m_subMeshes[i];

		file->ReadString(subMesh->name);
		subMesh->material = file->ReadUnsignedInt();
		subMesh->numTriangles = file->ReadUnsignedInt();
	}
}
