#ifndef __FRAMEWORK_UTIL_TYPESYSTEM_H_INCLUDED__
#define __FRAMEWORK_UTIL_TYPESYSTEM_H_INCLUDED__

#include "../common.h"

typedef const char* TYPE_IDENT;

#define TYPE_DEFINE(type, value)              \
	static #type GetType()                    \
	{                                         \
		static #type typeName = #value;       \
		return typeName;                      \
	}                                         \
	#type GetTypeOf() const                   \
	{                                         \
		return GetType();                     \
	}

#define TYPE_BASE(type)                                           \
	virtual type GetTypeOf() const = 0;                           \
	template<class T> BOOL Is() const                             \
	{                                                             \
		return (GetTypeOf() == T::GetType());                     \
	}                                                             \
	BOOL Is(type otherType) const                                 \
	{                                                             \
		return (GetTypeOf() == otherType);                        \
	}                                                             \
	template<class T> T* As()                                     \
	{                                                             \
		if (Is<T>())                                              \
			return (T*)this;                                      \
		else                                                      \
			return NULL;                                          \
	}                                                             \
	template<class T> const T* As() const                         \
	{                                                             \
		if (Is<T>())                                              \
			return (const T*)this;                                \
		else                                                      \
			return NULL;                                          \
	}

#define TYPE_DEFINE_DEFAULT(value)     TYPE_DEFINE(TYPE_IDENT, value)
#define TYPE_BASE_DEFAULT              TYPE_BASE(TYPE_IDENT)

#endif
