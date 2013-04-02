#include "../debug.h"

#include "viewcontext.h"
#include "../common.h"
#include "../gamewindow.h"
#include "../screenorientation.h"
#include "../graphics/glincludes.h"
#include "../graphics/glutils.h"
#include "../graphics/graphicsdevice.h"
#include "../math/camera.h"
#include "../math/mathhelpers.h"
#include "../math/matrix4x4.h"
#include "../math/matrixstack.h"
#include "../math/rect.h"

ViewContext::ViewContext()
{
	m_graphicsDevice = NULL;
	m_viewport = Rect(0, 0, 0, 0);
	m_viewportIsFixedSize = FALSE;
	m_camera = NULL;
	m_usingDefaultCamera = FALSE;
}

ViewContext::~ViewContext()
{
	Release();
}

void ViewContext::Release()
{
	if (m_usingDefaultCamera)
		SAFE_DELETE(m_camera);
	
	m_graphicsDevice = NULL;
	m_viewport = Rect(0, 0, 0, 0);
	m_viewportIsFixedSize = FALSE;
	m_screenOrientation = SCREEN_ANGLE_0;
	m_camera = NULL;
	m_usingDefaultCamera = FALSE;
}

BOOL ViewContext::Create(GraphicsDevice *graphicsDevice)
{
	ASSERT(m_graphicsDevice == NULL);
	if (m_graphicsDevice != NULL)
		return FALSE;
	
	ASSERT(graphicsDevice != NULL);
	m_graphicsDevice = graphicsDevice;
	
	m_viewport = m_graphicsDevice->GetWindow()->GetRect();
	m_viewportIsFixedSize = FALSE;
	m_screenOrientation = SCREEN_ANGLE_0;

	m_camera = new Camera(this);
	m_usingDefaultCamera = TRUE;
	
	return TRUE;
}

BOOL ViewContext::Create(GraphicsDevice *graphicsDevice, const Rect &fixedViewportSize)
{
	ASSERT(m_graphicsDevice == NULL);
	if (m_graphicsDevice != NULL)
		return FALSE;

	ASSERT(graphicsDevice != NULL);
	m_graphicsDevice = graphicsDevice;
	
	m_viewport = fixedViewportSize;
	m_viewportIsFixedSize = TRUE;
	m_screenOrientation = SCREEN_ANGLE_0;
	
	m_camera = new Camera(this);
	m_usingDefaultCamera = TRUE;
	
	return TRUE;
}

void ViewContext::OnNewContext()
{
	m_modelviewStack.Clear();
	m_modelviewStack.top = IDENTITY_MATRIX;
	m_projectionStack.Clear();
	m_projectionStack.top = IDENTITY_MATRIX;
}

void ViewContext::OnLostContext()
{
}

void ViewContext::OnResize(const Rect &size, SCREEN_ORIENTATION_ANGLE screenOrientation)
{
	SetupViewport(size, screenOrientation);
}

void ViewContext::OnRender()
{
	if (m_camera != NULL)
		m_camera->OnRender();
}

void ViewContext::OnApply(const Rect &size, SCREEN_ORIENTATION_ANGLE screenOrientation)
{
	SetupViewport(size, screenOrientation);
	
	// ensures it's set up for rendering immediately when this call returns
	// NOTE: we assume OnApply() is going to be called in some other class's
	//       OnRender() event only (like, e.g. if a new framebuffer is bound)
	if (m_camera != NULL)
		m_camera->OnRender();
}

void ViewContext::SetProjectionMatrix(const Matrix4x4 &m)
{
	if (!IgnoringScreenRotation() && m_screenOrientation != SCREEN_ANGLE_0)
	{
		// apply a rotation immediately _after_ the projection matrix transform
		float angle = (float)m_screenOrientation;
		Matrix4x4 adjusted = Matrix4x4::CreateRotationZ(-DegreesToRadians(angle)) * m;
		m_projectionStack.top = adjusted;
	}
	else
		m_projectionStack.top = m;
}

void ViewContext::PushProjectionMatrix()
{
	m_projectionStack.Push();
	// with MatrixStack, pushing does not change the top matrix, so
	// we don't need to re-set the projection matrix with OpenGL
}

void ViewContext::PopProjectionMatrix()
{
	m_projectionStack.Pop();
}

Matrix4x4 ViewContext::GetOrthographicProjectionMatrix()
{
	Matrix4x4 ortho = Matrix4x4::CreateOrthographic((float)m_viewport.left, (float)m_viewport.right, (float)m_viewport.top, (float)m_viewport.bottom, 0.0f, 1.0f);
	
	if (!IgnoringScreenRotation() && m_screenOrientation != SCREEN_ANGLE_0)
	{
		// apply a rotation immediately _after_ the projection matrix transform
		float angle = (float)m_screenOrientation;
		Matrix4x4 adjusted = Matrix4x4::CreateRotationZ(-DegreesToRadians(angle)) * ortho;
		return adjusted;
	}
	else
		return ortho;
}

void ViewContext::SetModelViewMatrix(const Matrix4x4 &m)
{
	m_modelviewStack.top = m;
}

void ViewContext::PushModelViewMatrix()
{
	m_modelviewStack.Push();
	// with MatrixStack, pushing does not change the top matrix, so
	// we don't need to re-set the modelview matrix with OpenGL
}

void ViewContext::PopModelViewMatrix()
{
	m_modelviewStack.Pop();
}

void ViewContext::SetCamera(Camera *camera)
{
	// using the default camera but a new camera is being provided?
	if (m_usingDefaultCamera && camera != NULL)
	{
		SAFE_DELETE(m_camera);
		m_usingDefaultCamera = FALSE;
		m_camera = camera;
	}
	
	// not using the default camera already, but setting a new camera
	else if (!m_usingDefaultCamera && camera != NULL)
	{
		m_camera = camera;
	}
	
	// not using the default camera, and clearing ("nulling") the camera
	else if (!m_usingDefaultCamera && camera == NULL)
	{
		m_camera = new Camera(this);
		ASSERT(m_camera != NULL);
		m_usingDefaultCamera = TRUE;
	}
}

void ViewContext::SetupViewport(const Rect &size, SCREEN_ORIENTATION_ANGLE screenOrientation)
{
	Rect viewport;
	
	if (m_viewportIsFixedSize)
	{
		viewport = m_viewport;
		m_screenOrientation = SCREEN_ANGLE_0;
	}
	else
	{
		// based on the orientation, we may need to swap the width/height
		// of the passed viewport dimensions
		// (we don't do viewport rotation if the viewport is fixed)
		if (!IgnoringScreenRotation() && (screenOrientation == SCREEN_ANGLE_90 || screenOrientation == SCREEN_ANGLE_270))
		{
			// swap width and height
			viewport.left = size.top;
			viewport.top = size.left;
			viewport.right = size.bottom;
			viewport.bottom = size.right;
		}
		else
			viewport = size;
		
		// we **don't** want this to be rotated
		m_viewport = size;
		
		m_screenOrientation = screenOrientation;
	}
	
	// we **do** obviously want this to be rotated (if there is a rotation)
	GL_CALL(glViewport(viewport.left, viewport.top, viewport.GetWidth(), viewport.GetHeight()));
		
	// we also **don't** want the camera to work with a rotated viewport
	if (m_camera != NULL)
		m_camera->OnResize(m_viewport);
}
