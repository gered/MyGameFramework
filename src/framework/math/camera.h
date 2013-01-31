#ifndef __FRAMEWORK_MATH_CAMERA_H_INCLUDED__
#define __FRAMEWORK_MATH_CAMERA_H_INCLUDED__

#include "vector3.h"
#include "matrix4x4.h"
#include "../common.h"

class Frustum;
class ViewContext;
struct Ray;
struct Rect;
struct Point2;

/**
 * Base class for Camera objects.
 */
class Camera
{
public:
	/**
	 * Creates a camera object.
	 * @param viewContext the view context object this camera is associated with
	 */
	Camera(ViewContext *viewContext);

	virtual ~Camera();

	/**
	 * Update (tick) callback.
	 * @param delta time since last update
	 */
	virtual void OnUpdate(float delta);

	/**
	 * Render callback.
	 */
	virtual void OnRender();

	/**
	 * Viewport resize callback.
	 * @param size new size of the viewport
	 */
	virtual void OnResize(const Rect &size);
	
	/**
	 * Recalculates the projection matrix using the viewport dimensions
	 * currently set on the parent ViewContext. This is normally done inside
	 * the OnResize() callback for most camera implementations and won't
	 * need to be called manually.
	 */
	virtual void UpdateProjectionMatrix();
	
	/**
	 * Recalculates the "look at" matrix. This is normally done inside
	 * the OnRender() callback for most camera implementations and won't
	 * need to be called manually.
	 * @param movement amount to move the camera by. This will be oriented by
	 *                 the current camera orientation.
	 */
	virtual void UpdateLookAtMatrix(const Vector3 &movement = ZERO_VECTOR);

	/**
	 * Convert 2D screen coordinates to a 3D ray that originates from the
	 * 3D coordinates that coorespond to the 2D screen coordinates and has
	 * a direction that points "into" the 3D world from this point.
	 * @param screenX 2D X coordinate in screen space
	 * @param screenY 2D Y coordinate in screen space
	 * @return Ray a ray originating from the given 2D screen space coordinates
	 */
	Ray Pick(uint16_t screenX, uint16_t screenY) const;

	/**
	 * Projects the given 3D coordinates into 2D screen space coordinates.
	 * @param objectPosition 3D world position to be converted.
	 * @return Point2 "projected" 2D screen space coordinates corresponding
	 *                with the given 3D coordinates
	 */
	Point2 Project(const Vector3 &objectPosition) const;

	/**
	 * Projects the given 3D coordinates into 2D screen space coordinates.
	 * Instead of using the camera object's current modelview and projection
	 * matrices, this will use those matrices specified.
	 * @param objectPosition 3D world position to be converted. 
	 * @param modelview modelview matrix to use in the coordinate projection operation
	 * @param projection projection matrix to use in the coordinate projection operation
	 * @return Point2 "projected" 2D screen space coordinates corresponding
	 *                with the given 3D coordinates
	 */
	Point2 Project(const Vector3 &objectPosition, const Matrix4x4 &modelview, const Matrix4x4 &projection) const;

	/**
	 * @return the camera's frustum object
	 */
	const Frustum* GetFrustum() const                      { return m_frustum; }

	/**
	 * @return the camera's "look at" matrix
	 */
	const Matrix4x4& GetLookAt() const                     { return m_lookAt; }

	/**
	 * @return the camera's projection matrix
	 */
	const Matrix4x4& GetProjection() const                 { return m_projection; }

	/**
	 * @return a vector containing the camera's current X, Y, and Z axis 
	 *         orientations
	 */
	Vector3& GetOrientation()                              { return m_orientation; }

	/**
	 * @return a vector containing the camera's current X, Y, and Z axis 
	 *         orientations
	 */
	const Vector3& GetOrientation() const                  { return m_orientation; }

	/**
	 * @return the current position of the camera
	 */
	Vector3& GetPosition()                                 { return m_position; }

	/**
	 * @return the current position of the camera
	 */
	const Vector3& GetPosition() const                     { return m_position; }

	/**
	 * @return the direction the camera facing
	 */
	Vector3& GetForward()                                  { return m_forward; }

	/**
	 * @return the direction the camera facing
	 */
	const Vector3& GetForward() const                      { return m_forward; }

	/**
	 * @return the "up" vector of the camera which is adjusted by it's
	 *         current orientation
	 */
	Vector3& GetUp()                                       { return m_up; }

	/**
	 * @return the "up" vector of the camera which is adjusted by it's
	 *         current orientation
	 */
	const Vector3& GetUp() const                           { return m_up; }

	/**
	 * @return the width of the camera's current viewport
	 */
	uint16_t GetViewportWidth() const                      { return m_viewportWidth; }

	/**
	 * @return the height of the camera's current viewport
	 */
	uint16_t GetViewportHeight() const                     { return m_viewportHeight; }

	/**
	 * @return the aspect ratio of the camera's current viewport
	 */
	float GetAspectRatio() const                           { return m_aspectRatio; }

	/**
	 * @return the distance to the near clipping plane
	 */
	float GetNearDistance() const                          { return m_near; }

	/**
	 * @return the distance to the far clipping plane
	 */
	float GetFarDistance() const                           { return m_far; }

protected:
	/**
	 * Calculates a default projection matrix using the viewport dimensions.
	 * @param left left X coordinate of the viewport
	 * @param top top Y coordinate of the viewport
	 * @param right right X coordinate of the viewport
	 * @param bottom bottom Y coordinate of the viewport
	 */
	void CalculateDefaultProjection(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom);

	/**
	 * Calculates a default "look at" matrix to position and orient the camera.
	 * @param movement amount to move the camera by. This will be oriented by
	 *                 the current camera orientation.
	 */
	void CalculateDefaultLookAt(const Vector3 &movement);
	
private:
	float m_fovAngle;
	float m_aspectRatio;
	float m_near;
	float m_far;
	float m_nearHeight;
	float m_nearWidth;

	uint16_t m_viewportWidth;
	uint16_t m_viewportHeight;

	ViewContext *m_viewContext;
	Frustum *m_frustum;
	Matrix4x4 m_lookAt;
	Matrix4x4 m_projection;
	Vector3 m_orientation;
	Vector3 m_position;
	Vector3 m_forward;
	Vector3 m_up;
};

#endif
