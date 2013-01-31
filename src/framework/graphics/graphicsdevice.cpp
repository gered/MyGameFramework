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

GraphicsDevice::GraphicsDevice(GameWindow *window)
{
	STACK_TRACE;
	m_boundVertexBuffer = NULL;
	m_boundIndexBuffer = NULL;
	m_boundShader = NULL;
	m_shaderVertexAttribsSet = FALSE;
	
	m_boundTextures = new const Texture*[MAX_BOUND_TEXTURES];
	m_boundFramebuffer = NULL;
	m_boundRenderbuffer = NULL;

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

	m_activeViewContext = NULL;
	m_defaultViewContext = new ViewContext();
	m_defaultViewContext->Create(this);
	m_activeViewContext = m_defaultViewContext;

	m_currentTextureParams = TEXPARAM_DEFAULT;

	m_debugRenderer = NULL;

	m_solidColorTextures = new SolidColorTextureCache(this);

	m_simpleColorShader = new SimpleColorShader();
	m_simpleColorTextureShader = new SimpleColorTextureShader();
	m_simpleTextureShader = new SimpleTextureShader();
	m_simpleTextureVertexLerpShader = new SimpleTextureVertexLerpShader();
	m_simpleTextureVertexSkinningShader = new SimpleTextureVertexSkinningShader();
	m_sprite2dShader = new Sprite2DShader();
	m_sprite3dShader = new Sprite3DShader();
	m_debugShader = new DebugShader();

	RegisterManagedResource(m_simpleColorShader);
	RegisterManagedResource(m_simpleColorTextureShader);
	RegisterManagedResource(m_simpleTextureShader);
	RegisterManagedResource(m_simpleTextureVertexLerpShader);
	RegisterManagedResource(m_simpleTextureVertexSkinningShader);
	RegisterManagedResource(m_sprite2dShader);
	RegisterManagedResource(m_sprite3dShader);
	RegisterManagedResource(m_debugShader);
}

GraphicsDevice::~GraphicsDevice()
{
	STACK_TRACE;
	UnregisterManagedResource(m_simpleColorShader);
	UnregisterManagedResource(m_simpleColorTextureShader);
	UnregisterManagedResource(m_simpleTextureShader);
	UnregisterManagedResource(m_simpleTextureVertexLerpShader);
	UnregisterManagedResource(m_simpleTextureVertexSkinningShader);
	UnregisterManagedResource(m_sprite2dShader);
	UnregisterManagedResource(m_sprite3dShader);
	UnregisterManagedResource(m_debugShader);

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
}

void GraphicsDevice::OnNewContext()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_GRAPHICS, "Initializing default state for new OpenGL context.\n");

	m_activeViewContext->OnNewContext();

	RENDERSTATE_DEFAULT.Apply();
	BLENDSTATE_DEFAULT.Apply();

	UnbindVertexBuffer();
	UnbindIndexBuffer();
	for (uint32_t i = 0; i < MAX_BOUND_TEXTURES; ++i)
		UnbindTexture(i);
	UnbindShader();
	UnbindRenderbuffer();
	UnbindFramebuffer();

	m_debugRenderer = new GeometryDebugRenderer(this);

	m_solidColorTextures->OnNewContext();

	for (ManagedResourceList::iterator i = m_managedResources.begin(); i != m_managedResources.end(); ++i)
		(*i)->OnNewContext();
}

void GraphicsDevice::OnLostContext()
{
	STACK_TRACE;
	LOG_INFO(LOGCAT_GRAPHICS, "Cleaning up objects/state specific to the lost OpenGL context.\n");
	
	m_activeViewContext->OnLostContext();

	SAFE_DELETE(m_debugRenderer);

	m_solidColorTextures->OnLostContext();

	for (ManagedResourceList::iterator i = m_managedResources.begin(); i != m_managedResources.end(); ++i)
		(*i)->OnLostContext();
}

void GraphicsDevice::OnResize(const Rect &size)
{
	STACK_TRACE;
	
	LOG_INFO(LOGCAT_GRAPHICS, "Window resized (%d, %d) - (%d, %d).\n", size.left, size.top, size.GetWidth(), size.GetHeight());
	if (m_window->GetScreenOrientation() != SCREEN_ANGLE_0)
		LOG_INFO(LOGCAT_GRAPHICS, "Screen is rotated (angle = %d).\n", (int)m_window->GetScreenOrientation());
	m_activeViewContext->OnResize(size, m_window->GetScreenOrientation());
}

void GraphicsDevice::OnRender()
{
	STACK_TRACE;
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
	STACK_TRACE;
	GL_CALL(glClearColor(r, g, b, a));
	GL_CALL(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void GraphicsDevice::Clear(const Color &color)
{
	STACK_TRACE;
	Clear(color.r, color.g, color.b, color.a);
}

Texture* GraphicsDevice::GetSolidColorTexture(const Color &color)
{
	STACK_TRACE;
	return m_solidColorTextures->Get(color);
}

void GraphicsDevice::BindTexture(const Texture *texture, uint32_t unit)
{
	STACK_TRACE;
	ASSERT(unit < MAX_BOUND_TEXTURES);
	ASSERT(texture != NULL);
	ASSERT(texture->IsInvalidated() == FALSE);
	if (texture != m_boundTextures[unit])
	{
		GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
		GL_CALL(glBindTexture(GL_TEXTURE_2D, texture->GetTextureName()));
	}
	m_boundTextures[unit] = texture;
}

void GraphicsDevice::BindSolidColorTexture(const Color &color, uint32_t unit)
{
	STACK_TRACE;
	Texture *texture = m_solidColorTextures->Get(color);
	BindTexture(texture, unit);
}

void GraphicsDevice::UnbindTexture(uint32_t unit)
{
	STACK_TRACE;
	ASSERT(unit < MAX_BOUND_TEXTURES);
	GL_CALL(glActiveTexture(GL_TEXTURE0 + unit));
	GL_CALL(glBindTexture(GL_TEXTURE_2D, 0));
	m_boundTextures[unit] = NULL;
}

void GraphicsDevice::UnbindTexture(const Texture *texture)
{
	STACK_TRACE;
	ASSERT(texture != NULL);
	if (texture == NULL)
		return;
	
	for (uint32_t i = 0; i < MAX_BOUND_TEXTURES; ++i)
	{
		if (m_boundTextures[i] == texture)
			UnbindTexture(i);
	}
}

void GraphicsDevice::BindRenderbuffer(Renderbuffer *renderbuffer)
{
	STACK_TRACE;
	ASSERT(renderbuffer != NULL);
	ASSERT(renderbuffer->IsInvalidated() == FALSE);
	if (m_boundRenderbuffer != renderbuffer)
	{
		GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, renderbuffer->GetRenderbufferName()));
		m_boundRenderbuffer = renderbuffer;
	}
}

void GraphicsDevice::UnbindRenderbuffer()
{
	STACK_TRACE;
	GL_CALL(glBindRenderbuffer(GL_RENDERBUFFER, 0));
	m_boundRenderbuffer = NULL;
}

void GraphicsDevice::UnbindRenderBuffer(Renderbuffer *renderBuffer)
{
	STACK_TRACE;
	ASSERT(renderBuffer != NULL);
	if (renderBuffer == NULL)
		return;
	
	if (m_boundRenderbuffer == renderBuffer)
		UnbindRenderbuffer();
}

void GraphicsDevice::BindFramebuffer(Framebuffer *framebuffer)
{
	STACK_TRACE;
	ASSERT(framebuffer != NULL);
	ASSERT(framebuffer->IsInvalidated() == FALSE);
	if (m_boundFramebuffer != framebuffer)
	{
		GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, framebuffer->GetFramebufferName()));
		m_boundFramebuffer = framebuffer;
		m_boundFramebuffer->OnBind();
	}
}

void GraphicsDevice::UnbindFramebuffer()
{
	STACK_TRACE;
	GL_CALL(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	if (m_boundFramebuffer != NULL)
		m_boundFramebuffer->OnUnBind();
	m_boundFramebuffer = NULL;
}

void GraphicsDevice::UnbindFramebuffer(Framebuffer *framebuffer)
{
	STACK_TRACE;
	ASSERT(framebuffer != NULL);
	if (framebuffer == NULL)
		return;
	
	if (m_boundFramebuffer == framebuffer)
		UnbindFramebuffer();
}

void GraphicsDevice::SetViewContext(ViewContext *viewContext)
{
	STACK_TRACE;
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
	STACK_TRACE;
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
	STACK_TRACE;
	ASSERT(resource != NULL);
	m_managedResources.remove(resource);
}

void GraphicsDevice::UnregisterAllManagedResources()
{
	STACK_TRACE;
	m_managedResources.clear();
}

void GraphicsDevice::BindVertexBuffer(VertexBuffer *buffer)
{
	STACK_TRACE;
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
	STACK_TRACE;
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

	m_boundVertexBuffer = NULL;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();
}

void GraphicsDevice::BindIndexBuffer(IndexBuffer *buffer)
{
	STACK_TRACE;
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
	STACK_TRACE;
	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

	m_boundIndexBuffer = NULL;
}

void GraphicsDevice::BindShader(Shader *shader)
{
	STACK_TRACE;
	ASSERT(shader != NULL);
	ASSERT(shader->IsReadyForUse() == TRUE);
	GL_CALL(glUseProgram(shader->GetProgramId()));
	
	m_boundShader = shader;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();

	m_boundShader->OnBind();
}

void GraphicsDevice::UnbindShader()
{
	STACK_TRACE;
	GL_CALL(glUseProgram(0));

	if (m_boundShader != NULL)
		m_boundShader->OnUnbind();

	m_boundShader = NULL;
	if (m_shaderVertexAttribsSet)
		ClearSetShaderVertexAttributes();
}

void GraphicsDevice::BindVBO(VertexBuffer *buffer)
{
	STACK_TRACE;
	if (buffer->IsDirty())
		buffer->Update();

	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, buffer->GetBufferId()));
}

void GraphicsDevice::BindClientBuffer(VertexBuffer *buffer)
{
	STACK_TRACE;
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GraphicsDevice::BindIBO(IndexBuffer *buffer)
{
	STACK_TRACE;
	if (buffer->IsDirty())
		buffer->Update();

	GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer->GetBufferId()));
}

void GraphicsDevice::BindClientBuffer(IndexBuffer *buffer)
{
	STACK_TRACE;
	GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

void GraphicsDevice::SetShaderVertexAttributes()
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundShader != NULL);
	ASSERT(m_enabledVertexAttribIndices.empty() == TRUE);
	ASSERT(m_boundVertexBuffer->GetNumAttributes() >= m_boundShader->GetNumAttributes());

	uint32_t numAttributes = m_boundShader->GetNumAttributes();
	for (uint32_t i = 0; i < numAttributes; ++i)
	{
		int32_t bufferAttribIndex = 0;
		if (m_boundShader->IsAttributeMappedToStandardType(i))
		{
			VERTEX_ATTRIBS standardType = m_boundShader->GetAttributeMappedStandardType(i);
			bufferAttribIndex = m_boundVertexBuffer->GetIndexOfStandardAttrib(standardType);
			ASSERT(bufferAttribIndex != -1);
			if (bufferAttribIndex == -1)
				continue;
		}
		else
			bufferAttribIndex = m_boundShader->GetAttributeMappedBufferIndex(i);

		uint32_t offset = 0;
		GLint size = 0;

		const VertexBufferAttribute *bufferAttribInfo = m_boundVertexBuffer->GetAttributeInfo((uint32_t)bufferAttribIndex);
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
		GL_CALL(glVertexAttribPointer(i, size, GL_FLOAT, FALSE, m_boundVertexBuffer->GetElementWidthInBytes(), buffer));

		m_enabledVertexAttribIndices.push_back(i);
	}

	m_shaderVertexAttribsSet = TRUE;
}

void GraphicsDevice::ClearSetShaderVertexAttributes()
{
	STACK_TRACE;
	while (!m_enabledVertexAttribIndices.empty())
	{
		uint32_t index = m_enabledVertexAttribIndices.back();
		m_enabledVertexAttribIndices.pop_back();
		GL_CALL(glDisableVertexAttribArray(index));
	}

	m_shaderVertexAttribsSet = FALSE;
}

void GraphicsDevice::RenderTriangles(const IndexBuffer *buffer)
{
	STACK_TRACE;
	ASSERT(buffer != NULL);
	ASSERT(buffer->IsClientSideBuffer() == TRUE);
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundIndexBuffer == NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	int numVertices = buffer->GetNumElements();
	ASSERT(numVertices % 3 == 0);
	GL_CALL(glDrawElements(GL_TRIANGLES, numVertices, GL_UNSIGNED_SHORT, buffer->GetBuffer()));
}

void GraphicsDevice::RenderTriangles()
{
	STACK_TRACE;
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

void GraphicsDevice::RenderTriangles(uint32_t startVertex, uint32_t numTriangles)
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint32_t numVertices = numTriangles * 3;

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		ASSERT((m_boundIndexBuffer->GetNumElements() - startVertex) >= numVertices);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		uint32_t indexOffset = startVertex * m_boundIndexBuffer->GetElementWidthInBytes();
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
	STACK_TRACE;
	ASSERT(buffer != NULL);
	ASSERT(buffer->IsClientSideBuffer() == TRUE);
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundIndexBuffer == NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint32_t numVertices = buffer->GetNumElements();
	ASSERT(numVertices % 2 == 0);
	GL_CALL(glDrawElements(GL_LINES, numVertices, GL_UNSIGNED_SHORT, buffer->GetBuffer()));
}

void GraphicsDevice::RenderLines()
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		int numIndices = m_boundIndexBuffer->GetNumElements();
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

void GraphicsDevice::RenderLines(uint32_t startVertex, uint32_t numLines)
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint32_t numVertices = numLines * 2;

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		ASSERT((m_boundIndexBuffer->GetNumElements() - startVertex) >= numVertices);

		// get the offset from the beginning of the index buffer to start rendering at
		// client buffer is just a raw pointer to the first index to use
		// IBO is basically the same idea, but the pointer "starts" at NULL and goes from there
		uint32_t indexOffset = startVertex * m_boundIndexBuffer->GetElementWidthInBytes();
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
	STACK_TRACE;
	ASSERT(buffer != NULL);
	ASSERT(buffer->IsClientSideBuffer() == TRUE);
	ASSERT(m_boundVertexBuffer != NULL);
	ASSERT(m_boundIndexBuffer == NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();
	uint32_t numVertices = buffer->GetNumElements();
	GL_CALL(glDrawElements(GL_POINTS, numVertices, GL_UNSIGNED_SHORT, buffer->GetBuffer()));
}

void GraphicsDevice::RenderPoints()
{
	STACK_TRACE;
	ASSERT(m_boundVertexBuffer != NULL);
	if (!m_shaderVertexAttribsSet)
		SetShaderVertexAttributes();

	if (m_boundIndexBuffer != NULL)
	{
		// using bound index buffer
		int numIndices = m_boundIndexBuffer->GetNumElements();

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

void GraphicsDevice::RenderPoints(uint32_t startVertex, uint32_t numPoints)
{
	STACK_TRACE;
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
		uint32_t indexOffset = startVertex * m_boundIndexBuffer->GetElementWidthInBytes();
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
