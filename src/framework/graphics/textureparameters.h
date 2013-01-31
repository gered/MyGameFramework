#ifndef __FRAMEWORK_GRAPHICS_TEXTUREPARAMETERS_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_TEXTUREPARAMETERS_H_INCLUDED__

#include "../common.h"

enum MINIFICATION_FILTER
{
	MIN_NEAREST,
	MIN_LINEAR,
	MIN_NEAREST_MIPMAP_NEAREST,
	MIN_LINEAR_MIPMAP_NEAREST,
	MIN_NEAREST_MIPMAP_LINEAR,
	MIN_LINEAR_MIPMAP_LINEAR
};

enum MAGNIFICATION_FILTER
{
	MAG_NEAREST,
	MAG_LINEAR
};

enum WRAP_MODE
{
	CLAMP_TO_EDGE,
	REPEAT
};

/**
 * Wraps texture parameter for OpenGL. Instances of this class can be initialized
 * and then used later on during execution to quickly apply a set of texture
 * parameters.
 */
class TextureParameters
{
public:
	/**
	 * Creates a new texture parameter object with default settings matching OpenGL's
	 * initial settings.
	 */
	TextureParameters();

	/**
	 * Creates a new texture parameter object with default settings matching OpenGL's
	 * initial settings except for those specified.
	 * @param minFilter texture minifying function
	 * @param magFilter texture magnification function
	 */
	TextureParameters(MINIFICATION_FILTER minFilter, MAGNIFICATION_FILTER magFilter);

	virtual ~TextureParameters();

	/**
	 * Applies texture parameter to the current OpenGL context.
	 */
	void Apply() const;

	/**
	 * @return the texture minifying function
	 */
	MINIFICATION_FILTER GetMinFilter() const               { return m_minFilter; }

	/**
	 * @return the texture magnification function
	 */
	MAGNIFICATION_FILTER GetMagFilter() const              { return m_magFilter; }

	/**
	 * @return texture wrapping mode for S (or U) texture coordinates
	 */
	WRAP_MODE GetWrapS() const                             { return m_wrapS; }

	/**
	 * @return texture wrapping mode for T (or V) texture coordinates
	 */
	WRAP_MODE GetWrapT() const                             { return m_wrapT; }

	/**
	 * Sets the texture minifying function.
	 */
	void SetMinFilter(MINIFICATION_FILTER filter)          { m_minFilter = filter; }

	/**
	 * Sets the texture magnification function.
	 */
	void SetMagFilter(MAGNIFICATION_FILTER filter)         { m_magFilter = filter; }

	/**
	 * Sets the texture wrapping mode for S (or U) texture coordinates.
	 */
	void SetWrapS(WRAP_MODE mode)                          { m_wrapS = mode; }

	/**
	 * Sets the texture wrapping mode for T (or V) texture coordinates.
	 */
	void SetWrapT(WRAP_MODE mode)                          { m_wrapT = mode; }

private:
	void Initialize();
	int FindMinificationFilterValue(MINIFICATION_FILTER filter) const;
	int FindMagnificationFilterValue(MAGNIFICATION_FILTER filter) const;
	int FindWrapModeValue(WRAP_MODE mode) const;

	MINIFICATION_FILTER m_minFilter;
	MAGNIFICATION_FILTER m_magFilter;
	WRAP_MODE m_wrapS;
	WRAP_MODE m_wrapT;
};

#define TEXPARAM_DEFAULT TextureParameters()
#define TEXPARAM_PIXELATED TextureParameters(MIN_NEAREST, MAG_NEAREST)

#endif
