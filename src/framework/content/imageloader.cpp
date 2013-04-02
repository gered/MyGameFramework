#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "imageloader.h"

#include "contentmanager.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../graphics/image.h"
#include "../file/file.h"
#include "../file/filesystem.h"

#define LOGGING_TAG "ImageLoader"

ImageLoader::ImageLoader(ContentManager *contentManager)
	: ContentLoaderMapStoreBase<Image>(LOGGING_TAG, contentManager, "assets://images/")
{
}

ImageLoader::ImageLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoaderMapStoreBase<Image>(LOGGING_TAG, contentManager, defaultPath)
{
}

ImageLoader::~ImageLoader()
{
}

Image* ImageLoader::LoadContent(const stl::string &file, const ContentParam *params)
{
	LOG_INFO(LOGCAT_ASSETS, "%s: loading \"%s\"\n", 
			 GetLoggingTag(), 
			 file.c_str()
			 );

	File *imageFile = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(file, FILEMODE_READ | FILEMODE_BINARY);
	ASSERT(imageFile != NULL);

	Image *image = new Image();
	BOOL success = image->Create(imageFile);
	
	SAFE_DELETE(imageFile);

	if (!success)
		SAFE_DELETE(image);
	
	return image;
}

void ImageLoader::FreeContent(Image *content)
{
	SAFE_DELETE(content);
}
