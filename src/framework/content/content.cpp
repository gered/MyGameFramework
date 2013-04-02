#include "../debug.h"

#include "content.h"

Content::Content()
{
	m_referenceCount = 0;
	m_isReferenceCounted = FALSE;
	m_wasLoadedByContentLoader = FALSE;
}

Content::~Content()
{
	ASSERT(m_isReferenceCounted == FALSE || m_referenceCount == 0);
}

void Content::Reference()
{
	m_isReferenceCounted = TRUE;
	++m_referenceCount;
}

void Content::ReleaseReference()
{
	ASSERT(m_isReferenceCounted == TRUE);
	ASSERT(IsReferenced() == TRUE);
	if (m_referenceCount > 0)
		--m_referenceCount;
}

