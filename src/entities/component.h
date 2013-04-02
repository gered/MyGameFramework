#ifndef __ENTITIES_COMPONENT_H_INCLUDED__
#define __ENTITIES_COMPONENT_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/util/typesystem.h"

typedef const char* COMPONENT_TYPE;

class Component
{
public:
	TYPE_BASE(COMPONENT_TYPE);
	
	virtual ~Component();
	virtual void Reset();
};

inline Component::~Component()
{
}

inline void Component::Reset()
{
}

#endif

