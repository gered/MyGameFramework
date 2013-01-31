#ifndef __ENTITIES_COMPONENT_H_INCLUDED__
#define __ENTITIES_COMPONENT_H_INCLUDED__

#include "../framework/common.h"

typedef const char* COMPONENT_TYPE;

class Component
{
public:
	virtual ~Component();
	virtual void Reset();

	virtual COMPONENT_TYPE GetTypeOf() const = 0;

	template<class T> BOOL Is() const;
	BOOL Is(COMPONENT_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;
};

inline Component::~Component()
{
}

inline void Component::Reset()
{
}

template<class T>
inline BOOL Component::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL Component::Is(COMPONENT_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* Component::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* Component::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif

