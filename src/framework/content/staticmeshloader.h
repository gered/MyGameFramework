#ifndef __FRAMEWORK_CONTENT_STATICMESHLOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_STATICMESHLOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloadermapstorebase.h"
#include "../assets/static/staticmesh.h"
#include <stl/string.h>

class ContentManager;

/**
 * Content loader for static meshes.
 */
class StaticMeshLoader : public ContentLoaderMapStoreBase<StaticMesh>
{
public:
	/**
	 * Creates a static mesh loader.
	 * @param contentManager the parent content manager
	 */
	StaticMeshLoader(ContentManager *contentManager);

	/**
	 * Creates a static mesh loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with mesh filenames
	 */
	StaticMeshLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~StaticMeshLoader();

protected:
	StaticMesh* LoadContent(const stl::string &file, const ContentParam *params);
	void FreeContent(StaticMesh *content);
};

#endif
