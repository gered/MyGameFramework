#ifndef __FRAMEWORK_CONTENT_SKELETALMESHLOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_SKELETALMESHLOADER_H_INCLUDED__

#include "../common.h"

#include "contentparam.h"
#include "contentloadermapstorebase.h"
#include "../assets/animation/skeletalmesh.h"
#include <stl/string.h>

class ContentManager;

class SkeletalMeshLoader : public ContentLoaderMapStoreBase<SkeletalMesh>
{
public:
	SkeletalMeshLoader(ContentManager *contentManager);
	SkeletalMeshLoader(ContentManager *contentManager, const stl::string &defaultPath);
	virtual ~SkeletalMeshLoader();

protected:
	SkeletalMesh* LoadContent(const stl::string &file, const ContentParam *params);
	void FreeContent(SkeletalMesh *content);
};

#endif
