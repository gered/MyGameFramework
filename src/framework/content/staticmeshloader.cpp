#include "../debug.h"
#include "../log.h"

#include "staticmeshloader.h"

#include "contentmanager.h"
#include "textureloader.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../file/file.h"
#include "../file/filesystem.h"
#include "../assets/static/staticmesh.h"
#include "../assets/static/staticmeshfile.h"
#include <stl/string.h>

const char* LOGGING_TAG = "StaticMeshLoader";

StaticMeshLoader::StaticMeshLoader(ContentManager *contentManager)
	: ContentLoaderMapStoreBase<StaticMesh>(LOGGING_TAG, contentManager, "assets://meshes/")
{
	STACK_TRACE;
}

StaticMeshLoader::StaticMeshLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoaderMapStoreBase<StaticMesh>(LOGGING_TAG, contentManager, defaultPath)
{
	STACK_TRACE;
}

StaticMeshLoader::~StaticMeshLoader()
{
	STACK_TRACE;
}

StaticMesh* StaticMeshLoader::LoadContent(const stl::string &file, const ContentParam *params)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "%s: loading \"%s\"\n",
			 GetLoggingTag(),
			 file.c_str()
			 );

	// load the file data
	File *f = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(file, FILEMODE_READ | FILEMODE_BINARY);
	StaticMeshFile *meshFile = new StaticMeshFile(f);
	ASSERT(meshFile != NULL);

	// convert it into a mesh object
	StaticMesh *mesh = new StaticMesh(meshFile, GetContentManager());
	SAFE_DELETE(meshFile);

	// done with the file now
	SAFE_DELETE(f);

	return mesh;
}

void StaticMeshLoader::FreeContent(StaticMesh *content)
{
	STACK_TRACE;
	SAFE_DELETE(content);
}
