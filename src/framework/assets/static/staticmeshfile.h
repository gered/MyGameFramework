#ifndef __FRAMEWORK_ASSETS_STATIC_STATICMESHFILE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_STATIC_STATICMESHFILE_H_INCLUDED__

#include "../../common.h"

#include <stl/string.h>

#include "../meshfile.h"

class File;
struct StaticMeshTriangle;
struct Vector2;
struct Vector3;

/**
 * Holds information about materials in a static mesh file.
 */
struct StaticMeshFileMaterial
{
	stl::string name;
	stl::string textureFile;
};

/**
 * Holds information about sub-meshes (subsets) in a static mesh file.
 */
struct StaticMeshFileSubMesh
{
	stl::string name;
	uint32_t material;
	uint32_t numTriangles;
};

/**
 * A static mesh file which is loaded using a custom "chunked" file format.
 */
class StaticMeshFile : public MeshFile
{
public:
	/**
	 * Creates a static mesh file object by loading the mesh information
	 * from the provided file.
	 * @param file the file that will be loaded as a static mesh
	 */
	StaticMeshFile(File *file);

	virtual ~StaticMeshFile();

	/**
	 * @return the total number of vertices this mesh contains
	 */
	uint32_t GetNumVertices() const                        { return m_numVertices; }

	/**
	 * @return pointer to the mesh's vertex data
	 */
	Vector3* GetVertices() const                           { return m_vertices; }

	/**
	 * @return the total number of normals this mesh contains
	 */
	uint32_t GetNumNormals() const                         { return m_numNormals; }

	/**
	 * @return pointer to the mesh's normal data, or NULL if none exist
	 */
	Vector3* GetNormals() const                            { return m_normals; }

	/**
	 * @return the total number of texture coordinates this mesh contains
	 */
	uint32_t GetNumTexCoords() const                       { return m_numTexCoords; }

	/**
	 * @return pointer to the mesh's texture coordinates, or NULL if none exist
	 */
	Vector2* GetTexCoords() const                          { return m_texCoords; }

	/**
	 * @return the total number of materials that this mesh contains
	 */
	uint32_t GetNumMaterials() const                       { return m_numMaterials; }

	/**
	 * @return pointer to the mesh's materials
	 */
	StaticMeshFileMaterial* GetMaterials() const           { return m_materials; }

	/**
	 * @return the total number of triangles that this mesh contains
	 */
	uint32_t GetNumTriangles() const                       { return m_numTriangles; }

	/**
	 * @return pointer to the mesh's triangles
	 */
	StaticMeshTriangle* GetTriangles() const               { return m_triangles; }

	/**
	 * @return the total number of sub-meshes (subsets) this mesh contains
	 */
	uint32_t GetNumSubMeshes() const                       { return m_numSubMeshes; }

	/**
	 * @return pointer to the mesh's sub-meshes (subsets)
	 */
	StaticMeshFileSubMesh* GetSubMeshes() const            { return m_subMeshes; }

private:
	void Load();

	uint32_t m_numVertices;
	Vector3 *m_vertices;
	uint32_t m_numNormals;
	Vector3 *m_normals;
	uint32_t m_numTexCoords;
	Vector2 *m_texCoords;
	uint32_t m_numMaterials;
	StaticMeshFileMaterial *m_materials;
	uint32_t m_numTriangles;
	StaticMeshTriangle *m_triangles;
	uint32_t m_numSubMeshes;
	StaticMeshFileSubMesh *m_subMeshes;
};

#endif
