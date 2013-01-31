#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "contentmanager.h"
#include "../basegameapp.h"
#include <stl/map.h>
#include <stl/string.h>

ContentManager::ContentManager(BaseGameApp *gameApp)
{
	STACK_TRACE;
	m_gameApp = gameApp;
}

ContentManager::~ContentManager()
{
	STACK_TRACE;
	for (ContentLoaderMap::iterator itor = m_loaders.begin(); itor != m_loaders.end(); ++itor)
	{
		ContentLoaderBase *loader = itor->second;
		loader->RemoveAllContent();
		SAFE_DELETE(loader);
	}
}

void ContentManager::OnLoadGame()
{
	STACK_TRACE;
	for (ContentLoaderMap::iterator itor = m_loaders.begin(); itor != m_loaders.end(); ++itor)
	{
		ContentLoaderBase *loader = itor->second;
		loader->OnLoadGame();
	}
}

void ContentManager::OnNewContext()
{
	STACK_TRACE;
	for (ContentLoaderMap::iterator itor = m_loaders.begin(); itor != m_loaders.end(); ++itor)
	{
		ContentLoaderBase *loader = itor->second;
		loader->OnNewContext();
	}
}

void ContentManager::OnLostContext()
{
	STACK_TRACE;
	for (ContentLoaderMap::iterator itor = m_loaders.begin(); itor != m_loaders.end(); ++itor)
	{
		ContentLoaderBase *loader = itor->second;
		loader->OnLostContext();
	}
}

void ContentManager::RegisterLoader(ContentLoaderBase *loader)
{
	STACK_TRACE;
	ASSERT(loader != NULL);

	ContentLoaderBase *existingLoader = GetLoaderFor(loader->GetTypeOf());
	ASSERT(existingLoader == NULL);

	m_loaders[loader->GetTypeOf()] = loader;
}

ContentLoaderBase* ContentManager::GetLoaderFor(CONTENT_TYPE type) const
{
	STACK_TRACE;
	ContentLoaderMap::const_iterator itor = m_loaders.find(type);
	if (itor == m_loaders.end())
		return NULL;
	else
		return itor->second;
}


