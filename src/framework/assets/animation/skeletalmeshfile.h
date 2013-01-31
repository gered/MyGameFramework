#ifndef __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHFILE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHFILE_H_INCLUDED__

#include "../../common.h"
#include "../meshfile.h"

class File;
class SkeletalMesh;

class SkeletalMeshFile : public MeshFile
{
public:
	SkeletalMeshFile(File *file);
	virtual ~SkeletalMeshFile();
	
	SkeletalMesh* CreateMesh();
};

#endif
