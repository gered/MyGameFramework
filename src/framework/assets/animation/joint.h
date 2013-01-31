#ifndef __FRAMEWORK_ASSETS_ANIMATION_JOINT_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_JOINT_H_INCLUDED__

#include "../../common.h"
#include "jointkeyframe.h"
#include "../../math/matrix4x4.h"
#include <stl/string.h>

const int32_t NO_JOINT = -1;

struct Joint
{
	Joint();
	virtual ~Joint();

	stl::string name;
	Joint *parent;
	int32_t parentIndex;
	Matrix4x4 relative;
	Matrix4x4 absolute;
	JointKeyFrame *frames;
};

inline Joint::Joint()
{
	frames = NULL;
	parent = NULL;
	parentIndex = NO_JOINT;
}

inline Joint::~Joint()
{
	SAFE_DELETE_ARRAY(frames);
}

#endif
