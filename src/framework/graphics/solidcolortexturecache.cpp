#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "solidcolortexturecache.h"
#include "color.h"
#include "graphicsdevice.h"
#include "image.h"
#include "texture.h"
#include <stl/map.h>

SolidColorTextureCache::SolidColorTextureCache(GraphicsDevice *graphicsDevice)
{
	m_graphicsDevice = graphicsDevice;
}

SolidColorTextureCache::~SolidColorTextureCache()
{
	FreeAll();
}

void SolidColorTextureCache::OnNewContext()
{
	LOG_INFO(LOGCAT_ASSETS, "SolidColorTextureCache: regenerating previous textures for new OpenGL context.\n");

	for (ColorTextureMap::iterator itor = m_textures.begin(); itor != m_textures.end(); ++itor)
	{
		Color color = Color::FromInt(itor->first);
		Texture *reloaded = CreateFor(color, itor->second);
		if (reloaded == NULL)
			LOG_WARN(LOGCAT_ASSETS, "SolidColorTextureCache: failed reloading texture for color 0x%8x.\n", color.ToInt());
	}
}

void SolidColorTextureCache::OnLostContext()
{
	LOG_INFO(LOGCAT_ASSETS, "SolidColorTextureCache: resetting generated texture IDs due to lost OpenGL context.\n");

	for (ColorTextureMap::iterator itor = m_textures.begin(); itor != m_textures.end(); ++itor)
		itor->second->OnLostContext();
}

Texture* SolidColorTextureCache::Get(const Color &color)
{
	Texture *texture;
	uint32_t colorInt = color.ToInt();

	ColorTextureMap::iterator itor = m_textures.find(colorInt);
	if (itor != m_textures.end())
		texture = itor->second;
	else
	{
		texture = CreateFor(color);
		if (texture != NULL)
			m_textures[colorInt] = texture;
	}

	return texture;
}

Texture* SolidColorTextureCache::CreateFor(const Color &color, Texture *existing)
{
	LOG_INFO(LOGCAT_ASSETS, "SolidColorTextureCache: creating texture for color 0x%8x.\n", color.ToInt());

	Image *img = new Image();
	bool imageCreateSuccess = img->Create(8, 8, IMAGE_FORMAT_RGBA);
	ASSERT(imageCreateSuccess == true);

	Texture *texture;
	if (existing != NULL)
	{
		texture = existing;
		texture->Release();
	}
	else
		texture = new Texture();

	img->Clear(color);	
	bool success = texture->Create(m_graphicsDevice, img);
	SAFE_DELETE(img);

	if (!success)
	{
		// if we allocated the texture ourselves, we should free it
		if (existing != NULL)
		{
			SAFE_DELETE(texture);
		}
	}

	return texture;
}

void SolidColorTextureCache::FreeAll()
{
	LOG_INFO(LOGCAT_ASSETS, "SolidColorTextureCache: freeing all generated textures.\n");
	for (ColorTextureMap::iterator itor = m_textures.begin(); itor != m_textures.end(); ++itor)
	{
		SAFE_DELETE(itor->second);
	}

	m_textures.clear();
}
