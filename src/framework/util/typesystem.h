#ifndef __FRAMEWORK_UTIL_TYPESYSTEM_H_INCLUDED__
#define __FRAMEWORK_UTIL_TYPESYSTEM_H_INCLUDED__

#include "../common.h"

// default generic type system identifier type
typedef const char* TYPE_IDENT;

// The macros below can be used to quickly add methods to classes which allow
// a "poor-mans" run-time type system to be used. This type system allows for:
//
//   - retrieval of a type-identifier at run-time via either:
//         object.GetTypeOf()        (instance method)      or
//         ObjectType::GetType()     (static method)
//
//   - comparison of two types using either template methods (static) or 
//     variables containing type identifiers
//         BOOL result = object.Is<ObjectType>()          or
//         BOOL result = object.Is(objectTypeId)
//
//   - "safe" casting - if the type doesn't match, NULL is returned
//         ObjectType* type = object.As<ObjectType>()


/**
 * Adds methods to a base type class that implement type identifier retrieval
 * (which will be overridden by subclasses) and type comparison and safe
 * casting between types.
 * @param type the compiler type used for the type identifier
 */
#define TYPE_BASE(type)                                                        \
	virtual type GetTypeOf() const = 0;                                        \
	template<class T> BOOL Is() const                                          \
	{                                                                          \
		return (GetTypeOf() == T::GetType());                                  \
	}                                                                          \
	BOOL Is(type otherType) const                                              \
	{                                                                          \
		return (GetTypeOf() == otherType);                                     \
	}                                                                          \
	template<class T> T* As()                                                  \
	{                                                                          \
		if (Is<T>())                                                           \
			return (T*)this;                                                   \
		else                                                                   \
			return NULL;                                                       \
	}                                                                          \
	template<class T> const T* As() const                                      \
	{                                                                          \
		if (Is<T>())                                                           \
			return (const T*)this;                                             \
		else                                                                   \
			return NULL;                                                       \
	}

/**
 * Adds methods used by subclasses of a type that defines methods for
 * type identifier retrieval and comparisons
 * @param type the compiler type used for the type identifier for this class
 * @param value the value to assign to this class's type identifier
 */
#define TYPE_DEFINE(type, value)                                               \
	static type GetType()                                                      \
	{                                                                          \
		static type typeName = value;                                          \
		return typeName;                                                       \
	}                                                                          \
	type GetTypeOf() const                                                     \
	{                                                                          \
		return GetType();                                                      \
	}

// these macros work the same as the above two, but they will use
// the TYPE_IDENT typedef defined above as the type identifier compiler type

#define TYPE_BASE_DEFAULT              TYPE_BASE(TYPE_IDENT)
#define TYPE_DEFINE_DEFAULT(value)     TYPE_DEFINE(TYPE_IDENT, value)

#endif
