#include "../framework/debug.h"
#include "../framework/common.h"

#include "entity.h"
#include "entitymanager.h"

Entity::Entity(EntityManager *entityManager)
{
	STACK_TRACE;
	m_entityManager = entityManager;
}

Entity::~Entity()
{
	STACK_TRACE;
}

