#include "../debug.h"
#include "../common.h"

#include "contentloader.h"
#include "content.h"

void ContentLoaderBase::MarkLoadedByContentLoader(Content *content)
{
	STACK_TRACE;
	ASSERT(content != NULL);
	content->m_wasLoadedByContentLoader = TRUE;
}

void ContentLoaderBase::Reference(Content *content)
{
	STACK_TRACE;
	ASSERT(content != NULL);
	content->Reference();
}

void ContentLoaderBase::ReleaseReference(Content *content)
{
	STACK_TRACE;
	ASSERT(content != NULL);
	content->ReleaseReference();
}
