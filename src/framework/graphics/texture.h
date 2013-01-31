#ifndef __FRAMEWORK_GRAPHICS_TEXTURE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_TEXTURE_H_INCLUDED__

#include "../common.h"
#include "../content/content.h"
#include "textureformats.h"

class GraphicsDevice;
class Image;

/**
 * Represents a texture that can be bound and rendered with.
 */
class Texture : public Content
{
public:
	static CONTENT_TYPE GetType()
	{
		static CONTENT_TYPE typeName = "Texture";
		return typeName;
	}
	CONTENT_TYPE GetTypeOf() const                         { return GetType(); }

	Texture();
	virtual ~Texture();
	
	/**
	 * Creates a new texture from the specified image.
	 * @param graphicsDevice the graphics device this texture is associated with
	 * @param image the image to create the texture from
	 * @return TRUE if the texture was created successfully
	 */
	BOOL Create(GraphicsDevice *graphicsDevice, Image *image);
	
	/**
	 * Creates a new texture with uninitialized image data. Image data should
	 * be uploaded to the texture before using it via Update().
	 * @param graphicsDevice the graphics device this texture is associated with
	 * @param width the width of the texture in pixels
	 * @param height the height of the texture in pixels
	 * @param textureFormat the format of the pixel data this texture contains
	 */
	BOOL Create(GraphicsDevice *graphicsDevice, uint16_t width, uint16_t height, TEXTURE_FORMAT textureFormat);
	
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
	 * @return TRUE if the texture was updated successfully
	 */
	BOOL Update(Image *image, uint16_t destX = 0, uint16_t destY = 0);

	/**
	 * @return the texture name or ID assigned to this texture by OpenGL
	 */
	uint32_t GetTextureName() const                        { return m_textureName; }

	/**
	 * @return the width of this texture in pixels
	 */
	uint16_t GetWidth() const                              { return m_width; }

	/**
	 * @return the height of this texture in pixels
	 */
	uint16_t GetHeight() const                             { return m_height; }

	/**
	 * @return the pixel format of this texture
	 */
	TEXTURE_FORMAT GetFormat() const                       { return m_format; }

	/**
	 * @return TRUE if the texture has invalidated and needs to be recreated
	 */
	BOOL IsInvalidated() const                             { return m_textureName == 0; }

	/**
	 * Callback which handles freeing the texture resources when the OpenGL
	 * context is lost. This prepares the texture object to be reloaded
	 * via one of the Create() methods when the OpenGL context is regained.
	 */
	void OnLostContext();

private:
	void GetTextureSpecsFromFormat(TEXTURE_FORMAT textureFormat, int *bpp, uint32_t *format, uint32_t *type);
	
	GraphicsDevice *m_graphicsDevice;
	uint32_t m_textureName;
	uint16_t m_width;
	uint16_t m_height;
	TEXTURE_FORMAT m_format;
};

#endif
