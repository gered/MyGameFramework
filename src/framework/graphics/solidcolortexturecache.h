#ifndef __FRAMEWORK_GRAPHICS_SOLIDCOLORTEXTURECACHE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SOLIDCOLORTEXTURECACHE_H_INCLUDED__

#include "../common.h"
#include <stl/map.h>

class GraphicsDevice;
class Texture;
struct Color;

typedef stl::map<uint32_t, Texture*> ColorTextureMap;

/**
 * Cache of textures that are filled with solid colors only.
 */
class SolidColorTextureCache
{
public:
	/**
	 * Creates a solid color texture cache.
	 * @param graphicsDevice the graphics device used to create textures with
	 */
	SolidColorTextureCache(GraphicsDevice *graphicsDevice);

	virtual ~SolidColorTextureCache();

	/**
	 * New OpenGL graphics context creation callback.
	 */
	void OnNewContext();

	/**
	 * Lost OpenGL graphics context callback.
	 */
	void OnLostContext();

	/**
	 * Gets a texture that is filled with a single solid color. If this texture
	 * has not been created yet it will be first, then returned.
	 * @param the color to get a texture for
	 * @return the texture or NULL on failure
	 */
	Texture* Get(const Color &color);

private:
	Texture* CreateFor(const Color &color, Texture *existing = NULL);
	void FreeAll();

	GraphicsDevice *m_graphicsDevice;
	ColorTextureMap m_textures;
};

#endif

