#include "../debug.h"
#include "../log.h"

#include "../common.h"
#include "texture.h"
#include "glincludes.h"
#include "glutils.h"
#include "graphicsdevice.h"
#include "image.h"
#include "textureparameters.h"
#include "../math/mathhelpers.h"

Texture::Texture()
{
	m_graphicsDevice = NULL;
	m_textureName = 0;
	m_width = 0;
	m_height = 0;
	m_format = TEXTURE_FORMAT_NONE;
}

Texture::~Texture()
{
	Release();
}

BOOL Texture::Create(GraphicsDevice *graphicsDevice, Image *image)
{
	ASSERT(m_textureName == 0);
	if (m_textureName != 0)
		return FALSE;
	
	ASSERT(graphicsDevice != NULL);
	ASSERT(IsPowerOf2(image->GetWidth()) && IsPowerOf2(image->GetHeight()));
	ASSERT(image->GetBpp() != 8 || image->GetFormat() == IMAGE_FORMAT_ALPHA);
	ASSERT(image->GetBpp() != 16);

	const void* pixels = image->GetPixels();
	TEXTURE_FORMAT format = TEXTURE_FORMAT_NONE;
	uint32_t glFormat = 0;
	uint32_t glType = GL_UNSIGNED_BYTE;
	
	if (image->GetFormat() == IMAGE_FORMAT_ALPHA)
	{
		glFormat = GL_ALPHA;
		format = TEXTURE_FORMAT_ALPHA;
	}
	else
	{
		if (image->GetBpp() == 24)
		{
			glFormat = GL_RGB;
			format = TEXTURE_FORMAT_RGB;
		}
		else if (image->GetBpp() == 32)
		{
			glFormat = GL_RGBA;
			format = TEXTURE_FORMAT_RGBA;
		}
	}
	ASSERT(glFormat != 0);
	if (glFormat == 0)
		return FALSE;

	m_graphicsDevice = graphicsDevice;
	m_width = image->GetWidth();
	m_height = image->GetHeight();
	m_format = format;
	
	GL_CALL(glGenTextures(1, &m_textureName));
	
	m_graphicsDevice->BindTexture(this, 0);
	m_graphicsDevice->GetTextureParameters()->Apply();

	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, glFormat, image->GetWidth(), image->GetHeight(), 0, glFormat, glType, pixels));

	LOG_INFO(LOGCAT_GRAPHICS, "Created texture from image. ID = %d, bpp = %d, size = %d x %d\n", m_textureName, image->GetBpp(), image->GetWidth(), image->GetHeight());
	
	return TRUE;
}

BOOL Texture::Create(GraphicsDevice *graphicsDevice, uint16_t width, uint16_t height, TEXTURE_FORMAT textureFormat)
{
	ASSERT(m_textureName == 0);
	if (m_textureName != 0)
		return FALSE;
		
	ASSERT(graphicsDevice != NULL);
	if (!graphicsDevice->IsNonPowerOfTwoTextureSupported())
	{
		ASSERT(IsPowerOf2(width) && IsPowerOf2(height));
		if (!IsPowerOf2(width) || !IsPowerOf2(height))
			return FALSE;
	}
	
	int bpp = 0;
	uint32_t format = 0;
	uint32_t type = 0;
	GetTextureSpecsFromFormat(textureFormat, &bpp, &format, &type);
	ASSERT(format != 0);
	if (format == 0)
		return FALSE;
	ASSERT(type != 0);
	if (type == 0)
		return FALSE;

	m_graphicsDevice = graphicsDevice;
	m_width = width;
	m_height = height;
	m_format = textureFormat;

	GL_CALL(glGenTextures(1, &m_textureName));
	
	m_graphicsDevice->BindTexture(this, 0);
	m_graphicsDevice->GetTextureParameters()->Apply();
		
	// allocate texture memory, but leave it uninitialized
	GL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, NULL));
	
	if (textureFormat == GL_DEPTH_COMPONENT)
		LOG_INFO(LOGCAT_GRAPHICS, "Created uninitialized texture. ID = %d, depth component only, size = %d x %d\n", m_textureName, m_width, m_height);
	else
		LOG_INFO(LOGCAT_GRAPHICS, "Created uninitialized texture. ID = %d, bpp = %d, size = %d x %d\n", m_textureName, bpp, m_width, m_height);

	return TRUE;
}

void Texture::Release()
{
	if (m_textureName != 0)
	{
		m_graphicsDevice->UnbindTexture(this);
		GL_CALL(glDeleteTextures(1, &m_textureName));

		LOG_INFO(LOGCAT_GRAPHICS, "Deleted Texture ID = %d.\n", m_textureName);
	}
	
	m_textureName = 0;
	m_graphicsDevice = NULL;
	m_width = 0;
	m_height = 0;
	m_format = TEXTURE_FORMAT_NONE;
}

BOOL Texture::Update(Image *image, uint16_t destX, uint16_t destY)
{
	ASSERT(m_textureName != 0);
	if (m_textureName == 0)
		return FALSE;
	
	// TODO: for now ...
	ASSERT(m_format != TEXTURE_FORMAT_DEPTH);
	if (m_format == TEXTURE_FORMAT_DEPTH)
		return FALSE;
	
	ASSERT(image != NULL);
	ASSERT(destX < m_width);
	ASSERT(destY < m_height);
	ASSERT(image->GetWidth() <= m_width);
	ASSERT(image->GetHeight() <= m_height);
	ASSERT(destX + image->GetWidth() <= m_width);
	ASSERT(destY + image->GetHeight() <= m_height);
		
	const void* pixels = image->GetPixels();
	uint32_t glFormat = 0;
	uint32_t glType = 0;
	
	if (image->GetFormat() == IMAGE_FORMAT_ALPHA)
	{
		glFormat = GL_ALPHA;
		glType = GL_UNSIGNED_BYTE;
	}
	else
	{
		glType = GL_UNSIGNED_BYTE;
		if (image->GetBpp() == 24)
			glFormat = GL_RGB;
		else if (image->GetBpp() == 32)
			glFormat = GL_RGBA;
	}
	
	ASSERT(glFormat != 0);
	if (glFormat == 0)
		return FALSE;
	
	m_graphicsDevice->BindTexture(this, 0);
	GL_CALL(glTexSubImage2D(GL_TEXTURE_2D, 0, destX, destY, image->GetWidth(), image->GetHeight(), glFormat, glType, pixels));
	
	return TRUE;
}

void Texture::OnLostContext()
{
	m_textureName = 0;
}

void Texture::GetTextureSpecsFromFormat(TEXTURE_FORMAT textureFormat, int *bpp, uint32_t *format, uint32_t *type)
{
	switch (textureFormat)
	{
		case TEXTURE_FORMAT_ALPHA:
			if (bpp)     *bpp = 8;
			if (format)  *format = GL_ALPHA;
			if (type)    *type = GL_UNSIGNED_BYTE;
			break;
		case TEXTURE_FORMAT_RGB:
			if (bpp)     *bpp = 24;
			if (format)  *format = GL_RGB;
			if (type)    *type = GL_UNSIGNED_BYTE;
			break;
		case TEXTURE_FORMAT_RGBA:
			if (bpp)     *bpp = 32;
			if (format)  *format = GL_RGBA;
			if (type)    *type = GL_UNSIGNED_BYTE;
			break;
		case TEXTURE_FORMAT_DEPTH:
			if (bpp)     *bpp = 0;        // doesn't really matter for this one... ?
			if (format)  *format = GL_DEPTH_COMPONENT;
			
			// TODO: check that these are correct ...
#ifdef MOBILE
			if (type)    *type = GL_UNSIGNED_SHORT;
#else
			if (type)    *type = GL_FLOAT;
#endif
			break;
		default:
			if (bpp)     *bpp = 0;
			if (format)  *format = 0;
			if (type)    *type = 0;
	}
}
