#ifndef __FRAMEWORK_CONTENT_CONTENTLOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_CONTENTLOADER_H_INCLUDED__

#include "../common.h"
#include "content.h"
#include "contentparam.h"
#include <stl/string.h>

class ContentManager;

/**
 * "Dummy" base class for ContentLoader classes. The main point of this
 * class existing is so that I can _kind of_ do this:
 * 
 *    std::map<CONTENT_TYPE, ContentLoader<class T>*> loaders;
 * 
 * which cannot be done as-is in C++. This "dummy" base class allows me
 * to almost do the above by doing it this way instead:
 *
 *    std::map<CONTENT_TYPE, ContentLoaderBase*> loaders;
 *
 * which I like a lot better then doing it like this:
 *
 *    std::map<CONTENT_TYPE, void*> loaders;
 */
class ContentLoaderBase
{
public:
	ContentLoaderBase()                                    {}
	virtual ~ContentLoaderBase()                           {}

	/**
	 * Initial game load after initialization success callback.
	 */
	virtual void OnLoadGame()                              {}

	/**
	 * New OpenGL graphics context creation callback.
	 */
	virtual void OnNewContext()                            {}

	/**
	 * Lost OpenGL graphics context callback.
	 */
	virtual void OnLostContext()                           {}

	/**
	 * Forcefully removes all loaded content.
	 */
	virtual void RemoveAllContent() = 0;
	
	/**
	 * @return the type of content that this loader object is for
	 */
	virtual CONTENT_TYPE GetTypeOf() const = 0;
	
protected:
	void MarkLoadedByContentLoader(Content *content);
	void Reference(Content *content);
	void ReleaseReference(Content *content);
};

/**
 * Strongly typed base class for ContentLoader classes.
 * @param <T> the type of content this class is for
 */
template <class T>
class ContentLoader : public ContentLoaderBase
{
public:
	/**
	 * Creates a content loader for the specified ContentManager
	 * @param contentManager the parent ContentManager this loader will be used with
	 */
	ContentLoader(ContentManager *contentManager);

	virtual ~ContentLoader();

	/**
	 * Retrieves the specified content either from a cache or from the
	 * platform's backing storage.
	 * @param name the path and filename of the content to retrieve
	 * @param params content-type-specified parameters used during loading or NULL
	 * @param preload true to mark the content as preloaded after loading it
	 * @param <T> the type of content to load
	 * @return the loaded content or NULL on failure
	 */
	virtual T* Get(const stl::string &name, const ContentParam *params, bool preload = false) = 0;

	/**
	 * Frees the specified content object. Content is only actually freed
	 * when it has no more references to it.
	 * @param content the content object to be freed
	 * @param preload true to allow this content to be freed even if it has
	 *                been preloaded
	 * @param <T> the type of content to be freed
	 */
	virtual void Free(T *content, bool preload = false) = 0;

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
	virtual void Free(const stl::string &name, const ContentParam *params, bool preload = false) = 0;
	
	/**
	 * Returns the path and filename that the given content object was
	 * loaded from.
	 * @param content the content object to get the path and filename for
	 * @param <T> the type of content to get the path and filename for
	 * @return the path and filename that the content object was loaded from
	 */
	virtual stl::string GetNameOf(T *content) const = 0;

	/**
	 * @return the type of content that this loader object is for
	 */
	CONTENT_TYPE GetTypeOf() const                           { return T::GetType(); }

protected:
	ContentManager* GetContentManager() const              { return m_contentManager; }

private:
	ContentManager *m_contentManager;
};

template <class T>
inline ContentLoader<T>::ContentLoader(ContentManager *contentManager)
{
	m_contentManager = contentManager;
}

template <class T>
inline ContentLoader<T>::~ContentLoader()
{
}

#endif

