#ifndef __FRAMEWORK_CONTENT_KEYFRAMEMESHLOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_KEYFRAMEMESHLOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloadermapstorebase.h"
#include "../assets/animation/keyframemesh.h"
#include <stl/string.h>

class ContentManager;

/**
 * Content loader for keyframe meshes.
 */
class KeyframeMeshLoader : public ContentLoaderMapStoreBase<KeyframeMesh>
{
public:
	/**
	 * Creates a keyframe mesh loader.
	 * @param contentManager the parent content manager
	 */
	KeyframeMeshLoader(ContentManager *contentManager);

	/**
	 * Creates a keyframe mesh loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with mesh filenames
	 */
	KeyframeMeshLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~KeyframeMeshLoader();

protected:
	KeyframeMesh* LoadContent(const stl::string &file, const ContentParam *params);
	void FreeContent(KeyframeMesh *content);
};

#endif
