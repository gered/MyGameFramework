#ifndef __FRAMEWORK_GRAPHICS_STANDARDSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_STANDARDSHADER_H_INCLUDED__

#include "shader.h"
#include <stl/string.h>

struct Matrix4x4;

/**
 * Base class for shaders which always provided uniforms for certain
 * things.
 */
class StandardShader : public Shader
{
public:
	virtual ~StandardShader();

	/**
	 * Sets the modelview matrix uniform.
	 * @param matrix the matrix to set
	 */
	void SetModelViewMatrix(const Matrix4x4 &matrix);

	/**
	 * Sets the projection matrix uniform.
	 * @param matrix the matrix to set
	 */
	void SetProjectionMatrix(const Matrix4x4 &matrix);

	/**
	 * New OpenGL graphics context creation callback.
	 */
	virtual void OnNewContext();

	/**
	 * Lost OpenGL graphics context callback.
	 */
	virtual void OnLostContext();

protected:
	/**
	 * Initializes standard uniform information to defaults.
	 */
	StandardShader();

	/**
	 * Loads, compiles and links shader sources from strings that are
	 * defined via constants or statically in the application C++ code.
	 * @param vertexShaderSource vertex shader source
	 * @param fragmentShaderSource fragment shader source
	 * @return TRUE if compilation and linking succeeded and the shader
	 *              is now ready for use
	 */
	BOOL LoadCompileAndLinkInlineSources(const char *inlineVertexShaderSource, const char *inlineFragmentShaderSource);

	/**
	 * @return the name of the modelview matrix uniform
	 */
	const stl::string& GetModelViewMatrixUniform() const           { return m_modelViewMatrixUniform; }

	/**
	 * @return the name of the projection matrix uniform
	 */
	const stl::string& GetProjectionMatrixUniform() const          { return m_projectionMatrixUniform; }
	
	/**
	 * Sets the name of the modelview matrix uniform.
.	 * @param name the name of the uniform
	 */
	void SetModelViewMatrixUniform(const stl::string &name)        { m_modelViewMatrixUniform = name; }

	/**
	 * Sets the name of the projection matrix uniform.
	 * @param name the name of the uniform
	 */
	void SetProjectionMatrixUniform(const stl::string &name)       { m_projectionMatrixUniform = name; }

private:
	stl::string m_modelViewMatrixUniform;
	stl::string m_projectionMatrixUniform;

	const char *m_inlineVertexShaderSource;
	const char *m_inlineFragmentShaderSource;
};

#endif
