#ifndef __FRAMEWORK_ASSETS_STATIC_STATICMESH_H_INCLUDED__
#define __FRAMEWORK_ASSETS_STATIC_STATICMESH_H_INCLUDED__

#include "../../common.h"
#include "../../content/content.h"
#include "../../util/typesystem.h"

class ContentManager;
class StaticMeshFile;
class StaticMeshSubset;

/**
 * Static mesh object containing geometry that does not animate.
 */
class StaticMesh : public Content
{
public:
	TYPE_DEFINE(CONTENT_TYPE, "StaticMesh");

	/**
	 * Creates a static mesh object.
	 * @param numSubsets the number of subsets being provided
	 * @param subsets mesh subsets that this static mesh is to be made up of
	 */
	StaticMesh(uint numSubsets, StaticMeshSubset **subsets);

	/**
	 * Creates a static mesh object.
	 * @param file the static mesh file to load the mesh data from
	 * @param contentManager the content manager that this mesh is being
	 *                       loaded from
	 */
	StaticMesh(const StaticMeshFile *file, ContentManager *contentManager);

	virtual ~StaticMesh();

	/**
	 * @return the number of subsets that make up this mesh
	 */
	uint GetNumSubsets() const                             { return m_numSubsets; }

	/**
	 * Gets the specified subset contained in this mesh.
	 * @param index the subset to get
	 * @return the mesh subset
	 */
	StaticMeshSubset* GetSubset(uint index) const          { return m_subsets[index]; }

private:
	void CreateSubsets(const StaticMeshFile *file, ContentManager *contentManager);

	uint m_numSubsets;
	StaticMeshSubset **m_subsets;
};

#endif
