#ifndef __ENTITIES_COMPONENTSYSTEM_H_INCLUDED__
#define __ENTITIES_COMPONENTSYSTEM_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/util/typesystem.h"
#include "../events/eventlistenerex.h"

class EntityManager;
class EventManager;
class RenderContext;
struct Event;

typedef const char* COMPONENTSYSTEM_TYPE;

class ComponentSystem : public EventListenerEx
{
public:
	TYPE_BASE(COMPONENTSYSTEM_TYPE);
	
	virtual ~ComponentSystem();

	virtual void OnLostContext()                           {}
	virtual void OnNewContext()                            {}
	virtual void OnRender(RenderContext *renderContext)    {}
	virtual void OnResize()                                {}
	virtual void OnUpdate(float delta)                     {}

	virtual bool Handle(const Event *event);

protected:
	ComponentSystem(EntityManager *entityManager, EventManager *eventManager);

	EntityManager* GetEntityManager() const                { return m_entityManager; }

private:
	EntityManager *m_entityManager;
};

inline ComponentSystem::ComponentSystem(EntityManager *entityManager, EventManager *eventManager)
	: EventListenerEx(eventManager)
{
	m_entityManager = entityManager;
}

inline ComponentSystem::~ComponentSystem()
{
}

inline bool ComponentSystem::Handle(const Event *event)
{
	return false;
}

#endif

