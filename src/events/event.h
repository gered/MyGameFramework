#ifndef __EVENTS_EVENT_H_INCLUDED__
#define __EVENTS_EVENT_H_INCLUDED__

#include "../framework/common.h"

typedef const char* EVENT_TYPE;

const EVENT_TYPE EVENT_TYPE_WILDCARD = "EventWildcard";

struct Event
{
	virtual ~Event();
	virtual EVENT_TYPE GetTypeOf() const = 0;

	template<class T> BOOL Is() const;
	BOOL Is(EVENT_TYPE type) const;
	template<class T> T* As();
	template<class T> const T* As() const;
};

inline Event::~Event()
{
}

template<class T>
inline BOOL Event::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL Event::Is(EVENT_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* Event::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* Event::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif
