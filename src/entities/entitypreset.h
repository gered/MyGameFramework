#ifndef __ENTITIES_ENTITYPRESET_H_INCLUDED__
#define __ENTITIES_ENTITYPRESET_H_INCLUDED__

#include "../framework/common.h"
#include "entitypresetargs.h"

typedef const char* ENTITYPRESET_TYPE;

class Entity;
class EntityManager;

class EntityPreset
{
public:
	EntityPreset(EntityManager *entityManager);
	virtual ~EntityPreset();

	virtual ENTITYPRESET_TYPE GetTypeOf() const = 0;
	
	template<class T> BOOL Is() const;
	BOOL Is(ENTITYPRESET_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;

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

template<class T>
inline BOOL EntityPreset::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL EntityPreset::Is(ENTITYPRESET_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* EntityPreset::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* EntityPreset::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif
