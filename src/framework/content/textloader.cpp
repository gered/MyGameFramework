#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "textloader.h"

#include "contentmanager.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../support/text.h"
#include "../file/file.h"
#include "../file/filesystem.h"
#include <stl/string.h>

#define LOGGING_TAG "TextLoader"

TextLoader::TextLoader(ContentManager *contentManager)
	: ContentLoaderMapStoreBase<Text>(LOGGING_TAG, contentManager, "")
{
	STACK_TRACE;
}

TextLoader::TextLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoaderMapStoreBase<Text>(LOGGING_TAG, contentManager, defaultPath)
{
	STACK_TRACE;
}

TextLoader::~TextLoader()
{
	STACK_TRACE;
}

Text* TextLoader::LoadContent(const stl::string &file, const ContentParam *params)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "%s: loading \"%s\"\n", 
			 GetLoggingTag(),
			 file.c_str()
			 );

	File *textFile = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(file, FILEMODE_READ);
	ASSERT(textFile != NULL);

	Text *text = Text::CreateFrom(textFile);
	ASSERT(text != NULL);

	SAFE_DELETE(textFile);

	return text;
}

void TextLoader::FreeContent(Text *content)
{
	STACK_TRACE;
	SAFE_DELETE(content);
}
