#ifndef __ENTITIES_ENTITYPRESET_H_INCLUDED__
#define __ENTITIES_ENTITYPRESET_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/util/typesystem.h"
#include "entitypresetargs.h"

typedef const char* ENTITYPRESET_TYPE;

class Entity;
class EntityManager;

class EntityPreset
{
public:
	TYPE_BASE(ENTITYPRESET_TYPE);
	
	EntityPreset(EntityManager *entityManager);
	virtual ~EntityPreset();

	virtual Entity* Create(EntityPresetArgs *args = NULL) = 0;

protected:
	EntityManager* GetEntityManager() const                { return m_entityManager; }

private:
	EntityManager *m_entityManager;
};

inline EntityPreset::EntityPreset(EntityManager *entityManager)
{
	m_entityManager = entityManager;
}

inline EntityPreset::~EntityPreset()
{
}

#endif
