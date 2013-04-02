#ifndef __FRAMEWORK_GRAPHICS_TEXTURE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_TEXTURE_H_INCLUDED__

#include "../common.h"
#include "../content/content.h"
#include "../util/typesystem.h"
#include "textureformats.h"

class GraphicsDevice;
class Image;

/**
 * Represents a texture that can be bound and rendered with.
 */
class Texture : public Content
{
public:
	TYPE_DEFINE(CONTENT_TYPE, "Texture");

	Texture();
	virtual ~Texture();
	
	/**
	 * Creates a new texture from the specified image.
	 * @param graphicsDevice the graphics device this texture is associated with
	 * @param image the image to create the texture from
	 * @return true if the texture was created successfully
	 */
	bool Create(GraphicsDevice *graphicsDevice, Image *image);
	
	/**
	 * Creates a new texture with uninitialized image data. Image data should
	 * be uploaded to the texture before using it via Update().
	 * @param graphicsDevice the graphics device this texture is associated with
	 * @param width the width of the texture in pixels
	 * @param height the height of the texture in pixels
	 * @param textureFormat the format of the pixel data this texture contains
	 */
	bool Create(GraphicsDevice *graphicsDevice, uint width, uint height, TEXTURE_FORMAT textureFormat);
	
	/**
	 * Frees the texture resources.
	 */
	void Release();
	
	/**
	 * Updates all of or just part of an this texture with new image data.
	 * @param image the image to upload to the texture. This image should
	 *              have dimensions equal to or less then this texture.
	 * @param destX the X coordinate on the texture to place the new image at
	 * @param destY the Y coordinate on the texture to place the new image at
	 * @return true if the texture was updated successfully
	 */
	bool Update(Image *image, uint destX = 0, uint destY = 0);

	/**
	 * @return the texture name or ID assigned to this texture by OpenGL
	 */
	uint GetTextureName() const                            { return m_textureName; }

	/**
	 * @return the width of this texture in pixels
	 */
	uint GetWidth() const                                  { return m_width; }

	/**
	 * @return the height of this texture in pixels
	 */
	uint GetHeight() const                                 { return m_height; }

	/**
	 * @return the pixel format of this texture
	 */
	TEXTURE_FORMAT GetFormat() const                       { return m_format; }

	/**
	 * @return true if the texture has invalidated and needs to be recreated
	 */
	bool IsInvalidated() const                             { return m_textureName == 0; }

	/**
	 * Callback which handles freeing the texture resources when the OpenGL
	 * context is lost. This prepares the texture object to be reloaded
	 * via one of the Create() methods when the OpenGL context is regained.
	 */
	void OnLostContext();

private:
	void GetTextureSpecsFromFormat(TEXTURE_FORMAT textureFormat, int *bpp, uint *format, uint *type);
	
	GraphicsDevice *m_graphicsDevice;
	uint m_textureName;
	uint m_width;
	uint m_height;
	TEXTURE_FORMAT m_format;
};

#endif
