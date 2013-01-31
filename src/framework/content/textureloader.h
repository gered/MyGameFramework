#ifndef __FRAMEWORK_CONTENT_TEXTURELOADER_H_INCLUDED__
#define __FRAMEWORK_CONTENT_TEXTURELOADER_H_INCLUDED__

#include "../common.h"
#include "contentparam.h"
#include "contentloadermapstorebase.h"
#include "../graphics/texture.h"
#include <stl/string.h>

class ContentManager;

/**
 * Content loader for textures.
 */
class TextureLoader : public ContentLoaderMapStoreBase<Texture>
{
public:
	/**
	 * Creates a texture loader.
	 * @param contentManager the parent content manager
	 */
	TextureLoader(ContentManager *contentManager);

	/**
	 * Creates a texture loader.
	 * @param contentManager the parent content manager
	 * @param defaultPath the default path to use if, when loading, paths aren't
	 *                    provided with texture filenames
	 */
	TextureLoader(ContentManager *contentManager, const stl::string &defaultPath);

	virtual ~TextureLoader();

	/**
	 * New OpenGL graphics context creation callback.
	 */
	void OnNewContext();

	/**
	 * Lost OpenGL graphics context callback.
	 */
	void OnLostContext();

protected:
	Texture* LoadContent(const stl::string &file, const ContentParam *params);
	void FreeContent(Texture *content);
	
private:
	Texture* Load(const stl::string &file, Texture *existingTexture = NULL);
};

#endif
