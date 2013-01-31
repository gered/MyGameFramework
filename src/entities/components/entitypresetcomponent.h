#ifndef __GAME_ENTITIES_COMPONENTS_ENTITYPRESETCOMPONENT_H_INCLUDED__
#define __GAME_ENTITIES_COMPONENTS_ENTITYPRESETCOMPONENT_H_INCLUDED__

#include "../component.h"
#include "../entitypreset.h"

class EntityPresetComponent : public Component
{
public:
	static COMPONENT_TYPE GetType()
	{
		static COMPONENT_TYPE typeName = "EntityPresetComponent";
		return typeName;
	}
	COMPONENT_TYPE GetTypeOf() const                       { return GetType(); }
	
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

