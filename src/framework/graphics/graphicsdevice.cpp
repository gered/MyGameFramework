#include "../debug.h"
#include "../log.h"

#include "graphicsdevice.h"

#include "blendstate.h"
#include "bufferobject.h"
#include "color.h"
#include "debugshader.h"
#include "framebuffer.h"
#include "framebufferdatatypes.h"
#include "geometrydebugrenderer.h"
#include "glincludes.h"
#include "glutils.h"
#include "graphicscontextresource.h"
#include "image.h"
#include "imageformats.h"
#include "indexbuffer.h"
#include "renderbuffer.h"
#include "renderstate.h"
#include "shader.h"
#include "simplecolorshader.h"
#include "simplecolortextureshader.h"
#include "simpletextureshader.h"
#include "simpletexturevertexlerpshader.h"
#include "simpletexturevertexskinningshader.h"
#include "standardshader.h"
#include "solidcolortexturecache.h"
#include "sprite2dshader.h"
#include "sprite3dshader.h"
#include "texture.h"
#include "textureformats.h"
#include "vertexattribs.h"
#include "vertexbuffer.h"
#include "viewcontext.h"
#include "../gamewindow.h"
#include "../screenorientation.h"
#include "../math/camera.h"
#include "../math/mathhelpers.h"

// these are based on the OpenGL ES 2.0 spec minimum values
const unsigned int MAX_BOUND_TEXTURES = 8;
const unsigned int MAX_GPU_ATTRIB_SLOTS = 8; 

GraphicsDevice::GraphicsDevice()
{
	m_hasNewContextRunYet = false;
	m_boundVertexBuffer = NULL;
	m_boundIndexBuffer = NULL;
	m_boundShader = NULL;
	m_shaderVertexAttribsSet = false;
	m_boundTextures = NULL;
	m_boundFramebuffer = NULL;
	m_boundRenderbuffer = NULL;
	m_activeViewContext = NULL;
	m_defaultViewContext = NULL;
	m_debugRenderer = NULL;
	m_solidColorTextures = NULL;
	m_simpleColorShader = NULL;
	m_simpleColorTextureShader = NULL;
	m_simpleTextureShader = NULL;
	m_simpleTextureVertexLerpShader = NULL;
	m_simpleTextureVertexSkinningShader = NULL;
	m_sprite2dShader = NULL;
	m_sprite3dShader = NULL;
	m_debugShader = NULL;
	m_isDepthTextureSupported = false;
	m_isNonPowerOfTwoTextureSupported = false;
	m_window = NULL;
}

bool GraphicsDevice::Initialize(GameWindow *window)
{
	ASSERT(m_window == NULL);
	if (m_window != NULL)
		return false;
	
	ASSERT(window != NULL);
	if (window == NULL)
		return false;
	
	m_hasNewContextRunYet = false;
	m_boundTextures = new const Texture*[MAX_BOUND_TEXTURES];
	m_enabledVertexAttribIndices.reserve(MAX_GPU_ATTRIB_SLOTS);

#ifdef MOBILE
	m_isDepthTextureSupported = IsGLExtensionPresent("OES_depth_texture");
	m_isNonPowerOfTwoTextureSupported = IsGLExtensionPresent("OES_texture_npot");
#else
	// TODO: Is this a good enough "catch-all" check for desktops?
	m_isDepthTextureSupported = IsGLExtensionPresent("ARB_depth_texture");
	
	m_isNonPowerOfTwoTextureSupported = IsGLExtensionPresent("ARB_texture_non_power_of_two");
#endif
	
	LOG_INFO(LOGCAT_GRAPHICS, "Support for depth textures was %s.\n", m_isDepthTextureSupported ? "found" : "not found");
	LOG_INFO(LOGCAT_GRAPHICS, "Support for NPOT textures was %s.\n", m_isNonPowerOfTwoTextureSupported ? "found" : "not found");
	
	m_window = window;

	m_defaultViewContext = new ViewContext();
	m_defaultViewContext->Create(this);
	m_activeViewContext = m_defaultViewContext;
	
	m_currentTextureParams = TEXPARAM_DEFAULT;
	
	m_solidColorTextures = new SolidColorTextureCache(this);
	
	return true;
}

void GraphicsDevice::Release()
{
	SAFE_DELETE(m_defaultViewContext);
	SAFE_DELETE(m_debugRenderer);
	SAFE_DELETE(m_solidColorTextures);
	SAFE_DELETE(m_simpleColorShader);
	SAFE_DELETE(m_simpleColorTextureShader);
	SAFE_DELETE(m_simpleTextureShader);
	SAFE_DELETE(m_simpleTextureVertexLerpShader);
	SAFE_DELETE(m_simpleTextureVertexSkinningShader);
	SAFE_DELETE(m_sprite2dShader);
	SAFE_DELETE(m_sprite3dShader);
	SAFE_DELETE(m_debugShader);
	SAFE_DELETE_ARRAY(m_boundTextures);
	m_enabledVertexAttribIndices.clear();
	m_managedResources.clear();
	
	m_hasNewContextRunYet = false;
	m_boundVertexBuffer = NULL;
	m_boundIndexBuffer = NULL;
	m_boundShader = NULL;
	m_shaderVertexAttribsSet = false;
	m_boundFramebuffer = NULL;
	m_boundRenderbuffer = NULL;
	m_activeViewContext = NULL;
	m_isDepthTextureSupported = false;
	m_isNonPowerOfTwoTextureSupported = false;
	m_window = NULL;
}

SimpleColorShader* GraphicsDevice::GetSimpleColorShader()
{
	if (m_simpleColorShader == NULL)
	{
		m_simpleColorShader = new SimpleColorShader();
		m_simpleColorShader->Initialize(this);
	}
	
	return m_simpleColorShader;
}

SimpleColorTextureShader* GraphicsDevice::GetSimpleColorTextureShader()
{
	if (m_simpleColorTextureShader == NULL)
	{
		m_simpleColorTextureShader = new SimpleColorTextureShader();
		m_simpleColorTextureShader->Initialize(this);
	}
	
	return m_simpleColorTextureShader;
}

SimpleTextureShader* GraphicsDevice::GetSimpleTextureShader()
{
	if (m_simpleTextureShader == NULL)
	{
		m_simpleTextureShader = new SimpleTextureShader();
		m_simpleTextureShader->Initialize(this);
	}
	
	return m_simpleTextureShader;
}

Sprite2DShader* GraphicsDevice::GetSprite2DShader()
{
	if (m_sprite2dShader == NULL)
	{
		m_sprite2dShader = new Sprite2DShader();
		m_sprite2dShader->Initialize(this);
	}
	
	return m_sprite2dShader;
}

Sprite3DShader* GraphicsDevice::GetSprite3DShader()
{
	if (m_sprite3dShader == NULL)
	{
		m_sprite3dShader = new Sprite3DShader();
		m_sprite3dShader->Initialize(this);
	}
	
	return m_sprite3dShader;
}

SimpleTextureVertexLerpShader* GraphicsDevice::GetSimpleTextureVertexLerpShader()
{
	if (m_simpleTextureVertexLerpShader == NULL)
	{
		m_simpleTextureVertexLerpShader = new SimpleTextureVertexLerpShader();
		m_simpleTextureVertexLerpShader->Initialize(this);
	}
	
	return m_simpleTextureVertexLerpShader;
}

SimpleTextureVertexSkinningShader* GraphicsDevice::GetSimpleTextureVertexSkinningShader()
{
	if (m_simpleTextureVertexSkinningShader == NULL)
	{
		m_simpleTextureVertexSkinningShader = new SimpleTextureVertexSkinningShader();
		m_simpleTextureVertexSkinningShader->Initialize(this);
	}
	
	return m_simpleTextureVertexSkinningShader;
}

DebugShader* GraphicsDevice::GetDebugShader()
{
	if (m_debugShader == NULL)
	{
		m_debugShader = new DebugShader();
		m_debugShader->Initialize(this);
	}
	
	return m_debugShader;
}

void GraphicsDevice::OnNewContext()
{
	LOG_INFO(LOGCAT_GRAPHICS, "Initializing default state for new OpenGL context.\n");

	m_activeViewContext->OnNewContext();

	RENDERSTATE_DEFAULT.Apply();
	BLENDSTATE_DEFAULT.Apply();

	UnbindVertexBuffer();
	UnbindIndexBuffer();
	for (uint i = 0; i < MAX_BOUND_TEXTURES; ++i)
		UnbindTexture(i);
	UnbindShader();
	UnbindRenderbuffer();
	UnbindFramebuffer();

	m_debugRenderer = new GeometryDebugRenderer(this);

	// don't attempt to restore old graphics resources if this is the first
	// new context event (meaning, any managed resources will be loaded at this point!)
	if (m_hasNewContextRunYet)
	{
		m_solidColorTextures->OnNewContext();
		
		for (ManagedResourceList::iterator i = m_managedResources.begin(); i != m_managedResources.end(); ++i)
			(*i)->OnNewContext();
	}
	
	m_hasNewContextRunYet = true;
}

void GraphicsDevice::OnLostContext()
{
	LOG_INFO(LOGCAT_GRAPHICS, "Cleaning up objects/state specific to the lost OpenGL context.\n");
	
	m_activeViewContext->OnLostContext();

	SAFE_DELETE(m_debugRenderer);

	m_solidColorTextures->OnLostContext();

	for (ManagedResourceList::iterator i = m_managedResources.begin(); i != m_managedResources.end(); ++i)
		(*i)->OnLostContext();
}

void GraphicsDevice::OnResize(const Rect &size)
{
	LOG_INFO(LOGCAT_GRAPHICS, "Window resized (%d, %d) - (%d, %d).\n", size.left, size.top, size.GetWidth(), size.GetHeight());
	if (m_window->GetScreenOrientation() != SCREEN_ANGLE_0)
		LOG_INFO(LOGCAT_GRAPHICS, "Screen is rotated (angle = %d).\n", (int)m_window->GetScreenOrientation());
	m_activeViewContext->OnResize(size, m_window->GetScreenOrientation());
}

void GraphicsDevice::OnRender()
{
	GLenum error = glGetError();
	ASSERT(error == GL_NO_ERROR);
	if (error != GL_NO_ERROR)
	{
		LOG_ERROR(LOGCAT_OPENGL, "OpenGL error \"%s\"\n", GetGLErrorString(error));

		// keep checking for and reporting errors until there are no more left
		while ((error = glGetError()) != GL_NO_ERROR)
			LOG_ERROR(LOGCAT_OPENGL, "OpenGL error \"%s\"\n", GetGLErrorString(error));
	}
	
	m_activeViewContext->OnRender();
}

void GraphicsDevice::Clear(float r, float g, float b, float a)
{
	GL_CALL(glClearColor(r, g, b, a));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void GraphicsDevice::Clear(const Color &color)
{
	Clear(color.r, color.g, color.b, color.a);
}

Texture* GraphicsDevice::GetSolidColorTexture(const Color &color)
{
	return m_solidColorTextures->Get(color);
}

void GraphicsDevice::BindTexture(const Texture *texture, uint unit)
{
	ASSERT(unit < MAX_BOUND_TEXTURES);
	ASSERT(texture != NULL);
	ASSERT(texture->IsInvalidated() == false);
	if (texture != m_boundTextures[unit])
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, texture->GetTextureName()));
	}
	m_boundTextures[unit] = texture;
}

void GraphicsDevice::BindSolidColorTexture(const Color &color, uint unit)
{
	Texture *texture = m_solidColorTextures->Get(color);
	BindTexture(texture, unit);
}

void GraphicsDevice::UnbindTexture(uint unit)
{
	ASSERT(unit < MAX_BOUND_TEXTURES);
	GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	m_boundTextures[unit] = NULL;
}

void GraphicsDevice::UnbindTexture(const Texture *texture)
{
	ASSERT(texture != NULL);
	if (texture == NULL)
		return;
	
	for (uint i = 0; i < MAX_BOUND_TEXTURES; ++i)
	{
		if (m_boundTextures[i] == texture)
			UnbindTexture(i);
	}
}

void GraphicsDevice::BindRenderbuffer(Renderbuffer *renderbuffer)
{
	ASSERT(renderbuffer != NULL);
	ASSERT(renderbuffer->IsInvalidated() == false);
	if (m_boundRenderbuffer != renderbuffer)
	{
		GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer->GetRenderbufferName()));
		m_boundRenderbuffer = renderbuffer;
	}
}

void GraphicsDevice::UnbindRenderbuffer()
{
	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	m_boundRenderbuffer = NULL;
}

void GraphicsDevice::UnbindRenderBuffer(Renderbuffer *renderBuffer)
{
	ASSERT(renderBuffer != NULL);
	if (renderBuffer == NULL)
		return;
	
	if (m_boundRenderbuffer == renderBuffer)
		UnbindRenderbuffer();
}

void GraphicsDevice::BindFramebuffer(Framebuffer *framebuffer)
{
	ASSERT(framebuffer != NULL);
	ASSERT(framebuffer->IsInvalidated() == false);
	if (m_boundFramebuffer != framebuffer)
	{
		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->GetFramebufferName()));
		m_boundFramebuffer = framebuffer;
		m_boundFramebuffer->OnBind();
	}
}

void GraphicsDevice::UnbindFramebuffer()
{
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	if (m_boundFramebuffer != NULL)
		m_boundFramebuffer->OnUnBind();
	m_boundFramebuffer = NULL;
}

void GraphicsDevice::UnbindFramebuffer(Framebuffer *framebuffer)
{
	ASSERT(framebuffer != NULL);
	if (framebuffer == NULL)
		return;
	
	if (m_boundFramebuffer == framebuffer)
		UnbindFramebuffer();
}

void GraphicsDevice::SetViewContext(ViewContext *viewContext)
{
	if (viewContext == m_activeViewContext)
		return;   // nothing has changed
	
	if (viewContext != NULL)
		m_activeViewContext = viewContext;
	else
		m_activeViewContext = m_defaultViewContext;
	
	m_activeViewContext->OnApply(m_window->GetRect(), m_window->GetScreenOrientation());
}

void GraphicsDevice::RegisterManagedResource(GraphicsContextResource *resource)
{
	ASSERT(resource != NULL);

	// make sure this resource isn't in our list already
	stl::list<BufferObject*>::iterator i;
	for (ManagedResourceList::iterator i = m_managedResources.begin(); i != m_managedResources.end(); ++i)
	{
		ASSERT((*i) != resource);
		if ((*i) == resource)
			return;
	}

	m_managedResources.push_back(resource);
}

void GraphicsDevice::UnregisterManagedResource(GraphicsContextResource *resource)
{
	ASSERT(resource != NULL);
	m_managedResources.remove(resource);
}

void GraphicsDevice::UnregisterAllManagedResources()
{
	m_managedResources.clear();
}

void GraphicsDevice::BindVertexBuffer(VertexBuffer *buffer)
{
	ASSERT(buffer != NULL);
	ASSERT(buffer->GetNumElements() > 0);

	// don't bind this buffer if it's already bound!
	if (m_boundVertexBuffer == buffer)
		return;

	if (!buffer->IsClientSideBuffer())
		BindVBO(buffer);
	else
		BindClientBuffer(buffer);

	m_boundVertexBuffer = buffer;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();
}

void GraphicsDevice::UnbindVertexBuffer()
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

	m_boundVertexBuffer = NULL;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();
}

void GraphicsDevice::BindIndexBuffer(IndexBuffer *buffer)
{
	ASSERT(buffer != NULL);
	ASSERT(buffer->GetNumElements() > 0);

	// don't bind this buffer if it's already bound!
	if (m_boundIndexBuffer == buffer)
		return;

	if (!buffer->IsClientSideBuffer())
		BindIBO(buffer);
	else
		BindClientBuffer(buffer);

	m_boundIndexBuffer = buffer;
}

void GraphicsDevice::UnbindIndexBuffer()
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	m_boundIndexBuffer = NULL;
}

void GraphicsDevice::BindShader(Shader *shader)
{
	ASSERT(shader != NULL);
	ASSERT(shader->IsReadyForUse() == true);
	GL_CALL(glUseProgram(shader->GetProgramId()));
	
	m_boundShader = shader;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();

	m_boundShader->OnBind();
}

void GraphicsDevice::UnbindShader()
{
	GL_CALL(glUseProgram(0));

	if (m_boundShader != NULL)
		m_boundShader->OnUnbind();

	m_boundShader = NULL;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();
}

void GraphicsDevice::BindVBO(VertexBuffer *buffer)
{
	if (buffer->IsDirty())
		buffer->Update();

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer->GetBufferId()));
}

void GraphicsDevice::BindClientBuffer(VertexBuffer *buffer)
{
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GraphicsDevice::BindIBO(IndexBuffer *buffer)
{
	if (buffer->IsDirty())
		buffer->Update();

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->GetBufferId()));
}

void GraphicsDevice::BindClientBuffer(IndexBuffer *buffer)
{
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

void GraphicsDevice::SetShaderVertexAttributes()
{
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundShader != NULL);
	ASSERT(m_enabledVertexAttribIndices.empty() == true);
	ASSERT(m_boundVertexBuffer->GetNumAttributes() >= m_boundShader->GetNumAttributes());

	uint numAttributes = m_boundShader->GetNumAttributes();
	for (uint i = 0; i < numAttributes; ++i)
	{
		int bufferAttribIndex = 0;
		if (m_boundShader->IsAttributeMappedToStandardType(i))
		{
			VERTEX_STANDARD_ATTRIBS standardType = m_boundShader->GetAttributeMappedStandardType(i);
			bufferAttribIndex = m_boundVertexBuffer->GetIndexOfStandardAttrib(standardType);
			ASSERT(bufferAttribIndex != -1);
			if (bufferAttribIndex == -1)
				continue;
		}
		else
			bufferAttribIndex = m_boundShader->GetAttributeMappedBufferIndex(i);

		uint offset = 0;
		GLint size = 0;

		const VertexBufferAttribute *bufferAttribInfo = m_boundVertexBuffer->GetAttributeInfo((uint)bufferAttribIndex);
		size = bufferAttribInfo->size;
		offset = bufferAttribInfo->offset;
		ASSERT(size != 0);

		// convert the offset into a pointer
		// client-side vertex data has a full pointer to the first element of the attribute data
		// VBO just specifies an offset in bytes from zero to the first element of the attribute data
		const void *buffer = NULL;
		if (m_boundVertexBuffer->IsClientSideBuffer())
			buffer = (float*)m_boundVertexBuffer->GetBuffer() + offset;
		else
			buffer = (int8_t*)NULL + (offset * sizeof(float));

		GL_CALL(glEnableVertexAttribArray(i));
		GL_CALL(glVertexAttribPointer(i, size, GL_FLOAT, false, m_boundVertexBuffer->GetElementWidthInBytes(), buffer));

		m_enabledVertexAttribIndices.push_back(i);
	}

	m_shaderVertexAttribsSet = true;
}

void GraphicsDevice::ClearSetShaderVertexAttributes()
{
	while (!m_enabledVertexAttribIndices.empty())
	{
		uint index = m_enabledVertexAttribIndices.back();
		m_enabledVertexAttribIndices.pop_back();
		GL_CALL(glDisableVertexAttribArray(index));
	}

	m_shaderVertexAttribsSet = false;
}

void GraphicsDevice::RenderTriangles(const IndexBuffer *buffer)
{
	ASSERT(buffer != NULL);
	ASSERT(buffer->IsClientSideBuffer() == true);
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundIndexBuffer == NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint numVertices = buffer->GetNumElements();
	ASSERT(numVertices % 3 == 0);
	GL_CALL(glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_SHORT, buffer->GetBuffer()));
}

void GraphicsDevice::RenderTriangles()
{
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		int numIndices = m_boundIndexBuffer->GetNumElements();
		ASSERT(numIndices % 3 == 0);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = m_boundIndexBuffer->GetBuffer();
		else
			offset = NULL;

		GL_CALL(glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the whole vertex buffer
		ASSERT(m_boundVertexBuffer->GetNumElements() % 3 == 0);
		GL_CALL(glDrawArrays(GL_TRIANGLES, 0, m_boundVertexBuffer->GetNumElements()));
	}
}

void GraphicsDevice::RenderTriangles(uint startVertex, uint numTriangles)
{
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint numVertices = numTriangles * 3;

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		ASSERT((m_boundIndexBuffer->GetNumElements() - startVertex) >= numVertices);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		uint indexOffset = startVertex * m_boundIndexBuffer->GetElementWidthInBytes();
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = ((int8_t*)m_boundIndexBuffer->GetBuffer() + indexOffset);
		else
			offset = ((int8_t*)NULL + indexOffset);

		GL_CALL(glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the given range using only the bound vertex buffer
		ASSERT((m_boundVertexBuffer->GetNumElements() - startVertex) >= numVertices);
		GL_CALL(glDrawArrays(GL_TRIANGLES, startVertex, numVertices));
	}
}

void GraphicsDevice::RenderLines(const IndexBuffer *buffer)
{
	ASSERT(buffer != NULL);
	ASSERT(buffer->IsClientSideBuffer() == true);
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundIndexBuffer == NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint numVertices = buffer->GetNumElements();
	ASSERT(numVertices % 2 == 0);
	GL_CALL(glDrawElements(GL_LINES, numVertices, GL_UNSIGNED_SHORT, buffer->GetBuffer()));
}

void GraphicsDevice::RenderLines()
{
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		uint numIndices = m_boundIndexBuffer->GetNumElements();
		ASSERT(numIndices % 2 == 0);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = m_boundIndexBuffer->GetBuffer();
		else
			offset = NULL;

		GL_CALL(glDrawElements(GL_LINES, numIndices, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the whole vertex buffer
		ASSERT(m_boundVertexBuffer->GetNumElements() % 2 == 0);
		GL_CALL(glDrawArrays(GL_LINES, 0, m_boundVertexBuffer->GetNumElements()));
	}
}

void GraphicsDevice::RenderLines(uint startVertex, uint numLines)
{
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint numVertices = numLines * 2;

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		ASSERT((m_boundIndexBuffer->GetNumElements() - startVertex) >= numVertices);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		uint indexOffset = startVertex * m_boundIndexBuffer->GetElementWidthInBytes();
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = ((int8_t*)m_boundIndexBuffer->GetBuffer() + indexOffset);
		else
			offset = ((int8_t*)NULL + indexOffset);

		GL_CALL(glDrawElements(GL_LINES, numVertices, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the given range using only the bound vertex buffer
		ASSERT((m_boundVertexBuffer->GetNumElements() - startVertex) >= numVertices);
		GL_CALL(glDrawArrays(GL_LINES, startVertex, numVertices));
	}
}

void GraphicsDevice::RenderPoints(const IndexBuffer *buffer)
{
	ASSERT(buffer != NULL);
	ASSERT(buffer->IsClientSideBuffer() == true);
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundIndexBuffer == NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint numVertices = buffer->GetNumElements();
	GL_CALL(glDrawElements(GL_POINTS, numVertices, GL_UNSIGNED_SHORT, buffer->GetBuffer()));
}

void GraphicsDevice::RenderPoints()
{
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		uint numIndices = m_boundIndexBuffer->GetNumElements();

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = m_boundIndexBuffer->GetBuffer();
		else
			offset = NULL;

		GL_CALL(glDrawElements(GL_POINTS, numIndices, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the whole vertex buffer
		GL_CALL(glDrawArrays(GL_POINTS, 0, m_boundVertexBuffer->GetNumElements()));
	}
}

void GraphicsDevice::RenderPoints(uint startVertex, uint numPoints)
{
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		ASSERT((m_boundIndexBuffer->GetNumElements() - startVertex) >= numPoints);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		uint indexOffset = startVertex * m_boundIndexBuffer->GetElementWidthInBytes();
		const void *offset;
		if (m_boundIndexBuffer->IsClientSideBuffer())
			offset = ((int8_t*)m_boundIndexBuffer->GetBuffer() + indexOffset);
		else
			offset = ((int8_t*)NULL + indexOffset);

		GL_CALL(glDrawElements(GL_POINTS, numPoints, GL_UNSIGNED_SHORT, offset));
	}
	else
	{
		// no index buffer, just render the given range using only the bound vertex buffer
		ASSERT((m_boundVertexBuffer->GetNumElements() - startVertex) >= numPoints);
		GL_CALL(glDrawArrays(GL_POINTS, startVertex, numPoints));
	}
}
