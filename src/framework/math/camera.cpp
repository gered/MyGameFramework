#include "../debug.h"

#include "camera.h"
#include "frustum.h"
#include "mathhelpers.h"
#include "point2.h"
#include "ray.h"
#include "rect.h"
#include "../graphics/viewcontext.h"

Camera::Camera(ViewContext *viewContext)
{
	STACK_TRACE;
	m_viewContext = viewContext;
	m_frustum = new Frustum(m_viewContext);
	ASSERT(m_frustum != NULL);
	m_position = ZERO_VECTOR;
	m_orientation = ZERO_VECTOR;
	m_forward = ZERO_VECTOR;
	m_up = UP;
	m_lookAt = IDENTITY_MATRIX;

	// fairly typical defaults
	m_fovAngle = 60.0f;
	m_near = 1.0f;
	m_far = 50.0f;

	CalculateDefaultProjection(
							   m_viewContext->GetViewportLeft(), 
							   m_viewContext->GetViewportTop(), 
							   m_viewContext->GetViewportRight(), 
							   m_viewContext->GetViewportBottom()
							   );
}

Camera::~Camera()
{
	STACK_TRACE;
	SAFE_DELETE(m_frustum);
}

void Camera::CalculateDefaultProjection(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom)
{
	STACK_TRACE;
	m_viewportWidth = right - left;
	m_viewportHeight = bottom - top;

	m_aspectRatio = (float)m_viewportWidth / (float)m_viewportHeight;

	m_nearHeight = m_near * tanf(DegreesToRadians(m_fovAngle / 2.0f));
	m_nearWidth = m_nearHeight * m_aspectRatio;

	m_projection = Matrix4x4::CreatePerspectiveFieldOfView(DegreesToRadians(m_fovAngle), m_aspectRatio, m_near, m_far);
}

void Camera::CalculateDefaultLookAt(const Vector3 &movement)
{
	STACK_TRACE;
	// final camera orientation. angles must be negative (or rather, inverted) for the camera matrix. also the matrix concatenation order is important!
	Matrix4x4 rotation = Matrix4x4::CreateRotationY(-m_orientation.y) * Matrix4x4::CreateRotationX(-m_orientation.x);

	// apply orientation to forward, movement and up vectors so they're pointing in the right direction
	m_forward = FORWARD * rotation;
	m_up = UP * rotation;
	Vector3 orientedMovement = movement * rotation;

	// move the camera position
	m_position += orientedMovement;

	Vector3 target = m_forward + m_position;
	m_lookAt = Matrix4x4::CreateLookAt(m_position, target, UP);
}

void Camera::OnUpdate(float delta)
{
	STACK_TRACE;
}

void Camera::OnRender()
{
	STACK_TRACE;
	UpdateLookAtMatrix(ZERO_VECTOR);
	m_viewContext->SetModelViewMatrix(m_lookAt);
	m_frustum->Calculate();
}

void Camera::OnResize(const Rect &size)
{
	STACK_TRACE;
	CalculateDefaultProjection(size.left, size.top, size.right, size.bottom);
	m_viewContext->SetProjectionMatrix(m_projection);
}

void Camera::UpdateLookAtMatrix(const Vector3 &movement)
{
	STACK_TRACE;
	CalculateDefaultLookAt(movement);
}

void Camera::UpdateProjectionMatrix()
{
	STACK_TRACE;
	CalculateDefaultProjection(
							   m_viewContext->GetViewportLeft(), 
							   m_viewContext->GetViewportTop(), 
							   m_viewContext->GetViewportRight(), 
							   m_viewContext->GetViewportBottom()
							   );
}

Ray Camera::Pick(uint16_t screenX, uint16_t screenY) const
{
	STACK_TRACE;
	float nx = 2.0f * ((float)(screenX - (m_viewContext->GetViewportWidth() / 2))) / ((float)m_viewContext->GetViewportWidth());
	float ny = 2.0f * -((float)(screenY - (m_viewContext->GetViewportHeight() / 2))) / ((float)m_viewContext->GetViewportHeight());

	// pick ray calculation method copied from http://code.google.com/p/libgdx/
	Vector3 vz = Vector3::Normalize(m_forward * -1.0f);
	Vector3 vx = Vector3::Normalize(Vector3::Cross(UP, vz));
	Vector3 vy = Vector3::Normalize(Vector3::Cross(vz, vx));

	Vector3 near_center = m_position - (vz * m_near);

	Vector3 a = (vx * m_nearWidth) * nx;
	Vector3 b = (vy * m_nearHeight) * ny;
	Vector3 near_point = a + b + near_center;

	Vector3 dir = Vector3::Normalize(near_point - m_position);

	return Ray(m_position, dir);
}

Point2 Camera::Project(const Vector3 &objectPosition) const
{
	STACK_TRACE;
	Matrix4x4 modelview = m_viewContext->GetModelViewMatrix();
	Matrix4x4 projection = m_viewContext->GetProjectionMatrix();

	return Project(objectPosition, modelview, projection);
}

Point2 Camera::Project(const Vector3 &objectPosition, const Matrix4x4 &modelview, const Matrix4x4 &projection) const
{
	STACK_TRACE;
	// transform object position by modelview matrix (vector transform, w = 1)
	float tempX = objectPosition.x * modelview.m[_11] + objectPosition.y * modelview.m[_12] + objectPosition.z * modelview.m[_13] + modelview.m[_14];
	float tempY = objectPosition.x * modelview.m[_21] + objectPosition.y * modelview.m[_22] + objectPosition.z * modelview.m[_23] + modelview.m[_24];
	float tempZ = objectPosition.x * modelview.m[_31] + objectPosition.y * modelview.m[_32] + objectPosition.z * modelview.m[_33] + modelview.m[_34];
	float tempW = objectPosition.x * modelview.m[_41] + objectPosition.y * modelview.m[_42] + objectPosition.z * modelview.m[_43] + modelview.m[_44];

	// transform the above by the projection matrix (optimized for bottom row of the projection matrix always being [0, 0, -1, 0])
	float transformedX = tempX * projection.m[_11] + tempY * projection.m[_12] + tempZ * projection.m[_13] + tempW * projection.m[_14];
	float transformedY = tempX * projection.m[_21] + tempY * projection.m[_22] + tempZ * projection.m[_23] + tempW * projection.m[_24];
	float transformedZ = tempX * projection.m[_31] + tempY * projection.m[_32] + tempZ * projection.m[_33] + tempW * projection.m[_34];
	float transformedW = -tempZ;

	// w normalizes between -1 and 1
	// TODO: shouldn't really handle this using an assert... however, I'd like to know when/if this happens
	ASSERT(transformedW != 0.0f);
	transformedW = 1.0f / transformedW;

	// perspective division
	transformedX *= transformedW;
	transformedY *= transformedW;
	transformedZ *= transformedW;

	// map to 2D viewport coordinates (ignoring Z)
	Point2 out;
	out.x = (int32_t)(((transformedX * 0.5f) + 0.5f) * (float)m_viewContext->GetViewportWidth() + (float)m_viewContext->GetViewportLeft());
	out.y = (int32_t)(((transformedY * 0.5f) + 0.5f) * (float)m_viewContext->GetViewportHeight() + (float)m_viewContext->GetViewportTop());
	// float z = (1.0f + transformedZ) * 0.5f;   // would be between 0.0f and 1.0f

	// adjust Y coordinate so that 0 is at the top of the screen instead of the bottom
	out.y = (int32_t)m_viewContext->GetViewportHeight() - out.y;

	return out; 
}

