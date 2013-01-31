#ifndef __FRAMEWORK_ASSETS_ANIMATION_JOINTKEYFRAME_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_JOINTKEYFRAME_H_INCLUDED__

#include "../../math/quaternion.h"
#include "../../math/vector3.h"

struct JointKeyFrame
{
	Vector3 position;
	Quaternion rotation;
};

#endif
