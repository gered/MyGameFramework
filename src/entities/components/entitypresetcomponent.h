#ifndef __GAME_ENTITIES_COMPONENTS_ENTITYPRESETCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_ENTITYPRESETCOMPONENT_H_INCLUDED__

#include "../component.h"
#include "../entitypreset.h"
#include "../../framework/util/typesystem.h"

class EntityPresetComponent : public Component
{
public:
	TYPE_DEFINE(COMPONENT_TYPE, "EntityPresetComponent");
	
	EntityPresetComponent();
	void Reset();
	
	ENTITYPRESET_TYPE preset;
};

inline EntityPresetComponent::EntityPresetComponent()
{
	Reset();
}

inline void EntityPresetComponent::Reset()
{
	preset = NULL;
}

#endif

