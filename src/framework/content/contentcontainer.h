#ifndef __FRAMEWORK_CONTENT_CONTENTCONTAINER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_CONTENTCONTAINER_H_INCLUDED__

#include "../common.h"

template <class T>
struct ContentContainer
{
	T *content;
	BOOL isPreLoaded;
	
	ContentContainer();
};

template<class T>
inline ContentContainer<T>::ContentContainer()
{
	content = NULL;
	isPreLoaded = FALSE;
}

#endif
