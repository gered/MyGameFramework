#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "textureloader.h"

#include "contentmanager.h"
#include "imageloader.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../graphics/graphicsdevice.h"
#include "../graphics/image.h"
#include "../graphics/texture.h"
#include "../file/file.h"
#include "../file/filesystem.h"
#include <stl/string.h>

#define LOGGING_TAG "TextureLoader"

TextureLoader::TextureLoader(ContentManager *contentManager)
	: ContentLoaderMapStoreBase<Texture>(LOGGING_TAG, contentManager, "assets://textures/")
{
}

TextureLoader::TextureLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoaderMapStoreBase<Texture>(LOGGING_TAG, contentManager, defaultPath)
{
}

TextureLoader::~TextureLoader()
{
}

void TextureLoader::OnNewContext()
{
	LOG_INFO(LOGCAT_ASSETS, "%s: reloading previous textures for new OpenGL context.\n", GetLoggingTag());

	for (ContentStoreItor itor = m_content.begin(); itor != m_content.end(); ++itor)
	{
		Texture *reloaded = Load(itor->first, itor->second.content);
		if (reloaded == NULL)
		{
			LOG_WARN(LOGCAT_ASSETS, "%s: failed reloading texture \"%s\"\n",
					 GetLoggingTag(),
					 itor->first.c_str()
					 );
		}
	}
}

void TextureLoader::OnLostContext()
{
	LOG_INFO(LOGCAT_ASSETS, "%s: resetting loaded texture IDs due to lost OpenGL context.\n", GetLoggingTag());

	for (ContentStoreItor itor = m_content.begin(); itor != m_content.end(); ++itor)
		itor->second.content->OnLostContext();
}

Texture* TextureLoader::LoadContent(const stl::string &file, const ContentParam *params)
{
	return Load(file, NULL);
}

void TextureLoader::FreeContent(Texture *content)
{
	SAFE_DELETE(content);
}

Texture* TextureLoader::Load(const stl::string &file, Texture *existingTexture)
{
	LOG_INFO(LOGCAT_ASSETS, "%s: loading \"%s\"\n", 
			 GetLoggingTag(),
			 file.c_str()
			 );

	Image *image = GetContentManager()->Get<Image>(file.c_str());
	ASSERT(image != NULL);
	if (image == NULL)
		return NULL;
	
	// use existing texture object given, or create a new one
	Texture *texture = existingTexture;
	if (texture == NULL)
		texture = new Texture();
	
	// load image into it
	bool success = texture->Create(GetContentManager()->GetGameApp()->GetGraphicsDevice(), image);
	
	// don't need to keep the underlying image around if not being used elsewhere
	GetContentManager()->Free<Image>(image);

	// if texture creation fails and we allocated the texture object ourselves, 
	// we should free it ourselves. existing texture objects are the caller's
	// responsibility of course
	if (!success && existingTexture != NULL)
	{
		SAFE_DELETE(texture);
	}

	return texture;
}
