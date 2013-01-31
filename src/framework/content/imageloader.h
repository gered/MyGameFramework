#ifndef __FRAMEWORK_CONTENT_IMAGELOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_IMAGELOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloadermapstorebase.h"
#include "../graphics/image.h"
#include <stl/string.h>

class ContentManager;

/**
 * Content loader for images.
 */
class ImageLoader : public ContentLoaderMapStoreBase<Image>
{
public:
	/**
	 * Creates an image loader.
	 * @param contentManager the parent content manager
	 */
	ImageLoader(ContentManager *contentManager);

	/**
	 * Creates an image loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with image filenames
	 */
	ImageLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~ImageLoader();

protected:
	Image* LoadContent(const stl::string &file, const ContentParam *params);
	void FreeContent(Image *content);
};

#endif

