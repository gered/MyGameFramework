#include "../../debug.h"

#include "staticmesh.h"

#include "staticmeshfile.h"
#include "staticmeshsubset.h"
#include "staticmeshtriangle.h"
#include "../../content/contentmanager.h"
#include "../../graphics/texture.h"
#include "../../graphics/vertexbuffer.h"
#include "../../math/vector3.h"
#include "../../math/vector2.h"

StaticMesh::StaticMesh(uint32_t numSubsets, StaticMeshSubset **subsets)
	: Content()
{
	ASSERT(numSubsets > 0);
	ASSERT(subsets != NULL);

	// important to note: this object is now assuming responsibility for the
	// memory allocated to the StaticMeshSubset objects passed in. so this 
	// object should delete them when we're done, and not the caller
	m_numSubsets = numSubsets;
	m_subsets = subsets;
}

StaticMesh::StaticMesh(const StaticMeshFile *file, ContentManager *contentManager)
	: Content()
{
	m_numSubsets = 0;
	CreateSubsets(file, contentManager);
}

StaticMesh::~StaticMesh()
{
	for (uint32_t i = 0; i < m_numSubsets; ++i)
		SAFE_DELETE(m_subsets[i]);
	SAFE_DELETE_ARRAY(m_subsets);
}

void StaticMesh::CreateSubsets(const StaticMeshFile *file, ContentManager *contentManager)
{
	m_numSubsets = file->GetNumSubMeshes();
	m_subsets = new StaticMeshSubset*[m_numSubsets];
	ASSERT(m_subsets != NULL);

	StaticMeshTriangle *triangles = file->GetTriangles();
	StaticMeshFileMaterial *materials = file->GetMaterials();
	StaticMeshFileSubMesh *subMeshes = file->GetSubMeshes();
	Vector3 *vertices = file->GetVertices();
	Vector3 *normals = file->GetNormals();
	Vector2 *texCoords = file->GetTexCoords();

	for (uint32_t i = 0; i < m_numSubsets; ++i)
	{
		StaticMeshFileSubMesh *subMesh = &subMeshes[i];
		StaticMeshFileMaterial *material = &materials[subMesh->material];

		Texture *texture = NULL;
		if (material->textureFile.length() > 0)
			texture = contentManager->Get<Texture>(material->textureFile.c_str());
		StaticMeshSubset *subset = new StaticMeshSubset(subMesh->numTriangles, texture);
		ASSERT(subset != NULL);

		// add the triangles to the subset
		uint32_t n = 0;
		for (uint32_t j = 0; j < file->GetNumTriangles(); ++j)
		{
			if (triangles[j].subMeshIndex == i)
			{
				uint32_t pos = n * 3;

				subset->GetVertices()->SetPosition3(pos, vertices[triangles[j].vertices[0]]);
				subset->GetVertices()->SetPosition3(pos + 1, vertices[triangles[j].vertices[1]]);
				subset->GetVertices()->SetPosition3(pos + 2, vertices[triangles[j].vertices[2]]);

				if (file->GetNumNormals() > 0)
				{
					subset->GetVertices()->SetNormal(pos, normals[triangles[j].vertices[0]]);
					subset->GetVertices()->SetNormal(pos + 1, normals[triangles[j].vertices[1]]);
					subset->GetVertices()->SetNormal(pos + 2, normals[triangles[j].vertices[2]]);
				}

				if (file->GetNumTexCoords() > 0)
				{
					subset->GetVertices()->SetTexCoord(pos, texCoords[triangles[j].vertices[0]]);
					subset->GetVertices()->SetTexCoord(pos + 1, texCoords[triangles[j].vertices[1]]);
					subset->GetVertices()->SetTexCoord(pos + 2, texCoords[triangles[j].vertices[2]]);
				}

				++n;
			}
		}

		m_subsets[i] = subset;
	}
}
