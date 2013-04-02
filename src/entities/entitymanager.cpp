#include "../framework/debug.h"

#include "entitymanager.h"
#include "componentsystem.h"
#include "entity.h"
#include "entitypreset.h"
#include "components/entitypresetcomponent.h"
#include "components/inactivecomponent.h"

EntityManager::EntityManager(EventManager *eventManager)
{
	m_eventManager = eventManager;
}

EntityManager::~EntityManager()
{
	RemoveAll();
	RemoveAllSubsystems();
	RemoveAllPresets();
	RemoveAllGlobalComponents();
}

void EntityManager::RemoveAllSubsystems()
{
	for (ComponentSystemList::iterator i = m_componentSystems.begin(); i != m_componentSystems.end(); ++i)
		SAFE_DELETE(*i);

	m_componentSystems.clear();
}

void EntityManager::RemoveAllPresets()
{
	while (!m_entityPresets.empty())
	{
		EntityPresetMap::iterator i = m_entityPresets.begin();
		
		EntityPreset *preset = i->second;
		SAFE_DELETE(preset);
		
		m_entityPresets.erase(i);
	}
}

Entity* EntityManager::Add()
{
	Entity *entity = new Entity(this);
	m_entities.insert(entity);
	return entity;
}

Entity* EntityManager::AddUsingPreset(ENTITYPRESET_TYPE preset, EntityPresetArgs *args)
{
	EntityPresetMap::iterator i = m_entityPresets.find(preset);
	ASSERT(i != m_entityPresets.end());
	if (i == m_entityPresets.end())
		return NULL;

	Entity *entity = i->second->Create(args);
	ASSERT(entity != NULL);
	if (entity == NULL)
		return NULL;
	
	entity->Add<EntityPresetComponent>()->preset = preset;
	
	return entity;
}

bool EntityManager::WasCreatedUsingPreset(const Entity *entity, ENTITYPRESET_TYPE type) const
{
	ASSERT(entity != NULL);
	ASSERT(type != NULL);
	EntityPresetComponent *preset = entity->Get<EntityPresetComponent>();
	if (preset != NULL && preset->preset == type)
		return true;
	else
		return false;
}

void EntityManager::Remove(Entity *entity)
{
	ASSERT(entity != NULL);
	if (!IsValid(entity))
		return;

	EntitySet::iterator itor = m_entities.find(entity);

	RemoveAllComponentsFrom(entity);
	
	m_entities.erase(itor);
	SAFE_DELETE(entity);
}

void EntityManager::RemoveAll()
{
	for (EntitySet::iterator i = m_entities.begin(); i != m_entities.end(); ++i)
	{
		Entity *entity = *i;
		RemoveAllComponentsFrom(entity);
		SAFE_DELETE(entity);
	}

	m_entities.clear();
}

void EntityManager::RemoveAllComponentsFrom(Entity *entity)
{
	ASSERT(entity != NULL);
	
	for (ComponentStore::iterator i = m_components.begin(); i != m_components.end(); ++i)
	{
		EntityComponentsMap &entitiesWithComponent = i->second;
		EntityComponentsMap::iterator j = entitiesWithComponent.find(entity);
		if (j != entitiesWithComponent.end())
		{
			SAFE_DELETE(j->second);
			entitiesWithComponent.erase(entity);
		}
	}
}

bool EntityManager::IsValid(const Entity *entity) const
{
	if (entity == NULL)
		return false;
	
	// HACK: lol, this cast is aboslutely terrible and I shouldn't be doing this
	EntitySet::const_iterator i = m_entities.find((Entity*)entity);
	if (i == m_entities.end())
		return false;
	else
		return true;
}

void EntityManager::GetAllComponentsFor(const Entity *entity, ComponentList &list) const
{
	ASSERT(entity != NULL);
	if (!IsValid(entity))
		return;
	
	for (ComponentStore::const_iterator i = m_components.begin(); i != m_components.end(); ++i)
	{
		const EntityComponentsMap &entitiesWithComponent = i->second;
		EntityComponentsMap::const_iterator j = entitiesWithComponent.find(entity);
		if (j != entitiesWithComponent.end())
			list.push_back(j->second);
	}
}

void EntityManager::RemoveAllGlobalComponents()
{
	while (!m_globalComponents.empty())
	{
		GlobalComponentStore::iterator i = m_globalComponents.begin();

		GlobalComponent *component = i->second;
		SAFE_DELETE(component);

		m_globalComponents.erase(i);
	}
}

void EntityManager::OnLostContext()
{
	for (ComponentSystemList::iterator i = m_componentSystems.begin(); i != m_componentSystems.end(); ++i)
		(*i)->OnLostContext();
}

void EntityManager::OnNewContext()
{
	for (ComponentSystemList::iterator i = m_componentSystems.begin(); i != m_componentSystems.end(); ++i)
		(*i)->OnNewContext();
}

void EntityManager::OnRender(RenderContext *renderContext)
{
	for (ComponentSystemList::iterator i = m_componentSystems.begin(); i != m_componentSystems.end(); ++i)
		(*i)->OnRender(renderContext);
}

void EntityManager::OnResize()
{
	for (ComponentSystemList::iterator i = m_componentSystems.begin(); i != m_componentSystems.end(); ++i)
		(*i)->OnResize();
}

void EntityManager::OnUpdate(float delta)
{
	// find any inactive components and remove the associated entities before 
	// we update anything
	EntityList list;
	GetAllWith<InactiveComponent>(list);
	for (EntityList::iterator i = list.begin(); i != list.end(); ++i)
		Remove(*i);

	for (ComponentSystemList::iterator i = m_componentSystems.begin(); i != m_componentSystems.end(); ++i)
		(*i)->OnUpdate(delta);
}
