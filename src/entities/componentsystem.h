#ifndef __ENTITIES_COMPONENTSYSTEM_H_INCLUDED__
#define __ENTITIES_COMPONENTSYSTEM_H_INCLUDED__

#include "../framework/common.h"
#include "../events/eventlistenerex.h"

class EntityManager;
class EventManager;
class RenderContext;
struct Event;

typedef const char* COMPONENTSYSTEM_TYPE;

class ComponentSystem : public EventListenerEx
{
public:
	virtual ~ComponentSystem();

	virtual void OnLostContext()                           {}
	virtual void OnNewContext()                            {}
	virtual void OnRender(RenderContext *renderContext)    {}
	virtual void OnResize()                                {}
	virtual void OnUpdate(float delta)                     {}

	virtual BOOL Handle(const Event *event);

	virtual COMPONENTSYSTEM_TYPE GetTypeOf() const = 0;

	template<class T> BOOL Is() const;
	BOOL Is(COMPONENTSYSTEM_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;

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

inline BOOL ComponentSystem::Handle(const Event *event)
{
	return FALSE;
}

template<class T>
inline BOOL ComponentSystem::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL ComponentSystem::Is(COMPONENTSYSTEM_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* ComponentSystem::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* ComponentSystem::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif

