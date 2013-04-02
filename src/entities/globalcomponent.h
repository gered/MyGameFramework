#ifndef __ENTITIES_GLOBALCOMPONENT_H_INCLUDED__
#define __ENTITIES_GLOBALCOMPONENT_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/util/typesystem.h"

typedef const char* GLOBAL_COMPONENT_TYPE;

class GlobalComponent
{
public:
	TYPE_BASE(GLOBAL_COMPONENT_TYPE);
	
	virtual ~GlobalComponent();
	virtual void Reset();
};

inline GlobalComponent::~GlobalComponent()
{
}

inline void GlobalComponent::Reset()
{
}

#endif

