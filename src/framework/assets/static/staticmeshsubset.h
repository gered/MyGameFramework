#ifndef __FRAMEWORK_ASSETS_STATIC_STATICMESHSUBSET_H_INCLUDED__
#define __FRAMEWORK_ASSETS_STATIC_STATICMESHSUBSET_H_INCLUDED__

#include "../../common.h"

class Texture;
class VertexBuffer;

/**
 * Contains vertex and texture information for a single static mesh
 * sub-mesh (subset).
 */
class StaticMeshSubset
{
public:
	/**
	 * Creates a static mesh subset.
	 * @param numTriangles the number of triangles this subset will contain
	 * @param texture the texture to be applied to this subset
	 */
	StaticMeshSubset(uint numTriangles, Texture *texture);

	virtual ~StaticMeshSubset();

	/**
	 * @return the vertices contained in this subset
	 */
	VertexBuffer* GetVertices() const                      { return m_vertices; }

	/**
	 * @return the texture applied to this subset, or NULL if no texture is set
	 */
	Texture* GetTexture() const                            { return m_texture; }

private:
	VertexBuffer *m_vertices;
	Texture *m_texture;
};


#endif
