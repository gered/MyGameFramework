#ifndef __ENTITIES_COMPONENTS_INACTIVECOMPONENT_H_INCLUDED__
#define __ENTITIES_COMPONENTS_INACTIVECOMPONENT_H_INCLUDED__

#include "../component.h"
#include "../../framework/util/typesystem.h"

class InactiveComponent : public Component
{
public:
	TYPE_DEFINE(COMPONENT_TYPE, "InactiveComponent");
};

#endif

