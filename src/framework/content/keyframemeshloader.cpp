#include "../debug.h"
#include "../log.h"

#include "keyframemeshloader.h"

#include "contentmanager.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../assets/animation/keyframemesh.h"
#include "../assets/animation/keyframemeshfile.h"
#include "../file/file.h"
#include "../file/filesystem.h"
#include <stl/string.h>

#define LOGGING_TAG "KeyframeMeshLoader"

KeyframeMeshLoader::KeyframeMeshLoader(ContentManager *contentManager)
	: ContentLoaderMapStoreBase<KeyframeMesh>(LOGGING_TAG, contentManager, "assets://meshes/")
{
	STACK_TRACE;
}

KeyframeMeshLoader::KeyframeMeshLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoaderMapStoreBase<KeyframeMesh>(LOGGING_TAG, contentManager, defaultPath)
{
	STACK_TRACE;
}

KeyframeMeshLoader::~KeyframeMeshLoader()
{
	STACK_TRACE;
}

KeyframeMesh* KeyframeMeshLoader::LoadContent(const stl::string &file, const ContentParam *params)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "%s: loading \"%s\"\n", 
			 GetLoggingTag(), 
			 file.c_str()
			 );

	// load the file data
	File *f = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(file, FILEMODE_READ | FILEMODE_BINARY);
	KeyframeMeshFile *meshFile = new KeyframeMeshFile(f);
	ASSERT(meshFile != NULL);

	// convert it into a mesh object
	KeyframeMesh *mesh = new KeyframeMesh(meshFile);
	SAFE_DELETE(meshFile);

	// close the file
	SAFE_DELETE(f);

	return mesh;
}

void KeyframeMeshLoader::FreeContent(KeyframeMesh *content)
{
	STACK_TRACE;
	SAFE_DELETE(content);
}

