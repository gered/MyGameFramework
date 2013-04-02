#ifndef __ENTITIES_ENTITYMANAGER_H_INCLUDED__
#define __ENTITIES_ENTITYMANAGER_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/debug.h"

#include <stl/list.h>
#include <stl/map.h>
#include <stl/set.h>

#include "component.h"
#include "componentsystem.h"
#include "entitypreset.h"
#include "globalcomponent.h"

class Entity;
class EventManager;
class RenderContext;

typedef stl::list<Entity*> EntityList;
typedef stl::list<Component*> ComponentList;
typedef stl::set<Entity*> EntitySet;
typedef stl::map<const Entity*, Component*> EntityComponentsMap;
typedef stl::map<COMPONENT_TYPE, EntityComponentsMap> ComponentStore;
typedef stl::list<ComponentSystem*> ComponentSystemList;
typedef stl::map<ENTITYPRESET_TYPE, EntityPreset*> EntityPresetMap;
typedef stl::map<GLOBAL_COMPONENT_TYPE, GlobalComponent*> GlobalComponentStore;

class EntityManager
{
public:
	EntityManager(EventManager *eventManager);
	virtual ~EntityManager();

	template<class T> T* AddSubsystem();
	template<class T, class TBefore> T* AddSubsystemBefore();
	template<class T> T* GetSubsystem() const;
	template<class T> void RemoveSubsystem();
	void RemoveAllSubsystems();

	template<class T> void AddPreset();
	template<class T> void RemovePreset();
	void RemoveAllPresets();

	Entity* Add();
	template<class T> Entity* AddUsingPreset(EntityPresetArgs *args = NULL);
	Entity* AddUsingPreset(ENTITYPRESET_TYPE type, EntityPresetArgs *args = NULL);
	template<class T> Entity* GetWith() const;
	template<class T> void GetAllWith(EntityList &matches) const;
	template<class T> BOOL WasCreatedUsingPreset(Entity *entity) const;
	BOOL WasCreatedUsingPreset(const Entity *entity, ENTITYPRESET_TYPE type) const;
	void Remove(Entity *entity);
	void RemoveAll();
	BOOL IsValid(const Entity *entity) const;
	uint GetNumEntities() const                        { return m_entities.size(); }

	template<class T> T* AddComponent(Entity *entity);
	template<class T> T* GetComponent(const Entity *entity) const;
	template<class T> void RemoveComponent(Entity *entity);
	template<class T> BOOL HasComponent(const Entity *entity) const;
	void GetAllComponentsFor(const Entity *entity, ComponentList &list) const;

	template<class T> T* AddGlobalComponent();
	template<class T> T* GetGlobalComponent() const;
	template<class T> void RemoveGlobalComponent();
	template<class T> BOOL HasGlobalComponent() const;
	void RemoveAllGlobalComponents();

	void OnLostContext();
	void OnNewContext();
	void OnRender(RenderContext *renderContext);
	void OnResize();
	void OnUpdate(float delta);

	EventManager* GetEventManager() const                  { return m_eventManager; }

private:
	void RemoveAllComponentsFrom(Entity *entity);
	template<class T> ComponentSystemList::const_iterator FindSubsystem() const;

	EntitySet m_entities;
	ComponentStore m_components;
	GlobalComponentStore m_globalComponents;
	ComponentSystemList m_componentSystems;
	EntityPresetMap m_entityPresets;
	EventManager *m_eventManager;
};

template<class T>
T* EntityManager::AddSubsystem()
{
	ASSERT(GetSubsystem<T>() == NULL);
	T* subsystem = new T(this, m_eventManager);
	m_componentSystems.push_back(subsystem);
	return subsystem;
}

template<class T, class TBefore> 
T* EntityManager::AddSubsystemBefore()
{
	ASSERT(GetSubsystem<T>() == NULL);
	ComponentSystemList::const_iterator i = FindSubsystem<TBefore>();
	ASSERT(i != m_componentSystems.end());
	T* subsystem = new T(this, m_eventManager);
	m_componentSystems.insert(i, subsystem);
	return subsystem;
}

template<class T>
T* EntityManager::GetSubsystem() const
{
	ComponentSystemList::const_iterator i = FindSubsystem<T>();
	if (i == m_componentSystems.end())
		return NULL;
	else
		return (*i)->As<T>();
}

template<class T>
void EntityManager::RemoveSubsystem()
{
	ComponentSystemList::iterator i = FindSubsystem<T>();
	if (i == m_componentSystems.end())
		return;

	SAFE_DELETE(*i);
	m_componentSystems.erase(i);
}

template<class T>
void EntityManager::AddPreset()
{
	T* preset = new T(this);
	EntityPresetMap::const_iterator i = m_entityPresets.find(T::GetType());
	ASSERT(i == m_entityPresets.end());
	if (i != m_entityPresets.end())
		return;

	m_entityPresets[T::GetType()] = preset;
}

template<class T>
void EntityManager::RemovePreset()
{
	EntityPresetMap::iterator i = m_entityPresets.find(T::GetType());
	if (i == m_entityPresets.end())
		return;
	
	EntityPreset *preset = i->second;
	SAFE_DELETE(preset);
	
	m_entityPresets.erase(i);
}

template<class T>
Entity* EntityManager::AddUsingPreset(EntityPresetArgs *args)
{
	return AddUsingPreset(T::GetType(), args);
}

template<class T>
T* EntityManager::AddComponent(Entity *entity)
{
	ASSERT(GetComponent<T>(entity) == NULL);
	T* component = new T();
	m_components[T::GetType()].insert(EntityComponentsMap::value_type(entity, component));
	return component;
}

template <class T>
T* EntityManager::GetComponent(const Entity *entity) const
{
	ComponentStore::const_iterator i = m_components.find(T::GetType());
	if (i == m_components.end())
		return NULL;

	const EntityComponentsMap &entitiesWithComponent = i->second;
	EntityComponentsMap::const_iterator j = entitiesWithComponent.find(entity);
	if (j == entitiesWithComponent.end())
		return NULL;
	else
		return j->second->As<T>();
}

template<class T>
void EntityManager::RemoveComponent(Entity *entity)
{
	ComponentStore::iterator i = m_components.find(T::GetType());
	if (i == m_components.end())
		return;

	EntityComponentsMap &entitiesWithComponent = i->second;
	EntityComponentsMap::iterator j = entitiesWithComponent.find(entity);
	if (j == entitiesWithComponent.end())
		return;
	else
	{
		SAFE_DELETE(j->second);
		entitiesWithComponent.erase(entity);
	}
}

template<class T>
BOOL EntityManager::HasComponent(const Entity *entity) const
{
	ComponentStore::const_iterator i = m_components.find(T::GetType());
	if (i == m_components.end())
		return FALSE;

	const EntityComponentsMap &entitiesWithComponent = i->second;
	EntityComponentsMap::const_iterator j = entitiesWithComponent.find(entity);
	if (j == entitiesWithComponent.end())
		return FALSE;
	else
		return TRUE;
}

template<class T>
T* EntityManager::AddGlobalComponent()
{
	ASSERT(GetGlobalComponent<T>() == NULL);
	T* newComponent = new T();
	m_globalComponents.insert(GlobalComponentStore::value_type(T::GetType(), newComponent));
	return newComponent;
}

template<class T>
T* EntityManager::GetGlobalComponent() const
{
	GlobalComponentStore::const_iterator i = m_globalComponents.find(T::GetType());
	if (i == m_globalComponents.end())
		return NULL;
	else
		return i->second->As<T>();
}

template<class T>
void EntityManager::RemoveGlobalComponent()
{
	GlobalComponentStore::iterator i = m_globalComponents.find(T::GetType());
	if (i == m_globalComponents.end())
		return;

	SAFE_DELETE(i->second);
	m_globalComponents.erase(i);
}

template<class T>
BOOL EntityManager::HasGlobalComponent() const
{
	GlobalComponentStore::const_iterator i = m_globalComponents.find(T::GetType());
	if (i == m_globalComponents.end())
		return FALSE;
	else
		return TRUE;
}

template<class T>
Entity* EntityManager::GetWith() const
{
	ComponentStore::const_iterator i = m_components.find(T::GetType());
	if (i == m_components.end())
		return NULL;

	const EntityComponentsMap &entitiesWithComponent = i->second;
	if (!entitiesWithComponent.empty())
		return (Entity*)entitiesWithComponent.begin()->first;
	else
		return NULL;
}

template<class T>
void EntityManager::GetAllWith(EntityList &matches) const
{
	ComponentStore::const_iterator i = m_components.find(T::GetType());
	if (i == m_components.end())
		return;

	const EntityComponentsMap &entitiesWithComponent = i->second;
	for (EntityComponentsMap::const_iterator j = entitiesWithComponent.begin(); j != entitiesWithComponent.end(); ++j)
		matches.push_back((Entity*)j->first);
}

template<class T>
BOOL EntityManager::WasCreatedUsingPreset(Entity *entity) const
{
	return WasCreatedUsingPreset(entity, T::GetType());
}

template<class T>
ComponentSystemList::const_iterator EntityManager::FindSubsystem() const
{
	ComponentSystemList::const_iterator i = m_componentSystems.begin();
	while (i != m_componentSystems.end())
	{
		ComponentSystem *system = *i;
		if (system->Is<T>())
			return i;

		++i;
	}

	return i;
}

#endif

