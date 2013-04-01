#include "../debug.h"
#include "../log.h"

#include "shader.h"
#include "color.h"
#include "glincludes.h"
#include "glutils.h"
#include "../math/matrix3x3.h"
#include "../math/matrix4x4.h"
#include "../math/point2.h"
#include "../math/point3.h"
#include "../math/quaternion.h"
#include "../math/vector2.h"
#include "../math/vector3.h"
#include "../math/vector4.h"
#include "../support/text.h"
#include <string.h>

// HACK: ensure these are all the sizes we expect without any compiler-added
//       alignment padding. some Shader::SetUniform() calls do memcpy's assuming
//       contiguous memory in arrays of these structs so this is very important!
//       (I have no idea if any of these asserts would ever not pass. Each of
//       these structs contains _only_ member variables each 4 bytes in size.)
STATIC_ASSERT(sizeof(Vector2) == 2 * sizeof(float));
STATIC_ASSERT(sizeof(Vector3) == 3 * sizeof(float));
STATIC_ASSERT(sizeof(Vector4) == 4 * sizeof(float));
STATIC_ASSERT(sizeof(Quaternion) == 4 * sizeof(float));
STATIC_ASSERT(sizeof(Matrix3x3) == 9 * sizeof(float));
STATIC_ASSERT(sizeof(Matrix4x4) == 16 * sizeof(float));
STATIC_ASSERT(sizeof(Point2) == 2 * sizeof(int32_t));
STATIC_ASSERT(sizeof(Point3) == 3 * sizeof(int32_t));

Shader::Shader()
{
	STACK_TRACE;
	m_isBound = FALSE;
	m_cachedVertexShaderSource = NULL;
	m_cachedFragmentShaderSource = NULL;
	m_vertexShaderCompileStatus = FALSE;
	m_fragmentShaderCompileStatus = FALSE;
	m_linkStatus = FALSE;
	m_vertexShaderId = 0;
	m_fragmentShaderId = 0;
	m_programId = 0;
	m_attributeMapping = NULL;
	m_numAttributes = 0;
}

BOOL Shader::Initialize(GraphicsDevice *graphicsDevice)
{
	STACK_TRACE;
	if (!GraphicsContextResource::Initialize(graphicsDevice))
		return FALSE;
	
	return TRUE;
}

BOOL Shader::Initialize(GraphicsDevice *graphicsDevice, const char *vertexShaderSource, const char *fragmentShaderSource)
{
	STACK_TRACE;
	if (!GraphicsContextResource::Initialize(graphicsDevice))
		return FALSE;
	
	ASSERT(vertexShaderSource != NULL);
	if (vertexShaderSource == NULL)
		return FALSE;
	
	ASSERT(fragmentShaderSource != NULL);
	if (fragmentShaderSource == NULL)
		return FALSE;

	if (!LoadCompileAndLink(vertexShaderSource, fragmentShaderSource))
		return FALSE;
	
	CacheShaderSources(vertexShaderSource, fragmentShaderSource);
	
	return TRUE;
}

BOOL Shader::Initialize(GraphicsDevice *graphicsDevice, const Text *vertexShaderSource, const Text *fragmentShaderSource)
{
	STACK_TRACE;
	ASSERT(vertexShaderSource != NULL && vertexShaderSource->GetLength() > 0);
	if (vertexShaderSource == NULL || vertexShaderSource->GetLength() == 0)
		return FALSE;
	
	ASSERT(fragmentShaderSource != NULL && fragmentShaderSource->GetLength() > 0);
	if (fragmentShaderSource == NULL && fragmentShaderSource->GetLength() == 0)
		return FALSE;
	
	return Initialize(graphicsDevice, vertexShaderSource->GetText(), fragmentShaderSource->GetText());
}

void Shader::Release()
{
	STACK_TRACE;
	if (m_vertexShaderId)
	{
		GL_CALL(glDeleteShader(m_vertexShaderId));
	}
	if (m_fragmentShaderId)
	{
		GL_CALL(glDeleteShader(m_fragmentShaderId));
	}
	if (m_programId)
	{
		GL_CALL(glDeleteProgram(m_programId));
	}
	
	SAFE_DELETE_ARRAY(m_attributeMapping);
	
	// if these were pointing to the same string, we obviously should only
	// delete once...
	if (m_cachedVertexShaderSource == m_cachedFragmentShaderSource)
	{
		SAFE_DELETE_ARRAY(m_cachedVertexShaderSource);
		m_cachedFragmentShaderSource = NULL;
	}
	else
	{
		SAFE_DELETE_ARRAY(m_cachedVertexShaderSource);
		SAFE_DELETE_ARRAY(m_cachedFragmentShaderSource);
	}

	m_isBound = FALSE;
	m_cachedVertexShaderSource = NULL;
	m_cachedFragmentShaderSource = NULL;
	m_vertexShaderCompileStatus = FALSE;
	m_fragmentShaderCompileStatus = FALSE;
	m_linkStatus = FALSE;
	m_vertexShaderId = 0;
	m_fragmentShaderId = 0;
	m_programId = 0;
	m_attributeMapping = NULL;
	m_numAttributes = 0;
	
	m_uniforms.clear();
	m_attributes.clear();
	m_cachedUniforms.clear();
	
	GraphicsContextResource::Release();
}

BOOL Shader::LoadCompileAndLink(const char *vertexShaderSource, const char *fragmentShaderSource)
{
	STACK_TRACE;
	const char *vertexShaderToLoad = vertexShaderSource;
	const char *fragmentShaderToLoad = fragmentShaderSource;

	// if no source was provided, see if there is some cached source to load instead
	if (vertexShaderToLoad == NULL)
		vertexShaderToLoad = m_cachedVertexShaderSource;
	if (fragmentShaderToLoad == NULL)
		fragmentShaderToLoad = m_cachedFragmentShaderSource;

	// must have *some* kind of source to load at any rate...
	ASSERT(vertexShaderToLoad != NULL);
	ASSERT(fragmentShaderToLoad != NULL);

	if (!Compile(vertexShaderToLoad, fragmentShaderToLoad))
		return FALSE;

	if (!Link())
		return FALSE;

	LoadUniformInfo();
	LoadAttributeInfo();

	return TRUE;
}

BOOL Shader::ReloadCompileAndLink(const char *vertexShaderSource, const char *fragmentShaderSource)
{
	STACK_TRACE;
	// clear out data that will be reset during the reload first
	m_isBound = FALSE;
	m_vertexShaderCompileStatus = FALSE;
	m_fragmentShaderCompileStatus = FALSE;
	m_linkStatus = FALSE;
	m_vertexShaderId = 0;
	m_fragmentShaderId = 0;
	m_programId = 0;
	m_uniforms.clear();
	m_attributes.clear();
	m_cachedUniforms.clear();

	// TODO: leaving the attribute type mappings intact. This could maybe be a problem?
	//       I think only if the attribute ID's can be assigned randomly by OpenGL even if
	//       the source remains the same each time would it ever be a problem to keep the
	//       old type mappings intact. Since OpenGL assigns the attribute index a zero-based
	//       number I have a feeling it is based on the declaration order in the shader
	//       source... so as long as the source doesn't change it should be the same

	// then load + compile/link
	return LoadCompileAndLink(vertexShaderSource, fragmentShaderSource);
}

void Shader::CacheShaderSources(const char *vertexShaderSource, const char *fragmentShaderSource)
{
	STACK_TRACE;
	ASSERT(vertexShaderSource != NULL);
	ASSERT(fragmentShaderSource != NULL);

	// we reallocate cache buffers if they already have source in them
	// (who knows, this may come in handy later for dynamic reloading of modified
	// shader source files ... ?)

	size_t vertexSourceLength = strlen(vertexShaderSource);
	SAFE_DELETE_ARRAY(m_cachedVertexShaderSource);
	m_cachedVertexShaderSource = new char[vertexSourceLength + 1];
	memcpy(m_cachedVertexShaderSource, vertexShaderSource, vertexSourceLength + 1);

	// if the given source buffers point to the same location, then we don't
	// need to allocate 2 cache buffers (they are the same string!). so just
	// copy the vertex source cache pointer and reuse
	if (vertexShaderSource == fragmentShaderSource)
		m_cachedFragmentShaderSource = m_cachedVertexShaderSource;
	else
	{
		size_t fragmentSourceLength = strlen(fragmentShaderSource);
		SAFE_DELETE_ARRAY(m_cachedFragmentShaderSource);
		m_cachedFragmentShaderSource = new char[fragmentSourceLength + 1];
		memcpy(m_cachedFragmentShaderSource, fragmentShaderSource, fragmentSourceLength + 1);
	}
}

BOOL Shader::Compile(const char *vertexShaderSource, const char *fragmentShaderSource)
{
	STACK_TRACE;
	ASSERT(m_vertexShaderId == 0);
	ASSERT(m_fragmentShaderId == 0);
	ASSERT(m_programId == 0);
	ASSERT(vertexShaderSource != NULL);
	ASSERT(fragmentShaderSource != NULL);

	// load and compile the vertex shader first...
	GL_CALL(m_vertexShaderId = glCreateShader(GL_VERTEX_SHADER));
	ASSERT(m_vertexShaderId != 0);

	// add in a special #define for convenience when we want to put both
	// vertex and fragment shaders in the same source file
	const char *vertexSources[2] = { "#define VERTEX\n", vertexShaderSource };
	GLint vertexSourcesLength[2];
	vertexSourcesLength[0] = strlen(vertexSources[0]);
	vertexSourcesLength[1] = strlen(vertexSources[1]);
	GL_CALL(glShaderSource(m_vertexShaderId, 2, vertexSources, vertexSourcesLength));

	GL_CALL(glCompileShader(m_vertexShaderId));
	GL_CALL(glGetShaderiv(m_vertexShaderId, GL_COMPILE_STATUS, &m_vertexShaderCompileStatus));

	// log compiler errors
	if (!m_vertexShaderCompileStatus)
	{
		GLint logLength = 0;
		GL_CALL(glGetShaderiv(m_vertexShaderId, GL_INFO_LOG_LENGTH, &logLength));
		if (logLength > 1)
		{
			char *infoLog = new char[logLength];
			ASSERT(infoLog != NULL);

			GLsizei infoLogLength = 0;
			GL_CALL(glGetShaderInfoLog(m_vertexShaderId, logLength, &infoLogLength, infoLog));
			LOG_ERROR(LOGCAT_OPENGL, "Error compiling vertex shader:\n%s\n", infoLog);

			SAFE_DELETE(infoLog);
		}
	}

	// ... and the fragment shader next
	GL_CALL(m_fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER));
	ASSERT(m_fragmentShaderId != 0);

	// add in a special #define for convenience when we want to put both
	// vertex and fragment shaders in the same source file
	const char *fragmentSources[2] = { "#define FRAGMENT\n", fragmentShaderSource };
	GLint fragmentSourcesLength[2];
	fragmentSourcesLength[0] = strlen(fragmentSources[0]);
	fragmentSourcesLength[1] = strlen(fragmentSources[1]);
	GL_CALL(glShaderSource(m_fragmentShaderId, 2, fragmentSources, fragmentSourcesLength));

	GL_CALL(glCompileShader(m_fragmentShaderId));
	GL_CALL(glGetShaderiv(m_fragmentShaderId, GL_COMPILE_STATUS, &m_fragmentShaderCompileStatus));

	// log compiler errors
	if (!m_fragmentShaderCompileStatus)
	{
		GLint logLength = 0;
		GL_CALL(glGetShaderiv(m_fragmentShaderId, GL_INFO_LOG_LENGTH, &logLength));
		if (logLength > 1)
		{
			char *infoLog = new char[logLength];
			ASSERT(infoLog != NULL);

			GLsizei infoLogLength = 0;
			GL_CALL(glGetShaderInfoLog(m_fragmentShaderId, logLength, &infoLogLength, infoLog));
			LOG_ERROR(LOGCAT_OPENGL, "Error compiling fragment shader:\n%s\n", infoLog);

			SAFE_DELETE(infoLog);
		}
	}

	// only return success if both compiled successfully
	if (m_fragmentShaderCompileStatus && m_vertexShaderCompileStatus)
		return TRUE;
	else
		return FALSE;
}

BOOL Shader::Link()
{
	STACK_TRACE;
	ASSERT(m_vertexShaderId != 0);
	ASSERT(m_fragmentShaderId != 0);
	ASSERT(m_programId == 0);

	GL_CALL(m_programId = glCreateProgram());
	ASSERT(m_programId != 0);

	GL_CALL(glAttachShader(m_programId, m_vertexShaderId));
	GL_CALL(glAttachShader(m_programId, m_fragmentShaderId));

	GL_CALL(glLinkProgram(m_programId));
	GL_CALL(glGetProgramiv(m_programId, GL_LINK_STATUS, &m_linkStatus));

	// log linker errors
	if (!m_linkStatus)
	{
		GLint logLength = 0;
		GL_CALL(glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &logLength));
		if (logLength > 1)
		{
			char *infoLog = new char[logLength];
			ASSERT(infoLog != NULL);

			GL_CALL(glGetProgramInfoLog(m_programId, logLength, NULL, infoLog));
			LOG_ERROR(LOGCAT_OPENGL, "Error linking program:\n%s\n", infoLog);

			SAFE_DELETE(infoLog);
		}
	}

	return m_linkStatus;
}

void Shader::LoadUniformInfo()
{
	STACK_TRACE;
	ASSERT(m_programId != 0);

	m_uniforms.clear();

	GLint numUniforms = 0;
	GL_CALL(glGetProgramiv(m_programId, GL_ACTIVE_UNIFORMS, &numUniforms));

	// don't need to proceed if there are no uniforms
	if (numUniforms == 0)
		return;

	// get the maximum uniform name length for a temporary buffer to hold them
	GLint maxUniformNameLength = 0;
	GL_CALL(glGetProgramiv(m_programId, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformNameLength));
	ASSERT(maxUniformNameLength > 0);
	char *uniformName = new char[maxUniformNameLength];

	// get info about each uniform
	for (int32_t i = 0; i < numUniforms; ++i)
	{
		GLint size;
		GLenum type;
		GLint location;

		GL_CALL(glGetActiveUniform(m_programId, i, maxUniformNameLength, NULL, &size, &type, uniformName));
		GL_CALL(location = glGetUniformLocation(m_programId, uniformName));
		ASSERT(location != -1);   // means the uniform is not active... this assert shouldn't ever fail really...

		// store the uniform info
		ShaderUniform uniform;
		uniform.location = (uint32_t)location;
		uniform.type = (uint32_t)type;
		uniform.size = (uint32_t)size;
		stl::string name = uniformName;

		// it seems Windows/Mac (possibly Linux too) have differing opinions on
		// including "[0]" in the uniform name for uniforms that are arrays
		// we'll just chop any "[0]" off if found in the uniform name before we 
		// add it to our list
		size_t arraySubscriptPos = name.find("[0]");
		if (arraySubscriptPos != stl::string::npos)
			name = name.substr(0, arraySubscriptPos);

		m_uniforms[name] = uniform;
	}

	SAFE_DELETE_ARRAY(uniformName);
}

void Shader::LoadAttributeInfo()
{
	STACK_TRACE;
	ASSERT(m_programId != 0);

	m_attributes.clear();

	GLint numAttributes = 0;
	GL_CALL(glGetProgramiv(m_programId, GL_ACTIVE_ATTRIBUTES, &numAttributes));

	// sanity checking, which only matters for shader reloading (e.g. when a context is lost)
	if (m_attributeMapping != NULL)
	{
		ASSERT((uint32_t)numAttributes == m_numAttributes);
	}

	m_numAttributes = numAttributes;

	// don't need to proceed if there are no attributes
	if (numAttributes == 0)
		return;

	// leave existing attribute type mappings (they will already be there e.g. when a context is lost)
	if (m_attributeMapping == NULL)
	{
		// just allocate this for now, will be bound later on
		m_attributeMapping = new ShaderAttributeMapInfo[m_numAttributes];
		memset(m_attributeMapping, 0, sizeof(ShaderAttributeMapInfo) * m_numAttributes);
	}

	// get the maximum attribute name length for a temporary buffer to hold them
	GLint maxAttributeNameLength = 0;
	GL_CALL(glGetProgramiv(m_programId, GL_ACTIVE_ATTRIBUTE_MAX_LENGTH, &maxAttributeNameLength));
	ASSERT(maxAttributeNameLength > 0);
	char *attributeName = new char[maxAttributeNameLength];

	// get info about each attribute
	for (int32_t i = 0; i < numAttributes; ++i)
	{
		GLint size;
		GLenum type;
		GLint location;

		GL_CALL(glGetActiveAttrib(m_programId, i, maxAttributeNameLength, NULL, &size, &type, attributeName));
		GL_CALL(location = glGetAttribLocation(m_programId, attributeName));
		ASSERT(location != -1);   // means the attribute is not active... this assert shouldn't ever fail really...

		// store the attribute info
		ShaderAttribute attribute;
		attribute.location = (uint32_t)location;
		attribute.type = (uint32_t)type;
		attribute.size = (uint32_t)size;
		attribute.isTypeBound = FALSE;
		stl::string name = attributeName;

		m_attributes[name] = attribute;
	}

	SAFE_DELETE_ARRAY(attributeName);
}

BOOL Shader::HasUniform(const stl::string &name) const
{
	STACK_TRACE;
	ShaderUniformMap::const_iterator i = m_uniforms.find(name);
	if (i == m_uniforms.end())
		return FALSE;
	else
		return TRUE;
}

const ShaderUniform* Shader::GetUniform(const stl::string &name) const
{
	STACK_TRACE;
	ShaderUniformMap::const_iterator i = m_uniforms.find(name);
	if (i == m_uniforms.end())
		return NULL;
	else
		return &i->second;
}

ShaderUniform* Shader::GetUniform(const stl::string &name)
{
	STACK_TRACE;
	ShaderUniformMap::iterator i = m_uniforms.find(name);
	if (i == m_uniforms.end())
		return NULL;
	else
		return &i->second;
}

BOOL Shader::HasAttribute(const stl::string &name) const
{
	STACK_TRACE;
	ShaderAttributeMap::const_iterator i = m_attributes.find(name);
	if (i == m_attributes.end())
		return FALSE;
	else
		return TRUE;
}

const ShaderAttribute* Shader::GetAttribute(const stl::string &name) const
{
	STACK_TRACE;
	ShaderAttributeMap::const_iterator i = m_attributes.find(name);
	if (i == m_attributes.end())
		return NULL;
	else
		return &i->second;
}

ShaderAttribute* Shader::GetAttribute(const stl::string &name)
{
	STACK_TRACE;
	ShaderAttributeMap::iterator i = m_attributes.find(name);
	if (i == m_attributes.end())
		return NULL;
	else
		return &i->second;
}

void Shader::CacheUniform(const stl::string &name, CachedShaderUniform uniform)
{
	STACK_TRACE;
	m_cachedUniforms[name] = uniform;
}

CachedShaderUniform* Shader::GetCachedUniform(const stl::string &name)
{
	STACK_TRACE;
	CachedShaderUniformMap::iterator i = m_cachedUniforms.find(name);
	if (i == m_cachedUniforms.end())
	{
		m_cachedUniforms[name] = CachedShaderUniform();
		i = m_cachedUniforms.find(name);
	}

	return &i->second;
}

void Shader::FlushCachedUniforms()
{
	STACK_TRACE;
	ASSERT(m_isBound == TRUE);
	if (m_cachedUniforms.empty())
		return;

	for (CachedShaderUniformMap::iterator i = m_cachedUniforms.begin(); i != m_cachedUniforms.end(); ++i)
	{
		const stl::string &name = i->first;
		CachedShaderUniform &uniform = i->second;

		switch (uniform.type)
		{
		case CACHED_UNIFORM_1F:   SetUniform(name, uniform.f1.x); break;
		case CACHED_UNIFORM_1I:   SetUniform(name, uniform.i1.x); break;
		case CACHED_UNIFORM_2F:   SetUniform(name, uniform.f2.x, uniform.f2.y); break;
		case CACHED_UNIFORM_2I:   SetUniform(name, uniform.i2.x, uniform.i2.y); break;
		case CACHED_UNIFORM_3F:   SetUniform(name, uniform.f3.x, uniform.f3.y, uniform.f3.z); break;
		case CACHED_UNIFORM_3I:   SetUniform(name, uniform.i3.x, uniform.i3.y, uniform.i3.z); break;
		case CACHED_UNIFORM_4F:   SetUniform(name, uniform.f4.x, uniform.f4.y, uniform.f4.z, uniform.f4.w); break;
		case CACHED_UNIFORM_4I:   SetUniform(name, uniform.i4.x, uniform.i4.y, uniform.i4.z, uniform.i4.w); break;
		case CACHED_UNIFORM_9F:
			SetUniform(name, Matrix3x3(uniform.f9.m));
			break;
		case CACHED_UNIFORM_16F:
			SetUniform(name, Matrix4x4(uniform.f16.m));
			break;
		}
	}

	m_cachedUniforms.clear();
}

void Shader::SetUniform(const stl::string &name, float x)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform1f(uniform->location, x));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_1F;
		uniform.f1.x = x;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, int32_t x)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform1i(uniform->location, x));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_1I;
		uniform.i1.x = x;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, float x, float y)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform2f(uniform->location, x, y));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_2F;
		uniform.f2.x = x;
		uniform.f2.y = y;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, const Vector2 &v)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform2f(uniform->location, v.x, v.y));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_2F;
		uniform.f2.x = v.x;
		uniform.f2.y = v.y;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, int32_t x, int32_t y)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform2i(uniform->location, x, y));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_2I;
		uniform.i2.x = x;
		uniform.i2.y = y;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, const Point2 &p)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform2i(uniform->location, p.x, p.y));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_2I;
		uniform.i2.x = p.x;
		uniform.i2.y = p.y;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, float x, float y, float z)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform3f(uniform->location, x, y, z));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_3F;
		uniform.f3.x = x;
		uniform.f3.y = y;
		uniform.f3.z = z;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, const Vector3 &v)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform3f(uniform->location, v.x, v.y, v.z));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_3F;
		uniform.f3.x = v.x;
		uniform.f3.y = v.y;
		uniform.f3.z = v.z;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, int32_t x, int32_t y, int32_t z)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform3i(uniform->location, x, y, z));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_3I;
		uniform.i3.x = x;
		uniform.i3.y = y;
		uniform.i3.z = z;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, const Point3 &p)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform3i(uniform->location, p.x, p.y, p.z));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_3I;
		uniform.i3.x = p.x;
		uniform.i3.y = p.y;
		uniform.i3.z = p.z;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, float x, float y, float z, float w)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform4f(uniform->location, x, y, z, w));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_4F;
		uniform.f4.x = x;
		uniform.f4.y = y;
		uniform.f4.z = z;
		uniform.f4.w = w;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, const Vector4 &v)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform4f(uniform->location, v.x, v.y, v.z, v.w));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_4F;
		uniform.f4.x = v.x;
		uniform.f4.y = v.y;
		uniform.f4.z = v.z;
		uniform.f4.w = v.w;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, const Quaternion &q)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform4f(uniform->location, q.x, q.y, q.z, q.w));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_4F;
		uniform.f4.x = q.x;
		uniform.f4.y = q.y;
		uniform.f4.z = q.z;
		uniform.f4.w = q.w;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, const Color &c)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform4f(uniform->location, c.r, c.g, c.b, c.a));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_4F;
		uniform.f4.x = c.r;
		uniform.f4.y = c.g;
		uniform.f4.z = c.b;
		uniform.f4.w = c.a;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, int32_t x, int32_t y, int32_t z, int32_t w)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniform4i(uniform->location, x, y, z, w));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_4I;
		uniform.i4.x = x;
		uniform.i4.y = y;
		uniform.i4.z = z;
		uniform.i4.w = w;
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, const Matrix3x3 &m)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniformMatrix3fv(uniform->location, 1, FALSE, m.m));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_9F;
		memcpy(uniform.f9.m, m.m, sizeof(float) * 9);
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, const Matrix4x4 &m)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size == 1);
		GL_CALL(glUniformMatrix4fv(uniform->location, 1, FALSE, m.m));
	}
	else
	{
		CachedShaderUniform uniform;
		uniform.type = CACHED_UNIFORM_16F;
		memcpy(uniform.f16.m, m.m, sizeof(float) * 16);
		CacheUniform(name, uniform);
	}
}

void Shader::SetUniform(const stl::string &name, const float *x, uint32_t count)
{
	STACK_TRACE;
	if (m_isBound)
	{
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size >= count);
		GL_CALL(glUniform1fv(uniform->location, count, x));
	}
	else
	{
		ASSERT(!"Unimplemented");
	}
}

void Shader::SetUniform(const stl::string &name, const Vector2 *v, uint32_t count)
{
	STACK_TRACE;
	if (m_isBound)
	{
		// TODO: erm... this _seems_ unnecessarily ugly... even for me
		const float *values = (const float*)v;
		
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size >= count);
		GL_CALL(glUniform2fv(uniform->location, count, values));
	}
	else
	{
		ASSERT(!"Unimplemented");
	}
}

void Shader::SetUniform(const stl::string &name, const Vector3 *v, uint32_t count)
{
	STACK_TRACE;
	if (m_isBound)
	{
		// TODO: erm... this _seems_ unnecessarily ugly... even for me
		const float *values = (const float*)v;

		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size >= count);
		GL_CALL(glUniform3fv(uniform->location, count, values));
	}
	else
	{
		ASSERT(!"Unimplemented");
	}
}

void Shader::SetUniform(const stl::string &name, const Vector4 *v, uint32_t count)
{
	STACK_TRACE;
	if (m_isBound)
	{
		// TODO: erm... this _seems_ unnecessarily ugly... even for me
		const float *values = (const float*)v;
		
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size >= count);
		GL_CALL(glUniform4fv(uniform->location, count, values));
	}
	else
	{
		ASSERT(!"Unimplemented");
	}
}

void Shader::SetUniform(const stl::string &name, const Quaternion *q, uint32_t count)
{
	STACK_TRACE;
	if (m_isBound)
	{
		// TODO: erm... this _seems_ unnecessarily ugly... even for me
		const float *values = (const float*)q;

		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size >= count);

		GL_CALL(glUniform4fv(uniform->location, count, values));
	}
	else
	{
		ASSERT(!"Unimplemented");
	}
}

void Shader::SetUniform(const stl::string &name, const Color *c, uint32_t count)
{
	STACK_TRACE;
	if (m_isBound)
	{
		// TODO: erm... this _seems_ unnecessarily ugly... even for me
		const float *values = (const float*)c;
		
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size >= count);

		GL_CALL(glUniform4fv(uniform->location, count, values));
	}
	else
	{
		ASSERT(!"Unimplemented");
	}
}

void Shader::SetUniform(const stl::string &name, const Matrix3x3 *m, uint32_t count)
{
	STACK_TRACE;
	if (m_isBound)
	{
		// TODO: erm... this _seems_ unnecessarily ugly... even for me
		const float *values = (const float*)m;

		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size >= count);
		
		GL_CALL(glUniformMatrix3fv(uniform->location, count, FALSE, values));
	}
	else
	{
		ASSERT(!"Unimplemented");
	}
}

void Shader::SetUniform(const stl::string &name, const Matrix4x4 *m, uint32_t count)
{
	STACK_TRACE;
	if (m_isBound)
	{
		// TODO: erm... this _seems_ unnecessarily ugly... even for me
		const float *values = (const float*)m;
		
		const ShaderUniform *uniform = GetUniform(name);
		ASSERT(uniform != NULL);
		ASSERT(uniform->size >= count);
		
		GL_CALL(glUniformMatrix4fv(uniform->location, count, FALSE, values));
	}
	else
	{
		ASSERT(!"Unimplemented");
	}
}

void Shader::MapAttributeToVboAttribIndex(const stl::string &name, uint32_t vboAttribIndex)
{
	STACK_TRACE;
	ShaderAttribute *attribute = GetAttribute(name);
	ASSERT(attribute != NULL);
	ASSERT(attribute->location < m_numAttributes);

	ShaderAttributeMapInfo *mappingInfo = &m_attributeMapping[attribute->location];
	mappingInfo->usesStandardType = FALSE;
	mappingInfo->attribIndex = vboAttribIndex;

	attribute->isTypeBound = TRUE;
}

void Shader::MapAttributeToStandardAttribType(const stl::string &name, VERTEX_STANDARD_ATTRIBS standardAttribType)
{
	STACK_TRACE;
	ShaderAttribute *attribute = GetAttribute(name);
	ASSERT(attribute != NULL);
	ASSERT(attribute->location < m_numAttributes);

	ShaderAttributeMapInfo *mappingInfo = &m_attributeMapping[attribute->location];
	mappingInfo->usesStandardType = TRUE;
	mappingInfo->standardType = standardAttribType;

	attribute->isTypeBound = TRUE;
}

void Shader::OnNewContext()
{
	STACK_TRACE;
	ReloadCompileAndLink(NULL, NULL);
}

void Shader::OnLostContext()
{
	STACK_TRACE;
}

void Shader::OnBind()
{
	STACK_TRACE;
	ASSERT(m_isBound == FALSE);
	m_isBound = TRUE;
	FlushCachedUniforms();
}

void Shader::OnUnbind()
{
	STACK_TRACE;
	ASSERT(m_isBound == TRUE);
	m_isBound = FALSE;
	m_cachedUniforms.clear();
}
