#ifndef __FRAMEWORK_CONTENT_TEXTLOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_TEXTLOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloadermapstorebase.h"
#include "../support/text.h"
#include <stl/string.h>

class ContentManager;

/**
 * Content loader for text files.
 */
class TextLoader : public ContentLoaderMapStoreBase<Text>
{
public:
	/**
	 * Creates a text file loader.
	 * @param contentManager the parent content manager
	 */
	TextLoader(ContentManager *contentManager);

	/**
	 * Creates a text file loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with text filenames
	 */
	TextLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~TextLoader();

protected:
	Text* LoadContent(const stl::string &file, const ContentParam *params);
	void FreeContent(Text *content);	
};

#endif

