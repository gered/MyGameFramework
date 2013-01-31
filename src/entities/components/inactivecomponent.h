#ifndef __ENTITIES_COMPONENTS_INACTIVECOMPONENT_H_INCLUDED__
#define __ENTITIES_COMPONENTS_INACTIVECOMPONENT_H_INCLUDED__

#include "../component.h"

class InactiveComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "InactiveComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
};

#endif

