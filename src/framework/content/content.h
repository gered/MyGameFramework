#ifndef __FRAMEWORK_CONTENT_CONTENT_H_INCLUDED__
#define __FRAMEWORK_CONTENT_CONTENT_H_INCLUDED__

#include "../common.h"
#include "../util/typesystem.h"

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
	TYPE_BASE(CONTENT_TYPE);
	
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

#endif

