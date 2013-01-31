#include "../debug.h"
#include "../log.h"

#include "skeletalmeshloader.h"

#include "contentmanager.h"
#include "../basegameapp.h"
#include "../operatingsystem.h"
#include "../assets/animation/skeletalmesh.h"
#include "../assets/animation/skeletalmeshfile.h"
#include "../file/file.h"
#include "../file/filesystem.h"
#include <stl/string.h>

#define LOGGING_TAG "SkeletalMeshLoader"

SkeletalMeshLoader::SkeletalMeshLoader(ContentManager *contentManager)
	: ContentLoaderMapStoreBase<SkeletalMesh>(LOGGING_TAG, contentManager, "assets://meshes/")
{
	STACK_TRACE;
}

SkeletalMeshLoader::SkeletalMeshLoader(ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoaderMapStoreBase<SkeletalMesh>(LOGGING_TAG, contentManager, defaultPath)
{
	STACK_TRACE;
}

SkeletalMeshLoader::~SkeletalMeshLoader()
{
	STACK_TRACE;
}

SkeletalMesh* SkeletalMeshLoader::LoadContent(const stl::string &file, const ContentParam *params)
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_ASSETS, "%s: loading \"%s\"\n", 
			 GetLoggingTag(), 
			 file.c_str()
			 );
	
	// load the file data
	File *f = GetContentManager()->GetGameApp()->GetOperatingSystem()->GetFileSystem()->Open(file, FILEMODE_READ | FILEMODE_BINARY);
	SkeletalMeshFile *meshFile = new SkeletalMeshFile(f);
	ASSERT(meshFile != NULL);
	
	// convert it into a mesh object
	SkeletalMesh *mesh = meshFile->CreateMesh();
	SAFE_DELETE(meshFile);
	
	// close the file
	SAFE_DELETE(f);
	
	return mesh;
}

void SkeletalMeshLoader::FreeContent(SkeletalMesh *content)
{
	STACK_TRACE;
	SAFE_DELETE(content);
}

