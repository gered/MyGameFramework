#ifndef __FRAMEWORK_CONTENT_CONTENT_H_INCLUDED__
#define __FRAMEWORK_CONTENT_CONTENT_H_INCLUDED__

#include "../common.h"

class ContentLoaderBase;

typedef const char* CONTENT_TYPE;

/**
 * Base class for content classes that are to be loaded and freed with
 * ContentLoader classes and ContentManager.
 */
class Content
{
	friend class ContentLoaderBase;
	
public:
	Content();
	virtual ~Content();
	
	/**
	 * @return TRUE if this content object was created by a ContentLoader
	 */
	BOOL WasLoadedByContentLoader() const                  { return m_wasLoadedByContentLoader; }

	/**
	 * @return TRUE if something else is currently referencing this content
	 */
	BOOL IsReferenced() const                              { return m_referenceCount > 0; }

	/**
	 * @return TRUE if this content is currently being reference counted
	 */
	BOOL IsReferenceCounted() const                        { return m_isReferenceCounted; }

	/**
	 * @return the number of references currently being held for this content
	 */
	uint32_t GetNumReferences() const                      { return m_referenceCount; }
	
	/**
	 * @return the type of content being held in this object
	 */
	virtual CONTENT_TYPE GetTypeOf() const = 0;

	/**
	 * @param <T> the type of content to check
	 * @return TRUE if this content object is of type T
	 */
	template<class T> BOOL Is() const;

	/**
	 * @param type the content type to check
	 * @return TRUE if this content object is the same as the type specified
	 */
	BOOL Is(CONTENT_TYPE type) const;

	/**
	 * @param <T> the type of content to attempt to cast to
	 * @return this content object casted to type T if this content object
	 *         is of type T, or NULL if the types don't match
	 */
	template<class T> T* As();

	/**
	 * @param <T> the type of content to attempt to cast to
	 * @return this content object casted to type T if this content object
	 *         is of type T, or NULL if the types don't match
	 */
	template<class T> const T* As() const;

private:
	/**
	 * Increments the reference counter by 1.
	 */
	void Reference();
	
	/**
	 * Decrements the reference counter by 1.
	 */
	void ReleaseReference();	

	BOOL m_wasLoadedByContentLoader;
	BOOL m_isReferenceCounted;
	uint32_t m_referenceCount;
};

template<class T>
inline BOOL Content::Is() const
{
	return (GetTypeOf() == T::GetType());
}

inline BOOL Content::Is(CONTENT_TYPE type) const
{
	return (GetTypeOf() == type);
}

template<class T>
inline T* Content::As()
{
	if (Is<T>())
		return (T*)this;
	else
		return NULL;
}

template<class T>
inline const T* Content::As() const
{
	if (Is<T>())
		return (const T*)this;
	else
		return NULL;
}

#endif

