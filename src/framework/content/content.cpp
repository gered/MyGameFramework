#include "../debug.h"

#include "content.h"

Content::Content()
{
	STACK_TRACE;
	m_referenceCount = 0;
	m_isReferenceCounted = FALSE;
	m_wasLoadedByContentLoader = FALSE;
}

Content::~Content()
{
	STACK_TRACE;
	ASSERT(m_isReferenceCounted == FALSE || m_referenceCount == 0);
}

void Content::Reference()
{
	STACK_TRACE;
	m_isReferenceCounted = TRUE;
	++m_referenceCount;
}

void Content::ReleaseReference()
{
	STACK_TRACE;
	ASSERT(m_isReferenceCounted == TRUE);
	ASSERT(IsReferenced() == TRUE);
	if (m_referenceCount > 0)
		--m_referenceCount;
}

