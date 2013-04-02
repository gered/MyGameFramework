#include "../framework/common.h"

#include "entity.h"
#include "entitymanager.h"

Entity::Entity(EntityManager *entityManager)
{
	m_entityManager = entityManager;
}

Entity::~Entity()
{
}

