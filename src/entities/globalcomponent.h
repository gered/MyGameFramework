#ifndef __ENTITIES_GLOBALCOMPONENT_H_INCLUDED__
#define __ENTITIES_GLOBALCOMPONENT_H_INCLUDED__

#include "../framework/common.h"

typedef const char* GLOBAL_COMPONENT_TYPE;

class GlobalComponent
{
public:
	virtual ~GlobalComponent();
	virtual void Reset();

	virtual GLOBAL_COMPONENT_TYPE GetTypeOf() const = 0;

	template<class T> BOOL Is() const;
	BOOL Is(GLOBAL_COMPONENT_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;
};

inline GlobalComponent::~GlobalComponent()
{
}

inline void GlobalComponent::Reset()
{
}

template<class T>
inline BOOL GlobalComponent::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL GlobalComponent::Is(GLOBAL_COMPONENT_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* GlobalComponent::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* GlobalComponent::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif

