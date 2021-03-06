#include "../debug.h"
#include "../log.h"

#include "image.h"

#include "../file/file.h"
#include "../file/memoryfile.h"

#include <string.h>

#define STBI_NO_STDIO
#include <stb_image.h>

Image::Image()
{
	m_pixels = NULL;
	m_width = 0;
	m_height = 0;
	m_bpp = 0;
	m_format = IMAGE_FORMAT_NONE;
	m_pitch = 0;
}

void Image::Release()
{
	SAFE_DELETE(m_pixels);
	m_width = 0;
	m_height = 0;
	m_bpp = 0;
	m_format = IMAGE_FORMAT_NONE;
	m_pitch = 0;
}


bool Image::Create(uint width, uint height, IMAGE_FORMAT format)
{
	ASSERT(m_pixels == NULL);
	if (m_pixels != NULL)
		return false;
	
	ASSERT(width != 0);
	ASSERT(height != 0);
	if (width == 0 || height == 0)
		return false;
	
	int bpp = 0;
	if (format == IMAGE_FORMAT_RGB)
		bpp = 24;
	else if (format == IMAGE_FORMAT_RGBA)
		bpp = 32;
	else if (format == IMAGE_FORMAT_ALPHA)
		bpp = 8;
	
	ASSERT(bpp != 0);
	if (bpp == 0)
		return false;
	
	uint pixelsLength = (width * height) * (bpp / 8);
	m_pixels = new uint8_t[pixelsLength];
	memset(m_pixels, 0, pixelsLength);
	
	m_width = width;
	m_height = height;
	m_bpp = bpp;
	m_format = format;
	m_pitch = width * (bpp / 8);
	
	return true;
}

bool Image::Create(const Image *source)
{
	ASSERT(source != NULL);
	if (source == NULL)
		return false;
	else
		return Create(source, 0, 0, source->GetWidth(), source->GetHeight());
}

bool Image::Create(const Image *source, uint x, uint y, uint width, uint height)
{
	ASSERT(m_pixels == NULL);
	if (m_pixels != NULL)
		return false;
	
	ASSERT(source != NULL);
	if (source == NULL)
		return false;
	
	ASSERT(source->GetPixels() != NULL);
	if (source->GetPixels() == NULL)
		return false;
	
	ASSERT(x < source->GetWidth());
	ASSERT(y < source->GetHeight());
	ASSERT((x + width) <= source->GetWidth());
	ASSERT((y + height) <= source->GetHeight());
	
	bool baseCreateSuccess = Create(width, height, source->GetFormat());
	if (!baseCreateSuccess)
		return false;

	Copy(source, x, y, width, height, 0, 0);

	return true;
}

bool Image::Create(File *file)
{
	ASSERT(m_pixels == NULL);
	if (m_pixels != NULL)
		return false;
	
	ASSERT(file != NULL);
	if (file == NULL)
		return false;
	
	ASSERT(file->IsOpen());
	if (!file->IsOpen())
		return false;
	
	uint8_t *imageFileBytes = NULL;
	size_t imageFileSize = file->GetFileSize();
	
	// if the file object is a MemoryFile, we can skip needing to allocate + copy memory around for stb_image
	if (file->GetFileType() == FILETYPE_MEMORY)
	{
		MemoryFile *memoryFile = (MemoryFile*)file;
		imageFileBytes = (uint8_t*)memoryFile->GetFileData();
	}
	else
	{
		// load raw file bytes fully into memory (can't pass our File instance to stb_image)
		imageFileBytes = new uint8_t[imageFileSize];
		ASSERT(imageFileBytes != NULL);
		file->Read((int8_t*)imageFileBytes, imageFileSize);
	}
	
	// load using stb_image
	int width;
	int height;
	int componentsPerPixel;
	uint8_t *pixels = stbi_load_from_memory(imageFileBytes, imageFileSize, &width, &height, &componentsPerPixel, 0);
	if (pixels == NULL)
	{
		LOG_ERROR(LOGCAT_ASSETS, "Error loading image \"%s\": %s\n", file->GetFilename().c_str(), stbi_failure_reason());
		if (file->GetFileType() != FILETYPE_MEMORY)
			SAFE_DELETE_ARRAY(imageFileBytes);
		
		return false;
	}
	else
	{
		// don't need this anymore
		if (file->GetFileType() != FILETYPE_MEMORY)
			SAFE_DELETE_ARRAY(imageFileBytes);
		
		int bpp = componentsPerPixel * 8;
		IMAGE_FORMAT format;
		// TODO: detect IMAGE_FORMAT_ALPHA
		if (componentsPerPixel == 3)
			format = IMAGE_FORMAT_RGB;
		else if (componentsPerPixel == 4)
			format = IMAGE_FORMAT_RGBA;
		else
		{
			ASSERT(!"Unrecognized componentsPerPixel value.");
			stbi_image_free(pixels);
			return false;
		}
		
		// copy from STB "owned" memory to our own, then free up the STB stuff
		// (this is kind of unnecessary, but I'd like this method to be the
		// *only* one that knows/cares about stb_image at all)
		uint pixelsLength = (width * height) * componentsPerPixel;
		m_pixels = new uint8_t[pixelsLength];
		memcpy(m_pixels, pixels, pixelsLength);
		stbi_image_free(pixels);
		
		m_width = width;
		m_height = height;
		m_bpp = bpp;
		m_format = format;
		m_pitch = width * componentsPerPixel;		
		
		return true;
	}
}

Color Image::GetColor(uint x, uint y) const
{
	ASSERT(m_format == IMAGE_FORMAT_RGB || m_format == IMAGE_FORMAT_RGBA);

	if (m_format == IMAGE_FORMAT_RGB)
	{
		uint8_t *pixel = Get(x, y);
		return Color(*pixel, *(pixel + 1), *(pixel + 2));
	}
	else
	{
		uint8_t *pixel = Get(x, y);
		return Color(*pixel, *(pixel + 1), *(pixel + 2), *(pixel + 3));
	}
}

void Image::SetColor(uint x, uint y, const Color &color)
{
	ASSERT(m_format == IMAGE_FORMAT_RGB || m_format == IMAGE_FORMAT_RGBA);

	if (m_format == IMAGE_FORMAT_RGB)
	{
		uint8_t *pixel = Get(x, y);
		uint8_t dummy;
		color.ToInts(pixel, pixel + 1, pixel + 2, &dummy);
	}
	else
	{
		uint8_t *pixel = Get(x, y);
		color.ToInts(pixel, pixel + 1, pixel + 2, pixel + 3);
	}
}

void Image::Copy(const Image *source, uint destX, uint destY)
{
	ASSERT(source != NULL);
	Copy(source, 0, 0, source->GetWidth(), source->GetHeight(), destX, destY);
}

void Image::Copy(const Image *source, uint x, uint y, uint width, uint height, uint destX, uint destY)
{
	ASSERT(source != NULL);
	if (source == NULL)
		return;
	
	ASSERT(source->GetBpp() == m_bpp);
	ASSERT((x + width) <= source->GetWidth());
	ASSERT((y + height) <= source->GetHeight());
	ASSERT((destX + width) <= m_width);
	ASSERT((destY + height) <= m_height);

	uint8_t *sourcePixels = source->GetPixels() + source->GetOffsetFor(x, y);
	uint8_t *destPixels = m_pixels + GetOffsetFor(destX, destY);

	uint lineWidthInBytes = width * (m_bpp / 8);
	uint numLinesToCopy = height;

	for (uint i = 0; i < numLinesToCopy; ++i)
	{
		memcpy(destPixels, sourcePixels, lineWidthInBytes);
		sourcePixels += source->GetPitch();
		destPixels += m_pitch;
	}
}

void Image::Clear()
{
	memset(m_pixels, 0, GetSizeInBytes());
}

void Image::Clear(const Color &color)
{
	ASSERT(m_format == IMAGE_FORMAT_RGB || m_format == IMAGE_FORMAT_RGBA);

	uint sizeInBytes = GetSizeInBytes();
	uint8_t *pixel = m_pixels;

	if (m_format == IMAGE_FORMAT_RGB)
	{
		ASSERT(sizeInBytes % 3 == 0);
		uint8_t dummy;
		for (uint i = 0; i < sizeInBytes; i += 3)
		{
			color.ToInts(pixel, pixel + 1, pixel + 2, &dummy);
			pixel += 3;
		}
	}
	else
	{
		ASSERT(sizeInBytes % 4 == 0);
		for (uint i = 0; i < sizeInBytes; i += 4)
		{
			color.ToInts(pixel, pixel + 1, pixel + 2, pixel + 3);
			pixel += 4;
		}
	}
}

void Image::Clear(const uint32_t color)
{
	Clear(Color::FromInt(color));
}

void Image::Clear(const uint8_t alpha)
{
	ASSERT(m_format == IMAGE_FORMAT_ALPHA);
	memset(m_pixels, alpha, GetSizeInBytes());
}

void Image::FlipVertically()
{
	ASSERT(m_pixels != NULL);
	if (m_pixels == NULL)
		return;
	
	// TODO: this is a naive implementation. fix so it can flip the image
	//       one scanline at a time using the already allocated memory
	
	uint pixelsLength = (m_width * m_height) * (m_bpp / 8);
	uint8_t *flippedPixels = new uint8_t[pixelsLength];

	uint8_t *source = (m_pixels + pixelsLength) - m_pitch;   // first pixel of the last line
	uint8_t *dest = flippedPixels;                           // first pixel of the first line
	
	for (uint y = 0; y < m_height; ++y)
	{
		memcpy(dest, source, m_pitch);
		source -= m_pitch;                      // go up one line
		dest += m_pitch;                        // go down one line
	}
	
	SAFE_DELETE(m_pixels);
	m_pixels = flippedPixels;
}
