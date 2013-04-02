#ifndef __FRAMEWORK_GRAPHICS_IMAGE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_IMAGE_H_INCLUDED__

#include "../common.h"
#include "../content/content.h"
#include "../util/typesystem.h"

#include "color.h"
#include "imageformats.h"

class File;

/**
 * Represents raw 2D image data.
 */
class Image : public Content
{
public:
	TYPE_DEFINE(CONTENT_TYPE, "Image");

	Image();
	virtual ~Image()                                                            { Release(); }
	
	/**
	 * Creates a empty new image.
	 * @param width the width of the image
	 * @param height the height of the image
	 * @param format the pixel format of the image
	 * @return TRUE if successful
	 */
	BOOL Create(uint width, uint height, IMAGE_FORMAT format);
	
	/**
	 * Creates a copy of an image from another image object.
	 * @param source the source image object to copy
	 * @return TRUE if successful
	 */
	BOOL Create(const Image *source);
	
	/**
	 * Creates a copy of a subsection of an image.
	 * @param source the source image object to copy
	 * @param x left X coordinate of the source region to copy
	 * @param y top Y coordinate of the source region to copy
	 * @param width the width of the source region to copy
	 * @param height the height of the source region to copy
	 * @return TRUE if successful
	 */
	BOOL Create(const Image *source, uint x, uint y, uint width, uint height);
	
	/**
	 * Creates an image from an image file.
	 * @param file the file to load as an image
	 * @return TRUE if successful
	 */
	BOOL Create(File *file);
	
	/**
	 * Frees all image resources.
	 */
	void Release();

	/**
	 * @return pointer to raw image pixel data
	 */
	uint8_t* GetPixels() const                                                  { return m_pixels; }

	/**
	 * @return offset into the raw image pixel data that corresponds
	 *                to the given X and Y coordinate on the image
	 */
	uint GetOffsetFor(uint x, uint y) const;

	/**
	 * Gets a pointer to the raw image pixel data beginning at the given
	 * X and Y coordinates on the image.
	 * @param x X coordinate to get a pointer to
	 * @param y Y coordinate to get a pointer to
	 * @return pointer to the raw image pixel data for this location
	 */
	uint8_t* Get(uint x, uint y) const;

	/**
	 * Gets a filled color object that corresponds to the pixel located
	 * at the given image position.
	 * @param x X coordinate to get the color of
	 * @param y Y coordinate to get the color of
	 * @return color object containing the color of the specified pixel
	 */
	Color GetColor(uint x, uint y) const;

	/**
	 * Sets the color of a pixel on the image.
	 * @param x X coordinate of the pixel to set
	 * @param y Y coordinate of the pixel to set
	 * @param color the color to set the pixel to
	 */
	void SetColor(uint x, uint y, const Color &color);

	/**
	 * Sets the color of a pixel on the image.
	 * @param x X coordinate of the pixel to set
	 * @param y Y coordinate of the pixel to set
	 * @param color the color (as a 32-bit hex value) to set the pixel to
	 */
	void SetColor(uint x, uint y, uint32_t color)  { SetColor(x, y, Color::FromInt(color)); }

	/**
	 * Copies a source image and draws it onto this image at the position
	 * given. The source image must fit entirely in this image. No clipping
	 * is performed.
	 * @param source the source image to copy
	 * @param destX the X coordinate to copy the source image to on this image
	 * @param destY the Y coordinate to copy the source image to on this image
	 */
	void Copy(const Image *source, uint destX, uint destY);

	/**
	 * Copies a source image and draws it onto this image at the position
	 * given. The source image must fit entirely in this image. No clipping
	 * is performed.
	 * @param source the source image to copy
	 * @param x the left X coordinate of the region to copy from the source image
	 * @param y the top Y coordinate of the region to copy from the source image
	 * @param width the width of the region to copy from the source image
	 * @param height the height of the region to copy from the source image
	 * @param destX the X coordinate to copy the source image to on this image
	 * @param destY the Y coordinate to copy the source image to on this image
	 */
	void Copy(const Image *source, uint x, uint y, uint width, uint height, uint destX, uint destY);

	/**
	 * @return the width of this image
	 */
	uint GetWidth() const                                                       { return m_width; }

	/**
	 * @return the height of this image
	 */
	uint GetHeight() const                                                      { return m_height; }

	/**
	 * @return the bits per pixel of this image's pixel data
	 */
	uint GetBpp() const                                                         { return m_bpp; }

	/**
	 * @return the pixel format of this image
	 */
	IMAGE_FORMAT GetFormat() const                                              { return m_format; }

	/**
	 * @return the number of bytes equivalent to one horizontal line 
	 *         of pixel data
	 */
	uint GetPitch() const                                                       { return m_pitch; }

	/**
	 * @return the number of bytes the raw pixel data in this image
	 */
	uint GetSizeInBytes() const;

	/**
	 * Zeros-out the pixel data for this image.
	 */
	void Clear();

	/**
	 * Clears the image using the given color.
	 * @param color the color to clear the image with
	 */
	void Clear(const Color &color);

	/**
	 * Clears the image using the given color.
	 * @param color the color (as a 32-bit hex value) to clear the image with
	 */
	void Clear(const uint32_t color);

	/**
	 * Clears the image using the given alpha value. This can only be
	 * used with images where the format is IMAGE_FORMAT_ALPHA.
	 * @param alpha the alpha value to clear the image with
	 */
	void Clear(const uint8_t alpha);
	
	/**
	 * Flips the image vertically.
	 */
	void FlipVertically();

private:
	uint8_t *m_pixels;
	uint m_pitch;
	uint m_width;
	uint m_height;
	uint m_bpp;
	IMAGE_FORMAT m_format;
};

inline uint Image::GetOffsetFor(uint x, uint y) const
{
	return (x + (y * m_width)) * (m_bpp / 8);
}

inline uint8_t* Image::Get(uint x, uint y) const
{
	return m_pixels + GetOffsetFor(x, y);
}

inline uint Image::GetSizeInBytes() const
{
	return (m_width * m_height) * (m_bpp / 8);
}

#endif

