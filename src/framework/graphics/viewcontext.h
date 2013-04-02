#ifndef __FRAMEWORK_GRAPHICS_VIEWCONTEXT_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_VIEWCONTEXT_H_INCLUDED__

#include "../common.h"
#include "../math/matrix4x4.h"
#include "../math/matrixstack.h"
#include "../math/rect.h"
#include "../screenorientation.h"

class Camera;
class GraphicsDevice;

class ViewContext
{
public:
	ViewContext();
	virtual ~ViewContext();
	
	BOOL Create(GraphicsDevice *graphicsDevice);
	BOOL Create(GraphicsDevice *graphicsDevice, const Rect &fixedViewportSize);
	void Release();
	
	void OnNewContext();
	void OnLostContext();
	void OnResize(const Rect &size, SCREEN_ORIENTATION_ANGLE screenOrientation = SCREEN_ANGLE_0);
	void OnRender();
	void OnApply(const Rect &size, SCREEN_ORIENTATION_ANGLE screenOrientation = SCREEN_ANGLE_0);

	Matrix4x4 GetProjectionMatrix() const                  { return m_projectionStack.top; }
	void SetProjectionMatrix(const Matrix4x4 &m);
	void PushProjectionMatrix();
	void PopProjectionMatrix();
	Matrix4x4 GetOrthographicProjectionMatrix();

	Matrix4x4 GetModelViewMatrix() const                   { return m_modelviewStack.top; }
	void SetModelViewMatrix(const Matrix4x4 &m);
	void PushModelViewMatrix();
	void PopModelViewMatrix();
	
	Camera* GetCamera() const                              { return m_camera; }
	void SetCamera(Camera *camera);
	
	uint GetViewportTop() const                            { return (uint)m_viewport.top; }
	uint GetViewportLeft() const                           { return (uint)m_viewport.left; }
	uint GetViewportBottom() const                         { return (uint)m_viewport.bottom; }
	uint GetViewportRight() const                          { return (uint)m_viewport.right; }
	uint GetViewportWidth() const                          { return (uint)m_viewport.GetWidth(); }
	uint GetViewportHeight() const                         { return (uint)m_viewport.GetHeight(); }

	BOOL IsViewportFixedSize() const                       { return m_viewportIsFixedSize; }
	BOOL IgnoringScreenRotation() const                    { return m_viewportIsFixedSize; }
	
private:
	void SetupViewport(const Rect &size, SCREEN_ORIENTATION_ANGLE screenOrientation);
	
	GraphicsDevice *m_graphicsDevice;
	
	Rect m_viewport;
	BOOL m_viewportIsFixedSize;
	SCREEN_ORIENTATION_ANGLE m_screenOrientation;
	
	Camera *m_camera;
	BOOL m_usingDefaultCamera;
	
	MatrixStack m_modelviewStack;
	MatrixStack m_projectionStack;
};

#endif
