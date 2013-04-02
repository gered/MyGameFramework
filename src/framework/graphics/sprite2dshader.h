#ifndef __FRAMEWORK_GRAPHICS_SPRITE2DSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SPRITE2DSHADER_H_INCLUDED__

#include "spriteshader.h"

class GraphicsDevice;

/**
 * Shader for rendering 2D sprites with vertex color modulation. Includes
 * special support for color modulation using RGB, RGBA, and alpha-only
 * texture formats.
 */
class Sprite2DShader : public SpriteShader
{
public:
	Sprite2DShader();
	virtual ~Sprite2DShader();
	
	bool Initialize(GraphicsDevice *graphicsDevice);

private:
	static const char *m_vertexShaderSource;
	static const char *m_fragmentShaderSource;
};

#endif
