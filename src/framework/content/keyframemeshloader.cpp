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
}

KeyframeMeshLoader::KeyframeMeshLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoaderMapStoreBase<KeyframeMesh>(LOGGING_TAG, contentManager, defaultPath)
{
}

KeyframeMeshLoader::~KeyframeMeshLoader()
{
}

KeyframeMesh* KeyframeMeshLoader::LoadContent(const stl::string &file, const ContentParam *params)
{
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
	SAFE_DELETE(content);
}

