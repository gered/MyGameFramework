#ifndef __ENTITIES_ENTITYPRESETARGS_H_INCLUDED__
#define __ENTITIES_ENTITYPRESETARGS_H_INCLUDED__

#include "../framework/common.h"

typedef const char* ENTITYPRESETARGS_TYPE;

struct EntityPresetArgs
{
	virtual ENTITYPRESETARGS_TYPE GetTypeOf() const = 0;
	
	template<class T> BOOL Is() const;
	BOOL Is(ENTITYPRESETARGS_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;
};

template<class T>
inline BOOL EntityPresetArgs::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL EntityPresetArgs::Is(ENTITYPRESETARGS_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* EntityPresetArgs::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* EntityPresetArgs::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif
