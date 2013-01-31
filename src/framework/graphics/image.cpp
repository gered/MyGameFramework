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
	STACK_TRACE;
	m_pixels = NULL;
	m_width = 0;
	m_height = 0;
	m_bpp = 0;
	m_format = IMAGE_FORMAT_NONE;
	m_pitch = 0;
}

Image::~Image()
{
	STACK_TRACE;
	Release();
}

void Image::Release()
{
	STACK_TRACE;
	SAFE_DELETE(m_pixels);
	m_width = 0;
	m_height = 0;
	m_bpp = 0;
	m_format = IMAGE_FORMAT_NONE;
	m_pitch = 0;
}


BOOL Image::Create(uint16_t width, uint16_t height, IMAGE_FORMAT format)
{
	STACK_TRACE;
	ASSERT(m_pixels == NULL);
	if (m_pixels != NULL)
		return FALSE;
	
	ASSERT(width != 0);
	ASSERT(height != 0);
	if (width == 0 || height == 0)
		return FALSE;
	
	int bpp = 0;
	if (format == IMAGE_FORMAT_RGB)
		bpp = 24;
	else if (format == IMAGE_FORMAT_RGBA)
		bpp = 32;
	else if (format == IMAGE_FORMAT_ALPHA)
		bpp = 8;
	
	ASSERT(bpp != 0);
	if (bpp == 0)
		return FALSE;
	
	size_t pixelsLength = (width * height) * (bpp / 8);
	m_pixels = new uint8_t[pixelsLength];
	memset(m_pixels, 0, pixelsLength);
	
	m_width = width;
	m_height = height;
	m_bpp = bpp;
	m_format = format;
	m_pitch = width * (bpp / 8);
	
	return TRUE;
}

BOOL Image::Create(const Image *source)
{
	STACK_TRACE;
	ASSERT(source != NULL);
	if (source == NULL)
		return FALSE;
	else
		return Create(source, 0, 0, source->GetWidth() - 1, source->GetHeight() - 1);
}

BOOL Image::Create(const Image *source, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	STACK_TRACE;
	ASSERT(m_pixels == NULL);
	if (m_pixels != NULL)
		return FALSE;
	
	ASSERT(source->GetPixels() != NULL);
	if (source->GetPixels() == NULL)
		return FALSE;
	
	ASSERT(right > left);
	ASSERT(bottom > top);
	ASSERT(left < source->GetWidth());
	ASSERT(right < source->GetWidth());
	ASSERT(top < source->GetHeight());
	ASSERT(bottom < source->GetHeight());
	
	BOOL baseCreateSuccess = Create((right - left) + 1, (bottom - top) + 1, source->GetFormat());
	if (!baseCreateSuccess)
		return FALSE;

	Copy(source, left, top, right, bottom, 0, 0);

	return TRUE;
}

BOOL Image::Create(File *file)
{
	STACK_TRACE;
	ASSERT(m_pixels == NULL);
	if (m_pixels != NULL)
		return FALSE;
	
	ASSERT(file != NULL);
	if (file == NULL)
		return FALSE;
	
	ASSERT(file->IsOpen());
	if (!file->IsOpen())
		return FALSE;
	
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
		
		return FALSE;
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
			return FALSE;
		}
		
		// copy from STB "owned" memory to our own, then free up the STB stuff
		// (this is kind of unnecessary, but I'd like this method to be the
		// *only* one that knows/cares about stb_image at all)
		size_t pixelsLength = (width * height) * componentsPerPixel;
		m_pixels = new uint8_t[pixelsLength];
		memcpy(m_pixels, pixels, pixelsLength);
		stbi_image_free(pixels);
		
		m_width = width;
		m_height = height;
		m_bpp = bpp;
		m_format = format;
		m_pitch = width * componentsPerPixel;		
		
		return TRUE;
	}
}

Color Image::GetColor(uint16_t x, uint16_t y) const
{
	STACK_TRACE;
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

void Image::SetColor(uint16_t x, uint16_t y, const Color &color)
{
	STACK_TRACE;
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

void Image::Copy(const Image *source, uint16_t destX, uint16_t destY)
{
	STACK_TRACE;
	ASSERT(source != NULL);
	Copy(source, 0, 0, source->GetWidth() - 1, source->GetHeight() - 1, destX, destY);
}

void Image::Copy(const Image *source, uint16_t left, uint16_t top, uint16_t right, uint16_t bottom, uint16_t destX, uint16_t destY)
{
	STACK_TRACE;
	ASSERT(source != NULL);
	ASSERT(source->GetBpp() == m_bpp);
	ASSERT(right > left);
	ASSERT(bottom > top);
	ASSERT(left < source->GetWidth());
	ASSERT(right < source->GetWidth());
	ASSERT(top < source->GetHeight());
	ASSERT(bottom < source->GetHeight());
	ASSERT(destX < m_width);
	ASSERT(destY < m_height);
	ASSERT((right - left) < m_width);
	ASSERT((bottom - top) < m_height);
	ASSERT(destX + (right - left) < m_width);
	ASSERT(destY + (bottom - top) < m_height);

	uint8_t *sourcePixels = source->GetPixels() + source->GetOffsetFor(left, top);
	uint8_t *destPixels = m_pixels + GetOffsetFor(destX, destY);

	size_t lineWidthInBytes = ((right - left) + 1) * (m_bpp / 8);
	uint16_t numLinesToCopy = (bottom - top) + 1;

	for (uint16_t i = 0; i < numLinesToCopy; ++i)
	{
		memcpy(destPixels, sourcePixels, lineWidthInBytes);
		sourcePixels += source->GetPitch();
		destPixels += m_pitch;
	}
}

void Image::Clear()
{
	STACK_TRACE;
	memset(m_pixels, 0, GetSizeInBytes());
}

void Image::Clear(const Color &color)
{
	STACK_TRACE;
	ASSERT(m_format == IMAGE_FORMAT_RGB || m_format == IMAGE_FORMAT_RGBA);

	uint32_t sizeInBytes = GetSizeInBytes();
	uint8_t *pixel = m_pixels;

	if (m_format == IMAGE_FORMAT_RGB)
	{
		ASSERT(sizeInBytes % 3 == 0);
		uint8_t dummy;
		for (uint32_t i = 0; i < sizeInBytes; i += 3)
		{
			color.ToInts(pixel, pixel + 1, pixel + 2, &dummy);
			pixel += 3;
		}
	}
	else
	{
		ASSERT(sizeInBytes % 4 == 0);
		for (uint32_t i = 0; i < sizeInBytes; i += 4)
		{
			color.ToInts(pixel, pixel + 1, pixel + 2, pixel + 3);
			pixel += 4;
		}
	}
}

void Image::Clear(const uint32_t color)
{
	STACK_TRACE;
	Clear(Color::FromInt(color));
}

void Image::Clear(const uint8_t alpha)
{
	STACK_TRACE;
	ASSERT(m_format == IMAGE_FORMAT_ALPHA);
	memset(m_pixels, alpha, GetSizeInBytes());
}

void Image::FlipVertically()
{
	STACK_TRACE;
	ASSERT(m_pixels != NULL);
	if (m_pixels == NULL)
		return;
	
	// TODO: this is a naive implementation. fix so it can flip the image
	//       one scanline at a time using the already allocated memory
	
	size_t pixelsLength = (m_width * m_height) * (m_bpp / 8);
	uint8_t *flippedPixels = new uint8_t[pixelsLength];

	uint8_t *source = (m_pixels + pixelsLength) - m_pitch;   // first pixel of the last line
	uint8_t *dest = flippedPixels;                           // first pixel of the first line
	
	for (uint16_t y = 0; y < m_height; ++y)
	{
		memcpy(dest, source, m_pitch);
		source -= m_pitch;                      // go up one line
		dest += m_pitch;                        // go down one line
	}
	
	SAFE_DELETE(m_pixels);
	m_pixels = flippedPixels;
}
