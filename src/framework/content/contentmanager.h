#ifndef __FRAMEWORK_CONTENT_CONTENTMANAGER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_CONTENTMANAGER_H_INCLUDED__

#include "../debug.h"
#include "content.h"
#include "contentparam.h"
#include "contentloader.h"
#include <stl/map.h>
#include <stl/string.h>

class BaseGameApp;

typedef stl::map<CONTENT_TYPE, ContentLoaderBase*> ContentLoaderMap;

/**
 * Serves as the central point for accessing content provided by a number
 * of registered content loaders for each of the individual content types.
 */
class ContentManager
{
public:
	/**
	 * Creates a content manager object.
	 * @param gameApp parent game application object
	 */
	ContentManager(BaseGameApp *gameApp);

	virtual ~ContentManager();
	
	/**
	 * Initial game load after initialization success callback.
	 */
	void OnLoadGame();

	/**
	 * New OpenGL graphics context creation callback.
	 */
	void OnNewContext();

	/**
	 * Lost OpenGL graphics context callback.
	 */
	void OnLostContext();

	/**
	 * Registers a content loader that will handle loading and freeing of
	 * content objects that match a specific content type.
	 * @param loader the pre-initialized content loader object to register
	 */
	void RegisterLoader(ContentLoaderBase *loader);

	/**
	 * Retrieves the specified content either from a cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the content to retrieve
	 * @param preload true to mark the content as preloaded after loading it
	 * @param <T> the type of content to load
	 * @return the loaded content or NULL on failure
	 */
	template <class T> T* Get(const stl::string &name, bool preload = false);

	/**
	 * Retrieves the specified content either from a cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the content to retrieve
	 * @param params content-type-specified parameters used during loading or NULL
	 * @param preload true to mark the content as preloaded after loading it
	 * @param <T> the type of content to load
	 * @return the loaded content or NULL on failure
	 */
	template <class T> T* Get(const stl::string &name, const ContentParam &params, bool preload = false);
	
	/**
	 * Loads the content so that it is pre-loaded and subsequent calls to Get()
	 * for the same content are guaranteed to not require a load operation.
	 * Pre-loaded content is not reference counted and can only be freed with a
	 * call to Unload().
	 * @param name the path and filename of the content to retrieve
	 * @param <T> the type of content to load
	 * @return the loaded content or NULL on failure
	 */
	template <class T> T* Load(const stl::string &name);
	
	/**
	 * Loads the content so that it is pre-loaded and subsequent calls to Get()
	 * for the same content are guaranteed to not require a load operation.
	 * Pre-loaded content is not reference counted and can only be freed with a
	 * call to Unload().
	 * @param name the path and filename of the content to retrieve
	 * @param params content-type-specified parameters used during loading or NULL
	 * @param <T> the type of content to load
	 * @return the loaded content or NULL on failure
	 */
	template <class T> T* Load(const stl::string &name, const ContentParam &params);
	
	/**
	 * Frees the specified content if the content pointed to by the given path
	 * and filename has been loaded. Content is only actually freed when it has
	 * no more references to it.
	 * @param name the path and filename of the content to free
	 * @param preload true to allow this content to be freed even if it has
	 *                been preloaded
	 * @param <T> the type of content to be freed
	 */
	template <class T> void Free(const stl::string &name, bool preload = false);

	/**
	 * Frees the specified content if the content pointed to by the given path
	 * and filename has been loaded. Content is only actually freed when it has
	 * no more references to it.
	 * @param name the path and filename of the content to free
	 * @param params content-type-specific parameters that further describe the
	 *               exact content object to be freed, or NULL
	 * @param preload true to allow this content to be freed even if it has
	 *                been preloaded
	 * @param <T> the type of content to be freed
	 */
	template <class T> void Free(const stl::string &name, const ContentParam &params, bool preload = false);

	/**
	 * Frees the specified content object. Content is only actually freed
	 * when it has no more references to it.
	 * @param content the content object to be freed
	 * @param preload true to allow this content to be freed even if it has
	 *                been preloaded
	 * @param <T> the type of content to be freed
	 */
	template <class T> void Free(T *content, bool preload = false);
	
	/**
	 * Frees the specified pre-loaded content. This will free the content
	 * immediately, even if other code is still referencing it. Pre-loaded
	 * content is not reference counted.
	 * @param name the path and filename of the content to free
	 * @param <T> the type of content to be freed
	 */
	template <class T> void Unload(const stl::string &name);

	/**
	 * Frees the specified pre-loaded content. This will free the content
	 * immediately, even if other code is still referencing it. Pre-loaded
	 * content is not reference counted.
	 * @param name the path and filename of the content to free
	 * @param params content-type-specific parameters that further describe the
	 *               exact content object to be freed, or NULL
	 * @param <T> the type of content to be freed
	 */
	template <class T> void Unload(const stl::string &name, const ContentParam &params);

	/**
	 * Frees the specified pre-loaded content. This will free the content
	 * immediately, even if other code is still referencing it. Pre-loaded
	 * content is not reference counted.
	 * @param content the content object to be freed
	 * @param <T> the type of content to be freed
	 */
	template <class T> void Unload(T* content);
	
	/**
	 * Returns the path and filename that the given content object was
	 * loaded from.
	 * @param content the content object to get the path and filename for
	 * @param <T> the type of content to get the path and filename for
	 * @return the path and filename that the content object was loaded from
	 */
	template <class T> stl::string GetNameOf(T *content) const;

	/** 
	 * Returns the content loader object responsible for managing content of
	 * the specified type.
	 * @param <T> the content type who's content loader object is to be returned
	 * @return the content loader object or NULL if none exists
	 */ 
	template <class T> ContentLoader<T>* GetLoaderForType() const;

	/**
	 * @return the parent game app object
	 */
	BaseGameApp* GetGameApp() const                        { return m_gameApp; }
	
private:
	template <class T> ContentLoader<T>* GetLoader() const;
	ContentLoaderBase* GetLoaderFor(CONTENT_TYPE type) const;

	BaseGameApp *m_gameApp;
	ContentLoaderMap m_loaders;
};

template <class T>
T* ContentManager::Get(const stl::string &name, bool preload)
{
	ContentLoader<T> *loader = GetLoader<T>();
	T* content = loader->Get(name, NULL, preload);
	return content;
}

template <class T>
T* ContentManager::Get(const stl::string &name, const ContentParam &params, bool preload)
{
	ContentLoader<T> *loader = GetLoader<T>();
	T* content = loader->Get(name, &params, preload);
	return content;
}

template<class T>
T* ContentManager::Load(const stl::string &name)
{
	return Get<T>(name, true);
}

template<class T>
T* ContentManager::Load(const stl::string &name, const ContentParam &params)
{
	return Get<T>(name, params, true);
}

template <class T>
void ContentManager::Free(const stl::string &name, bool preload)
{
	ContentLoader<T> *loader = GetLoader<T>();
	loader->Free(name, NULL, preload);
}

template <class T>
void ContentManager::Free(const stl::string &name, const ContentParam &params, bool preload)
{
	ContentLoader<T> *loader = GetLoader<T>();
	loader->Free(name, &params, preload);
}

template <class T>
void ContentManager::Free(T *content, bool preload)
{
	ContentLoader<T> *loader = GetLoader<T>();
	loader->Free(content, preload);
}

template <class T>
void ContentManager::Unload(const stl::string &name)
{
	Free<T>(name, true);
}

template <class T>
void ContentManager::Unload(const stl::string &name, const ContentParam &params)
{
	Free<T>(name, params, true);
}

template <class T>
void ContentManager::Unload(T *content)
{
	Free<T>(content, true);
}

template <class T>
stl::string ContentManager::GetNameOf(T *content) const
{
	ContentLoader<T> *loader = GetLoader<T>();
	return loader->GetNameOf(content);
}

template <class T>
ContentLoader<T>* ContentManager::GetLoaderForType() const
{
	return GetLoader<T>();
}

template <class T>
inline ContentLoader<T>* ContentManager::GetLoader() const
{
	ContentLoaderBase *loaderBase = GetLoaderFor(T::GetType());
	ASSERT(loaderBase != NULL);
	return (ContentLoader<T>*)loaderBase;
}

#endif
