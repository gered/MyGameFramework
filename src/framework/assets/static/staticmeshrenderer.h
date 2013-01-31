#ifndef __FRAMEWORK_ASSETS_STATIC_STATICMESHRENDERER_H_INCLUDED__
#define __FRAMEWORK_ASSETS_STATIC_STATICMESHRENDERER_H_INCLUDED__

#include "../../common.h"

class GraphicsDevice;
class StaticMeshInstance;
class StaticMeshSubset;
class Texture;

/**
 * Helper object that renders instances of static mesh objects.
 */
class StaticMeshRenderer
{
public:
	/**
	 * Creates a static mesh renderer.
	 */
	StaticMeshRenderer();

	virtual ~StaticMeshRenderer();

	/**
	 * Renders an instance of a static mesh.
	 * @param graphicsDevice the graphics device to render with
	 * @param instance the static mesh instance to render
	 */
	void Render(GraphicsDevice *graphicsDevice, StaticMeshInstance *instance);

private:
	void RenderAllSubsets(GraphicsDevice *graphicsDevice, StaticMeshInstance *instance);
	void RenderAllSubsetsTextureless(GraphicsDevice *graphicsDevice, StaticMeshInstance *instance);

	void RenderSubset(GraphicsDevice *graphicsDevice, const StaticMeshSubset *subset, const Texture *texture);
	void RenderTexturelessSubset(GraphicsDevice *graphicsDevice, const StaticMeshSubset *subset);
};

#endif
