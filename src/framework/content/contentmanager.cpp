#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "contentmanager.h"
#include "../basegameapp.h"
#include <stl/map.h>
#include <stl/string.h>

ContentManager::ContentManager(BaseGameApp *gameApp)
{
	m_gameApp = gameApp;
}

ContentManager::~ContentManager()
{
	for (ContentLoaderMap::iterator itor = m_loaders.begin(); itor != m_loaders.end(); ++itor)
	{
		ContentLoaderBase *loader = itor->second;
		loader->RemoveAllContent();
		SAFE_DELETE(loader);
	}
}

void ContentManager::OnLoadGame()
{
	for (ContentLoaderMap::iterator itor = m_loaders.begin(); itor != m_loaders.end(); ++itor)
	{
		ContentLoaderBase *loader = itor->second;
		loader->OnLoadGame();
	}
}

void ContentManager::OnNewContext()
{
	for (ContentLoaderMap::iterator itor = m_loaders.begin(); itor != m_loaders.end(); ++itor)
	{
		ContentLoaderBase *loader = itor->second;
		loader->OnNewContext();
	}
}

void ContentManager::OnLostContext()
{
	for (ContentLoaderMap::iterator itor = m_loaders.begin(); itor != m_loaders.end(); ++itor)
	{
		ContentLoaderBase *loader = itor->second;
		loader->OnLostContext();
	}
}

void ContentManager::RegisterLoader(ContentLoaderBase *loader)
{
	ASSERT(loader != NULL);

	ContentLoaderBase *existingLoader = GetLoaderFor(loader->GetTypeOf());
	ASSERT(existingLoader == NULL);

	m_loaders[loader->GetTypeOf()] = loader;
}

ContentLoaderBase* ContentManager::GetLoaderFor(CONTENT_TYPE type) const
{
	ContentLoaderMap::const_iterator itor = m_loaders.find(type);
	if (itor == m_loaders.end())
		return NULL;
	else
		return itor->second;
}


