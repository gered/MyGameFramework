#ifndef __FRAMEWORK_GRAPHICS_SPRITESHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SPRITESHADER_H_INCLUDED__

#include "../common.h"
#include "standardshader.h"
#include <stl/string.h>

/**
 * Base class for shaders which will be used to render sprites primarily.
 */
class SpriteShader : public StandardShader
{
public:
	virtual ~SpriteShader();

	/**
	 * Sets whether the texture that will be used for rendering consists
	 * of only an alpha channel (no RGB information). This will affect
	 * the way which color modulation is done using vertex colors.
	 * @param hasAlphaOnly whether the texture has only an alpha component
	 */
	void SetTextureHasAlphaOnly(BOOL hasAlphaOnly);

protected:
	/**
	 * Initializes standard uniform information to defaults.
	 */
	SpriteShader();

	/**
	 * @return the name of the "texture has only an alpha component" uniform
	 */
	const stl::string& GetTextureHasAlphaOnlyUniform() const       { return m_textureHasAlphaOnlyUniform; }

	/**
	 * Sets the name of the "texture has only an alpha component" uniform.
.	 * @param name the name of the uniform
	 */
	void SetTextureHasAlphaOnlyUniform(const stl::string &name)    { m_textureHasAlphaOnlyUniform = name; }

private:
	stl::string m_textureHasAlphaOnlyUniform;
};

#endif
