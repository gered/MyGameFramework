#ifndef __FRAMEWORK_CONTENT_CONTENTLOADERMAPSTOREBASE_H_INCLUDED__
#define __FRAMEWORK_CONTENT_CONTENTLOADERMAPSTOREBASE_H_INCLUDED__

#include "../debug.h"
#include "../common.h"
#include "../log.h"
#include "content.h"
#include "contentloader.h"
#include "contentparam.h"
#include "contentcontainer.h"
#include <stl/map.h>
#include <stl/string.h>

class ContentManager;

template <class T>
class ContentLoaderMapStoreBase : public ContentLoader<T>
{
public:
	typedef stl::map<stl::string, ContentContainer<T> > ContentStore;
	typedef typename ContentStore::iterator ContentStoreItor;
	typedef typename ContentStore::const_iterator ContentStoreConstItor;
	
	ContentLoaderMapStoreBase(const stl::string &loggingTag, ContentManager *contentManager, const stl::string &defaultPath);
	virtual ~ContentLoaderMapStoreBase();

	void SetDefaultPath(const stl::string &path);
	
	T* Get(const stl::string &name, const ContentParam *params, bool preload = false);
	void Free(T *content, bool preload = false);
	void Free(const stl::string &name, const ContentParam *params, bool preload = false);
	
	void RemoveAllContent();
	
	stl::string GetNameOf(T *content) const;
	
protected:
	void Free(ContentStoreItor &itor, bool force, bool preload);
	
	virtual ContentStoreItor FindContent(const stl::string &file, const ContentParam *params);
	virtual T* LoadContent(const stl::string &file, const ContentParam *params) = 0;
	virtual void FreeContent(T *content) = 0;
	
	stl::string AddDefaultPathIfNeeded(const stl::string &filename) const;
	virtual stl::string ProcessFilename(const stl::string &filename, const ContentParam *params) const;
	
	const char* GetLoggingTag() const                      { return m_loggingTag.c_str(); }
	const stl::string& GetDefaultPath() const              { return m_defaultPath; }

	ContentStore m_content;
	
private:
	stl::string m_loggingTag;
	stl::string m_defaultPath;
};

template <class T>
ContentLoaderMapStoreBase<T>::ContentLoaderMapStoreBase(const stl::string &loggingTag, ContentManager *contentManager, const stl::string &defaultPath)
	: ContentLoader<T>(contentManager)
{
	ASSERT(loggingTag.length() > 0);
	m_loggingTag = loggingTag;
	SetDefaultPath(defaultPath);
}

template <class T>
ContentLoaderMapStoreBase<T>::~ContentLoaderMapStoreBase()
{
	ASSERT(m_content.empty());
}

template<class T>
T* ContentLoaderMapStoreBase<T>::Get(const stl::string &name, const ContentParam *params, bool preload)
{
	ContentContainer<T> content;
	
	stl::string filename = AddDefaultPathIfNeeded(name);
	filename = ProcessFilename(filename, params);
	
	ContentStoreItor itor = FindContent(filename, params);
	if (itor != m_content.end())
	{
		if (preload)
			LOG_WARN(LOGCAT_ASSETS, "%s: preload specified but \"%s\" is already loaded, not precaching.\n", 
					 GetLoggingTag(), 
					 filename.c_str()
					 );
		content = itor->second;
	}
	else
	{
		T *actualContentObject = LoadContent(filename, params);
		if (actualContentObject != NULL)
		{
			ContentLoaderBase::MarkLoadedByContentLoader(actualContentObject);
			content.content = actualContentObject;
			content.isPreLoaded = preload;
			m_content[filename] = content;
		}
	}
	
	if (!content.isPreLoaded && content.content != NULL)
		ContentLoaderBase::Reference(content.content);
	
	return content.content;
}

template<class T>
void ContentLoaderMapStoreBase<T>::Free(T *content, bool preload)
{
	ASSERT(content != NULL);
	
	for (ContentStoreItor itor = m_content.begin(); itor != m_content.end(); ++itor)
	{
		if (itor->second.content == content)
		{
			Free(itor, false, preload);
			break;
		}
	}
}

template<class T>
void ContentLoaderMapStoreBase<T>::Free(const stl::string &name, const ContentParam *params, bool preload)
{
	stl::string filename = AddDefaultPathIfNeeded(name);
	filename = ProcessFilename(filename, params);
	
	ContentStoreItor itor = FindContent(filename, params);
	if (itor != m_content.end())
		Free(itor, false, preload);
}

template<class T>
void ContentLoaderMapStoreBase<T>::Free(ContentStoreItor &itor, bool force, bool preload)
{
	ASSERT(itor->second.content->WasLoadedByContentLoader() == true);
	
	if (!itor->second.isPreLoaded)
		ContentLoaderBase::ReleaseReference(itor->second.content);
	
	// don't try to free this content if it's preloaded when this method was
	// not called to free preloaded content
	// (however, allow it if we're forcefully freeing, of course)
	if (!force && itor->second.isPreLoaded && !preload)
		return;
	
	if (itor->second.content->IsReferenced() && force)
	{
		// if this is being forced, we need to manually decrease the Content's
		// internal reference counter (or the destructor will have a fit).
		// I suppose this is probably kind of "hack-ish"...
		uint numToRelease = itor->second.content->GetNumReferences();
		for (uint i = 0; i < numToRelease; ++i)
			ContentLoaderBase::ReleaseReference(itor->second.content);
	}
	
	if (!itor->second.content->IsReferenced())
	{
		LOG_INFO(LOGCAT_ASSETS, "%s: %sfreed %s\"%s\"\n", 
				 GetLoggingTag(), 
				 (force ? "forcefully " : ""), 
				 (itor->second.isPreLoaded ? "preloaded " : ""), 
				 itor->first.c_str()
				 );
		T* content = itor->second.content;
		FreeContent(content);
		m_content.erase(itor);
	}
}

template<class T>
void ContentLoaderMapStoreBase<T>::RemoveAllContent()
{
	while (m_content.size() > 0)
	{
		ContentStoreItor itor = m_content.begin();
		LOG_WARN(LOGCAT_ASSETS, "%s: \"%s\" not explicitly freed (%d references%s).\n", 
				 GetLoggingTag(), 
				 itor->first.c_str(), 
				 itor->second.content->GetNumReferences(), 
				 (itor->second.isPreLoaded ? ", preloaded" : "")
				 );
		Free(itor, true, itor->second.isPreLoaded);
	}
	
	m_content.clear();
}

template<class T>
stl::string ContentLoaderMapStoreBase<T>::GetNameOf(T *content) const
{
	ASSERT(content != NULL);
	
	for (ContentStoreConstItor itor = m_content.begin(); itor != m_content.end(); ++itor)
	{
		if (itor->second.content == content)
			return itor->first;
	}
	
	return "";
}

template<class T>
typename ContentLoaderMapStoreBase<T>::ContentStoreItor ContentLoaderMapStoreBase<T>::FindContent(const stl::string &file, const ContentParam *params)
{
	// default implementation, we ignore the "params" parameter completely
	return m_content.find(file);
}

template<class T>
void ContentLoaderMapStoreBase<T>::SetDefaultPath(const stl::string &path)
{
	if (path.length() == 0)
	{
		m_defaultPath = "";
		return;
	}
	
	m_defaultPath = path;
	
	if (m_defaultPath[m_defaultPath.length() - 1] != '/')
		m_defaultPath += '/';
	
	LOG_INFO(LOGCAT_ASSETS, "%s: default path set to \"%s\"\n", 
			 GetLoggingTag(), 
			 m_defaultPath.c_str()
			 );
}

template<class T>
stl::string ContentLoaderMapStoreBase<T>::AddDefaultPathIfNeeded(const stl::string &imageFile) const
{
	if (imageFile[0] == '/' || m_defaultPath.length() == 0 || imageFile.substr(0, 9) == "assets://")
		return imageFile;
	else
		return m_defaultPath + imageFile;
}

template<class T>
stl::string ContentLoaderMapStoreBase<T>::ProcessFilename(const stl::string &filename, const ContentParam *params) const
{
	return filename;
}

#endif
