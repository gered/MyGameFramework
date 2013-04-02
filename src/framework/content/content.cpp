#include "../debug.h"

#include "content.h"

Content::Content()
{
	m_referenceCount = 0;
	m_isReferenceCounted = false;
	m_wasLoadedByContentLoader = false;
}

Content::~Content()
{
	ASSERT(m_isReferenceCounted == false || m_referenceCount == 0);
}

void Content::Reference()
{
	m_isReferenceCounted = true;
	++m_referenceCount;
}

void Content::ReleaseReference()
{
	ASSERT(m_isReferenceCounted == true);
	ASSERT(IsReferenced() == true);
	if (m_referenceCount > 0)
		--m_referenceCount;
}

