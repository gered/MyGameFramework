#ifndef __FRAMEWORK_ASSETS_STATIC_STATICMESHINSTANCE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_STATIC_STATICMESHINSTANCE_H_INCLUDED__

#include "../../common.h"

#include "staticmesh.h"

class RenderState;
class Texture;

/**
 * An instance of a static mesh that can be manipulated and rendered from
 * a single shared mesh object.
 */
class StaticMeshInstance
{
public:
	/**
	 * Creates a new static mesh instance.
	 * @param mesh the mesh this instance is of
	 */
	StaticMeshInstance(StaticMesh *mesh);

	virtual ~StaticMeshInstance();

	/**
	 * @return the render state used to render this instance
	 */
	RenderState* GetRenderState() const                    { return m_renderState; }

	/**
	 * @return the mesh this instance is of
	 */
	StaticMesh* GetMesh() const                            { return m_mesh; }

	/**
	 * @return the number of textures/materials that the mesh this instance
	 *         is for contains
	 */
	uint32_t GetNumTextures() const                        { return m_mesh->GetNumSubsets(); }

	/**
	 * Gets the texture for the specified subset/material.
	 * @param index the index of the specified subset/material to get the
	 *              texture for
	 * @return the texture, or NULL if one is not set
	 */
	Texture* GetTexture(uint32_t index) const              { return m_textures[index]; }

	/**
	 * Sets a new texture for the specified subset/material.
	 * @param index the index of the subset/material to change the texture for
	 * @param texture the new texture to set, or NULL to set no texture
	 */
	void SetTexture(uint32_t index, Texture *texture);

	/**
	 * Resets the texture for the specified subset/material back to the mesh's
	 * original texture.
	 * @param index the index of the subset/material to reset the texture for
	 */
	void ResetTexture(uint32_t index);

private:
	StaticMesh *m_mesh;
	RenderState *m_renderState;

	Texture **m_textures;
};

#endif
