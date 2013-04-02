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
	 * @return true if this content object was created by a ContentLoader
	 */
	bool WasLoadedByContentLoader() const                  { return m_wasLoadedByContentLoader; }

	/**
	 * @return true if something else is currently referencing this content
	 */
	bool IsReferenced() const                              { return m_referenceCount > 0; }

	/**
	 * @return true if this content is currently being reference counted
	 */
	bool IsReferenceCounted() const                        { return m_isReferenceCounted; }

	/**
	 * @return the number of references currently being held for this content
	 */
	uint GetNumReferences() const                          { return m_referenceCount; }

private:
	/**
	 * Increments the reference counter by 1.
	 */
	void Reference();
	
	/**
	 * Decrements the reference counter by 1.
	 */
	void ReleaseReference();	

	bool m_wasLoadedByContentLoader;
	bool m_isReferenceCounted;
	uint m_referenceCount;
};

#endif

