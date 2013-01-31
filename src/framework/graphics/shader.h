#ifndef __FRAMEWORK_GRAPHICS_SHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_SHADER_H_INCLUDED__

#include "../common.h"
#include "graphicscontextresource.h"
#include "shaderstructs.h"
#include "vertexattribs.h"

class Text;
struct Color;
struct Point2;
struct Point3;
struct Vector2;
struct Vector3;
struct Vector4;
struct Matrix3x3;
struct Matrix4x4;
struct Quaternion;

/**
 * Represents a GLSL vertex and fragment shader compiled into a program that 
 * can be used for rendering.
 */
class Shader : public GraphicsContextResource
{
	friend class GraphicsDevice;

public:
	/**
	 * Creates a new shader object using the given vertex and fragment shader
	 * sources.
	 * @param vertexShaderSource GLSL source for a vertex shader
	 * @param fragmentShaderSource GLSL source for a vertex shader
	 */
	Shader(const char *vertexShaderSource, const char *fragmentShaderSource);

	/**
	 * Creates a new shader object using the given vertex and fragment shader
	 * sources.
	 * @param vertexShaderSource GLSL source for a vertex shader
	 * @param fragmentShaderSource GLSL source for a vertex shader
	 */
	Shader(const Text *vertexShaderSource, const Text *fragmentShaderSource);

	virtual ~Shader();

	/**
	 * @return TRUE if this shader has been compiled and linked into a program
	 *         that can be bound and rendered with
	 */
	BOOL IsReadyForUse() const;
	
	/**
	 * @return TRUE if this shader is currently bound to the graphics device
	 */
	BOOL IsBound() const                                   { return m_isBound; }

	/**
	 * @return TRUE if the vertex shader was compiled without errors
	 */
	BOOL GetVertexShaderCompileStatus() const              { return m_vertexShaderCompileStatus; }

	/**
	 * @return TRUE if the fragment shader was compiled without errors
	 */
	BOOL GetFragmentShaderCompileStatus() const            { return m_fragmentShaderCompileStatus; }

	/**
	 * @return TRUE if the compiled vertex and fragment shaders were linked
	 *         without errors
	 */
	BOOL GetLinkStatus() const                             { return m_linkStatus; }

	/**
	 * @return the OpenGL program ID that can be bound
	 */
	uint32_t GetProgramId() const                          { return m_programId; }

	/**
	 * Checks if this shader contains a uniform. Note that the GLSL compiler
	 * could optimize out uniforms if they aren't used in the GLSL code.
	 * @param name the name of the uniform to check for
	 * @return TRUE if the uniform is present
	 */
	BOOL HasUniform(const stl::string &name) const;

	/**
	 * Checks if this shader contains an attribute. Note that the GLSL
	 * compiler could optimize out attributes if they aren't used in the
	 * GLSL code.
	 * @param name the name of the attribute to check for
	 * @return TRUE if the attribute is present
	 */
	BOOL HasAttribute(const stl::string &name) const;

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, float x);
	
	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, int32_t x);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, float x, float y);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, const Vector2 &v);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, int32_t x, int32_t y);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, const Point2 &p);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, float x, float y, float z);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, const Vector3 &v);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, int32_t x, int32_t y, int32_t z);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, const Point3 &p);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, float x, float y, float z, float w);
	
	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, const Vector4 &v);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, const Quaternion &q);
	
	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, const Color &c);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, int32_t x, int32_t y, int32_t z, int32_t w);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, const Matrix3x3 &m);

	/**
	 * Sets the value of a uniform.
	 * @param name the name of the uniform to set
	 */
	void SetUniform(const stl::string &name, const Matrix4x4 &m);

	void SetUniform(const stl::string &name, const float *x, uint32_t count);
	void SetUniform(const stl::string &name, const Vector2 *v, uint32_t count);
	void SetUniform(const stl::string &name, const Vector3 *v, uint32_t count);
	void SetUniform(const stl::string &name, const Vector4 *v, uint32_t count);
	void SetUniform(const stl::string &name, const Quaternion *q, uint32_t count);
	void SetUniform(const stl::string &name, const Color *c, uint32_t count);
	void SetUniform(const stl::string &name, const Matrix3x3 *m, uint32_t count);
	void SetUniform(const stl::string &name, const Matrix4x4 *m, uint32_t count);
	
	/**
	 * @return the number of attributes used in this shader
	 */
	uint32_t GetNumAttributes() const                      { return m_numAttributes; }

	/**
	 * Returns whether the given shader attribute has been mapped to a standard
	 * attribute type or not.
	 * @param attribIndex the index of the shader attribute to check the 
	 *                    mapping of
	 * @return TRUE if this shader attribute was mapped to a standard attribute
	 *         type or FALSE if it wasn't
	 */
	BOOL IsAttributeMappedToStandardType(uint32_t attribIndex) const;

	/**
	 * Gets a vertex buffer object attribute index that corresponds to the
	 * specified shader attribute.
	 * @param attribIndex the index of the shader attribute to get the mapping 
	 *                    for
	 * @return an index for a vertex buffer object's attributes that
	 *         corresponds to the specified shader attribute
	 */
	uint32_t GetAttributeMappedBufferIndex(uint32_t attribIndex) const;

	/**
	 * Gets the standard attribute type mapping corresponding to the given
	 * shader attribute.
	 * @param attribIndex the index of the shader attribute to get the mapping 
	 *                    for
	 * @return the standard attribute type mapping associated with this
	 *         shader attribute
	 */
	VERTEX_ATTRIBS GetAttributeMappedStandardType(uint32_t attribIndex) const;

	/**
	 * Maps the given shader attribute to an index that will be used to refer
	 * to an attribute in bound vertex buffer objects. This will be used by 
	 * GraphicsDevice when the shader is bound to automatically set up the 
	 * OpenGL vertex attribute mappings.
	 * @param name the name of the attribute to map
	 * @param vboAttribIndex the index value to map that will be used to refer
	 *                       to an attribute in bound vertex buffer objects
	 */
	void MapAttributeToVboAttribIndex(const stl::string &name, uint32_t vboAttribIndex);

	/**
	 * Maps the given shader attribute so that it will correspond to an
	 * attribute matching the provided standard type in bound vertex buffer
	 * objects, regardless of what attribute index it is located at. This will
	 * be used by GraphicsDevice when the shader is bound to automatically set
	 * up the OpenGL vertex attribute mappings.
	 * @param name the name of the attribute to map
	 * @param standardAttribType the standard type to map this shader attribute
	 *                           to in bound vertex buffer objects
	 */
	void MapAttributeToStandardAttribType(const stl::string &name, VERTEX_ATTRIBS standardAttribType);

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
	 * Initializes internal shader state in preparation for vertex and
	 * fragment sources to be provided.
	 */
	Shader();

	/**
	 * Loads the provided shader sources, compiles and links them and
	 * caches information about attributes and uniforms.
	 * @param vertexShaderSource vertex shader source
	 * @param fragmentShaderSource fragment shader source
	 * @return TRUE if compilation and linking succeeded and the shader
	 *              is now ready for use
	 */
	BOOL LoadCompileAndLink(const char *vertexShaderSource, const char *fragmentShaderSource);

	/**
	 * Reloads shader sources either from those provided or from the
	 * internal cache. This should be used when the OpenGL context
	 * was lost and recreated.
	 * @param vertexShaderSource vertex shader source, or NULL to load
	 *                           the source from the cache
	 * @param fragmentShaderSource fragment shader source, or NULL to load
	 *                             the source from the cache
	 * @return TRUE if compilation and linking succeeded and the shader
	 *              is now ready for use
	 */
	BOOL ReloadCompileAndLink(const char *vertexShaderSource, const char *fragmentShaderSource);

	/**
	 * Caches the provided shader sources. ReloadCompileAndLink() can
	 * be called with NULL arguments to reload from this cache later.
	 * @param vertexShaderSource vertex shader source to cache
	 * @param fragmentShaderSource fragment shader source to cache
	 */
	void CacheShaderSources(const char *vertexShaderSource, const char *fragmentShaderSource);

	/**
	 * Gets information about the specified uniform.
	 * @param name the name of the uniform to get info about
	 * @return information about the specified uniform or NULL if not found
	 */
	const ShaderUniform* GetUniform(const stl::string &name) const;

	/**
	 * Gets information about the specified uniform.
	 * @param name the name of the uniform to get info about
	 * @return information about the specified uniform or NULL if not found
	 */
	ShaderUniform* GetUniform(const stl::string &name);

	/**
	 * Gets information about the specified attribute.
	 * @param name the name of the attribute to get info about
	 * @return information about the specified attribute or NULL if not found
	 */
	const ShaderAttribute* GetAttribute(const stl::string &name) const;

	/**
	 * Gets information about the specified attribute.
	 * @param name the name of the attribute to get info about
	 * @return information about the specified attribute or NULL if not found
	 */
	ShaderAttribute* GetAttribute(const stl::string &name);

private:
	void Initialize();
	BOOL Compile(const char *vertexShaderSource, const char *fragmentShaderSource);
	BOOL Link();
	void LoadUniformInfo();
	void LoadAttributeInfo();

	void CacheUniform(const stl::string &name, CachedShaderUniform uniform);
	CachedShaderUniform* GetCachedUniform(const stl::string &name);
	void FlushCachedUniforms();

	void OnBind();
	void OnUnbind();

	char *m_cachedVertexShaderSource;
	char *m_cachedFragmentShaderSource;
	BOOL m_vertexShaderCompileStatus;
	BOOL m_fragmentShaderCompileStatus;
	BOOL m_linkStatus;
	uint32_t m_vertexShaderId;
	uint32_t m_fragmentShaderId;
	uint32_t m_programId;
	BOOL m_isBound;

	ShaderUniformMap m_uniforms;
	ShaderAttributeMap m_attributes;
	ShaderAttributeMapInfo *m_attributeMapping;
	uint32_t m_numAttributes;

	CachedShaderUniformMap m_cachedUniforms;
};

inline BOOL Shader::IsReadyForUse() const
{
	if (GetVertexShaderCompileStatus() && GetFragmentShaderCompileStatus() && GetLinkStatus())
		return TRUE;
	else
		return FALSE;
}

inline BOOL Shader::IsAttributeMappedToStandardType(uint32_t attribIndex) const
{
	return m_attributeMapping[attribIndex].usesStandardType;
}

inline uint32_t Shader::GetAttributeMappedBufferIndex(uint32_t attribIndex) const
{
	return m_attributeMapping[attribIndex].attribIndex;
}

inline VERTEX_ATTRIBS Shader::GetAttributeMappedStandardType(uint32_t attribIndex) const
{
	return m_attributeMapping[attribIndex].standardType;
}

#endif
