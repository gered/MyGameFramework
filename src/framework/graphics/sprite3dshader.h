#ifndef __FRAMEWORK_GRAPHICS_SPRITE3DSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SPRITE3DSHADER_H_INCLUDED__

#include "spriteshader.h"

class GraphicsDevice;

/**
 * Shader for rendering 3D billboard sprites with vertex color modulation. 
 * Includes special support for color modulation using RGB, RGBA, and 
 * alpha-only texture formats. Also performs alpha testing (fragments with
 * alpha == 0.0f are discarded).
 */
class Sprite3DShader : public SpriteShader
{
public:
	Sprite3DShader();
	virtual ~Sprite3DShader();
	
	BOOL Initialize(GraphicsDevice *graphicsDevice);

private:
	static const char *m_vertexShaderSource;
	static const char *m_fragmentShaderSource;
};

#endif
