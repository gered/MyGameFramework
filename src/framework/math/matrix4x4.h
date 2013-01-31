#ifndef __FRAMEWORK_MATH_MATRIX4X4_H_INCLUDED__
#define __FRAMEWORK_MATH_MATRIX4X4_H_INCLUDED__

#include "../common.h"
#include <math.h>
#include "common.h"
#include "mathhelpers.h"
#include "vector3.h"
#include "vector4.h"

enum MATRIX_ELEMENTS
{
	_11 = 0,
	_12 = 4,
	_13 = 8,
	_14 = 12,
	_21 = 1,
	_22 = 5,
	_23 = 9,
	_24 = 13,
	_31 = 2,
	_32 = 6,
	_33 = 10,
	_34 = 14,
	_41 = 3,
	_42 = 7,
	_43 = 11,
	_44 = 15
};

/**
 * Represents a 4x4 column-major Matrix and provides common methods for
 * matrix math.
 * <p>Referenced/based on code from:</p>
 * <ul>
 * <li>3D Math Primer for Graphics and Game Development (Dunn & Parberry, 2002)</li>
 * <li>http://www.dhpoware.com/source/mathlib.html</li>
 * <li>http://www.songho.ca/opengl/gl_transform.html</li>
 * <li>http://www.opengl.org/sdk/docs/man/</li>
 * </ul>
 */
struct Matrix4x4
{
	/**
	 * Creates an uninitialized matrix.
	 */
	Matrix4x4();

	/**
	 * Creates a matrix with the specified elements.
	 */
	Matrix4x4(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
		);

	/**
	 * Creates a matrix with the specified elements.
	 * @param mv an array of 16 values that will be used as this matrix's
	 *           elements
	 */
	Matrix4x4(const float *mv);

	/**
	 * Sets new element values for this matrix.
	 */
	void Set(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44
		);

	/**
	 * @return the determinant of this matrix
	 */
	float GetDeterminant() const;

	/**
	 * @return the forward vector (z-axis)
	 */
	Vector3 GetForward() const;

	/**
	 * @return the backward vector (inverse z-axis)
	 */
	Vector3 GetBackward() const;

	/**
	 * @return the left vector (x-axis)
	 */
	Vector3 GetLeft() const;

	/**
	 * @return the right vector (inverse x-axis)
	 */
	Vector3 GetRight() const;

	/**
	 * @return the up vector (y-axis)
	 */
	Vector3 GetUp() const;

	/**
	 * @return the down vector (inverse y-axis)
	 */
	Vector3 GetDown() const;

	/**
	 * @return the translation vector
	 */
	Vector3 GetTranslation() const;
	
	/**
	 * @return the values in the first column of the matrix
	 */
	Vector4 GetColumn1() const;
	
	/**
	 * @return the values in the second column of the matrix
	 */
	Vector4 GetColumn2() const;
	
	/**
	 * @return the values in the third column of the matrix
	 */
	Vector4 GetColumn3() const;
	
	/**
	 * @return the values in the fourth column of the matrix
	 */
	Vector4 GetColumn4() const;
	
	/**
	 * @return the values in the first row of the matrix
	 */
	Vector4 GetRow1() const;
	
	/**
	 * @return the values in the first row of the matrix
	 */
	Vector4 GetRow2() const;
	
	/**
	 * @return the values in the first row of the matrix
	 */
	Vector4 GetRow3() const;
	
	/**
	 * @return the values in the first row of the matrix
	 */
	Vector4 GetRow4() const;

	/** 
	 * @return identity matrix
	 */
	static Matrix4x4 Identity();

	/**
	 * Constructs a point billboard (spherical billboard) transformation matrix.
	 * @param objectPosition the position of the billboard object
	 * @param cameraPosition the position of the camera viewing the billboard
	 * @param cameraUp the up vector of the camera
	 * @param cameraForward the forward vector of the camera
	 * @return a billboard transformation matrix
	 */
	static Matrix4x4 CreateBillboard(const Vector3 &objectPosition, const Vector3 &cameraPosition, const Vector3 &cameraUp, const Vector3 &cameraForward);

	/**
	 * Creates a transformation matrix for a billboard that is constrained to 
	 * rotate about an arbitrary axis.
	 * @param objectPosition the position of the billboard object
	 * @param cameraPosition the position of the camera viewing the billboard
	 * @param cameraForward the forward vector of the camera
	 * @param axis the axis that the billboard is to rotate about
	 * @return a billboard transformation matrix
	 */
	static Matrix4x4 CreateCylindricalBillboard(const Vector3 &objectPosition, const Vector3 &cameraPosition, const Vector3 &cameraForward, const Vector3 &axis);

	/**
	 * Constructs a "screen-aligned" billboard transformation matrix.
	 * @param objectPosition the position of the billboard object
	 * @param cameraUp the up vector of the camera
	 * @param cameraForward the forward vector of the camera
	 * @return a billboard transformation matrix
	 */
	static Matrix4x4 CreateScreenAlignedBillboard(const Vector3 &objectPosition, const Vector3 &cameraUp, const Vector3 &cameraForward);

	/**
	 * Constructs a "screen-aligned" billboard that is also then constrained
	 * to rotate about only a single axis (similar to a cylindrical 
	 * billboard).
	 * @param objectPosition the position of the billboard object
	 * @param cameraForward the forward vector of the camera
	 * @param axis the axis that the billboard is to rotate about
	 * @return a billboard transformation matrix
	 */
	static Matrix4x4 CreateScreenAndAxisAlignedBillboard(const Vector3 &objectPosition, const Vector3 &cameraForward, const Vector3 &axis);

	/**
	 * Constructs a rotation matrix from Euler angles.
	 * @param x the x angle (in radians)
	 * @param y the y angle (in radians)
	 * @param z the z angle (in radians)
	 * @return a rotation matrix representing the provided angles
	 */
	static Matrix4x4 CreateFromEulerAngles(float x, float y, float z);

	/**
	 * Constructs a modelview matrix. This constructs the same matrix as a call
	 * to gluLookAt.
	 * @param cameraPosition position of the camera (eye)
	 * @param cameraTarget the direction the camera is pointing (center)
	 * @param cameraUp the up direction relative to the camera
	 * @return a modelview matrix
	 */
	static Matrix4x4 CreateLookAt(const Vector3 &cameraPosition, const Vector3 &cameraTarget, const Vector3 &cameraUp);

	/**
	 * Creates an orthogonal projection matrix. This is equivalent to a matrix
	 * created by using OpenGL's glOrtho() function.
	 * @param left coordinate of the left vertical clipping plane
	 * @param right coordinate of the right vertical clipping plane
	 * @param bottom coordinate of the bottom horizontal clipping plane
	 * @param top coordinate of the top horizontal clipping plane
	 * @param near near clipping plane distance (negative value is behind the viewer)
	 * @param far far clipping plane distance (negative value is behind the viewer)
	 * @return an orthogonal projection matrix
	 */
	static Matrix4x4 CreateOrthographic(float left, float right, float bottom, float top, float near_, float far_);

	/**
	 * Constructs a perspective projection matrix. This is equivalent to a 
	 * matrix created by using OpenGL's glFrustum() function.
	 * @param left coordinate of the left vertical clipping plane
	 * @param right coordinate of the right vertical clipping plane
	 * @param bottom coordinate of the bottom horizontal clipping plane
	 * @param top coordinate of the top horizontal clipping plane
	 * @param near near clipping plane distance
	 * @param far far clipping plane distance
	 * @return a perspective projection matrix
	 */
	static Matrix4x4 CreatePerspective(float left, float right, float bottom, float top, float near_, float far_);

	/**
	 * Constructs a perspective projection matrix. This is equivalent to a 
	 * matrix created by using OpenGL's gluPerspective() function.
	 * @param fieldOfView angle specifying the field of view (in radians)
	 * @param aspectRatio the aspect ratio of the viewport
	 * @param near near clipping plane distance
	 * @param far far clipping plane distance
	 * @return a perspective projection matrix
	 */
	static Matrix4x4 CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio, float near_, float far_);

	/**
	 * Sets up a rotation matrix about the specified axis. This returns a matrix
	 * equivalent to the matrix that OpenGL multiples into the modelview matrix
	 * whenever glRotate(angle, axis.x, axis.y, axis.z) is called.
	 * @param angle the angle to rotate by (in radians)
	 * @param axis unit vector denoting the axis for the rotation
	 * @return a rotation matrix representing the specific axis rotation
	 */
	static Matrix4x4 CreateRotation(float angle, const Vector3 &axis);

	/**
	 * Constructs a rotation matrix for a rotation around the x axis. This returns
	 * a matrix equivalent to the matrix that OpenGL multiples into the modelview
	 * matrix whenever glRotate(angle, 1.0f, 0.0f, 0.0f) is called.
	 * @param angle angle to rotate the x axis around (in radians)
	 * @return the rotation matrix
	 */
	static Matrix4x4 CreateRotationX(float angle);

	/**
	 * Constructs a rotation matrix for a rotation around the y axis. This returns
	 * a matrix equivalent to the matrix that OpenGL multiples into the modelview
	 * matrix whenever glRotate(angle, 0.0f, 1.0f, 0.0f) is called.
	 * @param angle angle to rotate the y axis around (in radians)
	 * @return the rotation matrix
	 */
	static Matrix4x4 CreateRotationY(float angle);

	/**
	 * Constructs a rotation matrix for a rotation around the z axis. This returns
	 * a matrix equivalent to the matrix that OpenGL multiples into the modelview
	 * matrix whenever glRotate(angle, 0.0f, 0.0f, 1.0f) is called.
	 * @param angle angle to rotate the z axis around (in radians)
	 * @return the rotation matrix
	 */
	static Matrix4x4 CreateRotationZ(float angle);

	/**
	 * Constructs a scaling matrix from scaling factors for each axis. This returns
	 * a matrix equivalent to the matrix that OpenGL multiples into the modelview
	 * matrix whenever glScale(x, y, z) is called.
	 * @param x the scale factor for the x axis
	 * @param y the scale factor for the y axis
	 * @param z the scale factor for the z axis
	 * @return a scaling matrix
	 */
	static Matrix4x4 CreateScale(float x, float y, float z);

	/**
	 * Constructs a translation matrix. This returns a matrix equivalent to the
	 * matrix that OpenGL multiples into the modelview matrix whenever 
	 * glTranslate(x, y, z) is called.
	 * @param x the amount to translate on the x axis
	 * @param y the amount to translate on the y axis
	 * @param z the amount to translate on the z axis
	 * @return a translation matrix
	 */
	static Matrix4x4 CreateTranslation(float x, float y, float z);

	/**
	 * Constructs a world matrix.
	 * @param position the position of the object (translation)
	 * @param forward the forward facing direction of the object
	 * @param up the up direction of the object
	 * @return a world matrix
	 */
	static Matrix4x4 CreateWorld(const Vector3 &position, const Vector3 &forward, const Vector3 &up);

	/**
	 * Calculates the inverse of the specified matrix.
	 * @param m the matrix to calculate the inverse of
	 * @return the inverse of the specified matrix
	 */
	static Matrix4x4 Inverse(const Matrix4x4 &m);

	/**
	 * Calculates the transpose of a given matrix.
	 * @param m the matrix to get the transpose of
	 * @return the transpose matrix
	 */
	static Matrix4x4 Transpose(const Matrix4x4 &m);
	
	/**
	 * Transforms the given vertex by a matrix.
	 * @param m the transformation matrix to use
	 * @param v the vertex to be transformed
	 * @return the transformed vertex
	 */
	static Vector3 Transform(const Matrix4x4 &m, const Vector3 &v);
	
	/**
	 * Transforms the given vertex by a matrix.
	 * @param m the transformation matrix to use
	 * @param v the vertex to be transformed
	 * @return the transformed vertex
	 */
	static Vector4 Transform(const Matrix4x4 &m, const Vector4 &v);
	
	/**
	 * Transforms the given vertex by a matrix using only the components of the
	 * matrix describing rotations (translation information not used).
	 * @param m the transformation matrix to use
	 * @param v the vertex to be transformed (rotated)
	 * @return the transformed (rotated) vertex
	 */
	static Vector3 TransformUsingRotationOnly(const Matrix4x4 &m, const Vector3 &v);

	float m[16];
};

Matrix4x4 operator+(const Matrix4x4 &left, const Matrix4x4 &right);
Matrix4x4 &operator+=(Matrix4x4 &left, const Matrix4x4 &right);
Matrix4x4 operator-(const Matrix4x4 &left, const Matrix4x4 &right);
Matrix4x4 &operator-=(Matrix4x4 &left, const Matrix4x4 &right);
Matrix4x4 operator*(const Matrix4x4 &left, const Matrix4x4 &right);
Matrix4x4 &operator*=(Matrix4x4 &left, const Matrix4x4 &right);
Matrix4x4 operator*(const Matrix4x4 &left, float right);
Matrix4x4 &operator*=(Matrix4x4 &left, float right);
Vector3 operator*(const Vector3 &left, const Matrix4x4 &right);
Vector4 operator*(const Vector4 &left, const Matrix4x4 &right);

#define IDENTITY_MATRIX Matrix4x4::Identity()

inline Matrix4x4::Matrix4x4()
{
}

inline Matrix4x4::Matrix4x4(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44
	)
{
	m[_11] = m11;
	m[_12] = m12;
	m[_13] = m13;
	m[_14] = m14;
	m[_21] = m21;
	m[_22] = m22;
	m[_23] = m23;
	m[_24] = m24;
	m[_31] = m31;
	m[_32] = m32;
	m[_33] = m33;
	m[_34] = m34;
	m[_41] = m41;
	m[_42] = m42;
	m[_43] = m43;
	m[_44] = m44;
}

inline Matrix4x4::Matrix4x4(const float *mv)
{
	m[_11] = mv[_11];
	m[_12] = mv[_12];
	m[_13] = mv[_13];
	m[_14] = mv[_14];
	m[_21] = mv[_21];
	m[_22] = mv[_22];
	m[_23] = mv[_23];
	m[_24] = mv[_24];
	m[_31] = mv[_31];
	m[_32] = mv[_32];
	m[_33] = mv[_33];
	m[_34] = mv[_34];
	m[_41] = mv[_41];
	m[_42] = mv[_42];
	m[_43] = mv[_43];
	m[_44] = mv[_44];
}

inline void Matrix4x4::Set(
	float m11, float m12, float m13, float m14,
	float m21, float m22, float m23, float m24,
	float m31, float m32, float m33, float m34,
	float m41, float m42, float m43, float m44
	)
{
	m[_11] = m11;
	m[_12] = m12;
	m[_13] = m13;
	m[_14] = m14;
	m[_21] = m21;
	m[_22] = m22;
	m[_23] = m23;
	m[_24] = m24;
	m[_31] = m31;
	m[_32] = m32;
	m[_33] = m33;
	m[_34] = m34;
	m[_41] = m41;
	m[_42] = m42;
	m[_43] = m43;
	m[_44] = m44;
}

inline float Matrix4x4::GetDeterminant() const
{
	return
		(m[_11] * m[_22] - m[_21] * m[_12]) * 
		(m[_33] * m[_44] - m[_43] * m[_34]) - 
		(m[_11] * m[_32] - m[_31] * m[_12]) * 
		(m[_23] * m[_44] - m[_43] * m[_24]) + 
		(m[_11] * m[_42] - m[_41] * m[_12]) * 
		(m[_23] * m[_34] - m[_33] * m[_24]) + 
		(m[_21] * m[_32] - m[_31] * m[_22]) * 
		(m[_13] * m[_44] - m[_43] * m[_14]) - 
		(m[_21] * m[_42] - m[_41] * m[_22]) * 
		(m[_13] * m[_34] - m[_33] * m[_14]) + 
		(m[_31] * m[_42] - m[_41] * m[_32]) * 
		(m[_13] * m[_24] - m[_23] * m[_14]);
}

inline Vector3 Matrix4x4::GetForward() const
{
	return Vector3(m[_13], m[_23], m[_33]);
}

inline Vector3 Matrix4x4::GetBackward() const
{
	return Vector3(-m[_13], -m[_23], -m[_33]);
}

inline Vector3 Matrix4x4::GetLeft() const
{
	return Vector3(m[_11], m[_21], m[_31]);
}

inline Vector3 Matrix4x4::GetRight() const
{
	return Vector3(-m[_11], -m[_21], -m[_31]);
}

inline Vector3 Matrix4x4::GetUp() const
{
	return Vector3(m[_12], m[_22], m[_32]);
}

inline Vector3 Matrix4x4::GetDown() const
{
	return Vector3(-m[_12], -m[_22], -m[_32]);
}

inline Vector3 Matrix4x4::GetTranslation() const
{
	return Vector3(m[_14], m[_24], m[_34]);
}

inline Vector4 Matrix4x4::GetColumn1() const
{
	return Vector4(m[_11], m[_21], m[_31], m[_41]);
}

inline Vector4 Matrix4x4::GetColumn2() const
{
	return Vector4(m[_12], m[_22], m[_32], m[_42]);
}

inline Vector4 Matrix4x4::GetColumn3() const
{
	return Vector4(m[_13], m[_23], m[_33], m[_43]);
}

inline Vector4 Matrix4x4::GetColumn4() const
{
	return Vector4(m[_14], m[_24], m[_34], m[_44]);
}

inline Vector4 Matrix4x4::GetRow1() const
{
	return Vector4(m[_11], m[_12], m[_13], m[_14]);
}

inline Vector4 Matrix4x4::GetRow2() const
{
	return Vector4(m[_21], m[_22], m[_23], m[_24]);
}

inline Vector4 Matrix4x4::GetRow3() const
{
	return Vector4(m[_31], m[_32], m[_33], m[_34]);
}

inline Vector4 Matrix4x4::GetRow4() const
{
	return Vector4(m[_41], m[_42], m[_43], m[_44]);
}

inline Matrix4x4 Matrix4x4::Identity()
{
	Matrix4x4 identity;

	identity.m[_11] = 1.0f;
	identity.m[_12] = 0.0f;
	identity.m[_13] = 0.0f;
	identity.m[_14] = 0.0f;
	identity.m[_21] = 0.0f;
	identity.m[_22] = 1.0f;
	identity.m[_23] = 0.0f;
	identity.m[_24] = 0.0f;
	identity.m[_31] = 0.0f;
	identity.m[_32] = 0.0f;
	identity.m[_33] = 1.0f;
	identity.m[_34] = 0.0f;
	identity.m[_41] = 0.0f;
	identity.m[_42] = 0.0f;
	identity.m[_43] = 0.0f;
	identity.m[_44] = 1.0f;

	return identity;
}

inline Matrix4x4 Matrix4x4::CreateBillboard(const Vector3 &objectPosition, const Vector3 &cameraPosition, const Vector3 &cameraUp, const Vector3 &cameraForward)
{
	Vector3 forward = objectPosition - cameraPosition;
	float forwardLengthSquared = Vector3::LengthSquared(forward);
	if (forwardLengthSquared < 0.0001f)
		forward = -cameraForward;
	else
		forward = forward * (1.0f / (sqrtf(forwardLengthSquared)));
		
	Vector3 left = Vector3::Normalize(Vector3::Cross(cameraUp, forward));
	Vector3 up = Vector3::Cross(forward, left);
		
	Matrix4x4 out;
		
	out.m[_11] = left.x;
	out.m[_21] = left.y;
	out.m[_31] = left.z;
	out.m[_41] = 0.0f;
		
	out.m[_12] = up.x;
	out.m[_22] = up.y;
	out.m[_32] = up.z;
	out.m[_42] = 0.0f;
		
	out.m[_13] = forward.x;
	out.m[_23] = forward.y;
	out.m[_33] = forward.z;
	out.m[_43] = 0.0f;
		
	out.m[_14] = objectPosition.x;
	out.m[_24] = objectPosition.y;
	out.m[_34] = objectPosition.z;
	out.m[_44] = 1.0f;
		
	return out;
}

inline Matrix4x4 Matrix4x4::CreateCylindricalBillboard(const Vector3 &objectPosition, const Vector3 &cameraPosition, const Vector3 &cameraForward, const Vector3 &axis)
{
	Vector3 temp = objectPosition - cameraPosition;
	float lengthSquared = Vector3::LengthSquared(temp);
	if (lengthSquared < 0.0001f)
		temp = -cameraForward;
	else
		temp = temp * (1.0f / (sqrtf(lengthSquared)));
	
	Vector3 up = axis;
	Vector3 forward;
	Vector3 left;
	
	float dot = Vector3::Dot(axis, temp);
	if (fabsf(dot) > 0.9982547f)
	{
		dot = Vector3::Dot(axis, FORWARD);
		if (fabsf(dot) > 0.9982547f)
			forward = RIGHT;
		else
			forward = FORWARD;
		
		left = Vector3::Normalize(Vector3::Cross(axis, forward));
		forward = Vector3::Normalize(Vector3::Cross(left, axis));
	}
	else
	{
		left = Vector3::Normalize(Vector3::Cross(axis, temp));
		forward = Vector3::Normalize(Vector3::Cross(left, up));
	}
	
 	Matrix4x4 out;
 	
	out.m[_11] = left.x;
	out.m[_21] = left.y;
	out.m[_31] = left.z;
	out.m[_41] = 0.0f;
	
	out.m[_12] = up.x;
	out.m[_22] = up.y;
	out.m[_32] = up.z;
	out.m[_42] = 0.0f;
	
	out.m[_13] = forward.x;
	out.m[_23] = forward.y;
	out.m[_33] = forward.z;
	out.m[_43] = 0.0f;
	
	out.m[_14] = objectPosition.x;
	out.m[_24] = objectPosition.y;
	out.m[_34] = objectPosition.z;
	out.m[_44] = 1.0f;
	
	return out;
}

inline Matrix4x4 Matrix4x4::CreateScreenAlignedBillboard(const Vector3 &objectPosition, const Vector3 &cameraUp, const Vector3 &cameraForward)
{
	Vector3 left = Vector3::Normalize(Vector3::Cross(cameraUp, cameraForward));
	Vector3 up = Vector3::Cross(cameraForward, left);

 	Matrix4x4 out;

	out.m[_11] = left.x;
	out.m[_21] = left.y;
	out.m[_31] = left.z;
	out.m[_41] = 0.0f;

	out.m[_12] = up.x;
	out.m[_22] = up.y;
	out.m[_32] = up.z;
	out.m[_42] = 0.0f;

	out.m[_13] = cameraForward.x;
	out.m[_23] = cameraForward.y;
	out.m[_33] = cameraForward.z;
	out.m[_43] = 0.0f;

	out.m[_14] = objectPosition.x;
	out.m[_24] = objectPosition.y;
	out.m[_34] = objectPosition.z;
	out.m[_44] = 1.0f;

	return out;
}

inline Matrix4x4 Matrix4x4::CreateScreenAndAxisAlignedBillboard(const Vector3 &objectPosition, const Vector3 &cameraForward, const Vector3 &axis)
{
	Vector3 up = axis;
	Vector3 forward;
	Vector3 left;

	float dot = Vector3::Dot(axis, cameraForward);
	if (fabsf(dot) > 0.9982547f)
	{
		dot = Vector3::Dot(axis, FORWARD);
		if (fabsf(dot) > 0.9982547f)
			forward = RIGHT;
		else
			forward = FORWARD;

		left = Vector3::Normalize(Vector3::Cross(axis, forward));
		forward = Vector3::Normalize(Vector3::Cross(left, axis));
	}
	else
	{
		left = Vector3::Normalize(Vector3::Cross(axis, cameraForward));
		forward = Vector3::Normalize(Vector3::Cross(left, up));
	}

 	Matrix4x4 out;

	out.m[_11] = left.x;
	out.m[_21] = left.y;
	out.m[_31] = left.z;
	out.m[_41] = 0.0f;

	out.m[_12] = up.x;
	out.m[_22] = up.y;
	out.m[_32] = up.z;
	out.m[_42] = 0.0f;

	out.m[_13] = forward.x;
	out.m[_23] = forward.y;
	out.m[_33] = forward.z;
	out.m[_43] = 0.0f;

	out.m[_14] = objectPosition.x;
	out.m[_24] = objectPosition.y;
	out.m[_34] = objectPosition.z;
	out.m[_44] = 1.0f;

	return out;
}

inline Matrix4x4 Matrix4x4::CreateFromEulerAngles(float x, float y, float z)
{
	Matrix4x4 rotateZ = CreateRotationZ(z);
	Matrix4x4 rotateY = CreateRotationY(y);
	Matrix4x4 rotateX = CreateRotationX(x);

	// "right-to-left" column-major matrix concatenation
	return rotateZ * rotateY * rotateX;
}

inline Matrix4x4 Matrix4x4::CreateLookAt(const Vector3 &cameraPosition, const Vector3 &cameraTarget, const Vector3 &cameraUp)
{
	Matrix4x4 out;

	// build basic lookat matrix without translation component included
	Vector3 forward = Vector3::Normalize(cameraTarget - cameraPosition);
	Vector3 left = Vector3::Normalize(Vector3::Cross(forward, cameraUp));
	Vector3 up = Vector3::Cross(left, forward);
		
	out.m[_11] = left.x;
	out.m[_21] = up.x;
	out.m[_31] = -forward.x;
	out.m[_41] = 0.0f;
		
	out.m[_12] = left.y;
	out.m[_22] = up.y;
	out.m[_32] = -forward.y;
	out.m[_42] = 0.0f;
		
	out.m[_13] = left.z;
	out.m[_23] = up.z;
	out.m[_33] = -forward.z;
	out.m[_43] = 0.0f;
		
	out.m[_14] = 0.0f;
	out.m[_24] = 0.0f;
	out.m[_34] = 0.0f;
	out.m[_44] = 1.0f;
		
	// multiply the translation into the lookat matrix 
	// this matrix multiplication is simplified so that we're only multiplying components that can actually affect the result
	// out = Matrix4x4::CreateTranslation(-cameraPosition.x, -cameraPosition.y, -cameraPosition.z) * out;
	out.m[_14] = out.m[_11] * -cameraPosition.x + out.m[_12] * -cameraPosition.y + out.m[_13] * -cameraPosition.z + out.m[_14];
	out.m[_24] = out.m[_21] * -cameraPosition.x + out.m[_22] * -cameraPosition.y + out.m[_23] * -cameraPosition.z + out.m[_24];
	out.m[_34] = out.m[_31] * -cameraPosition.x + out.m[_32] * -cameraPosition.y + out.m[_33] * -cameraPosition.z + out.m[_34];
	out.m[_44] = out.m[_41] * -cameraPosition.x + out.m[_42] * -cameraPosition.y + out.m[_43] * -cameraPosition.z + out.m[_44];
		
	return out;
}

inline Matrix4x4 Matrix4x4::CreateOrthographic(float left, float right, float bottom, float top, float near_, float far_)
{
	Matrix4x4 out;
		
	out.m[_11] = 2.0f / (right - left);
	out.m[_12] = 0.0f;
	out.m[_13] = 0.0f;
	out.m[_14] = -((right + left) / (right - left));
		
	out.m[_21] = 0.0f;
	out.m[_22] = 2.0f / (top - bottom);
	out.m[_23] = 0.0f;
	out.m[_24] = -((top + bottom) / (top - bottom));
		
	out.m[_31] = 0.0f;
	out.m[_32] = 0.0f;
	out.m[_33] = -2.0f / (far_ - near_);
	out.m[_34] = -((far_ + near_) / (far_ - near_));
		
	out.m[_41] = 0.0f;
	out.m[_42] = 0.0f;
	out.m[_43] = 0.0f;
	out.m[_44] = 1.0f;
			
	return out;
}

inline Matrix4x4 Matrix4x4::CreatePerspective(float left, float right, float bottom, float top, float near_, float far_)
{
	Matrix4x4 out;
		
	out.m[_11] = (2.0f * near_) / (right - left);
	out.m[_12] = 0.0f;
	out.m[_13] = (right + left) / (right - left);
	out.m[_14] = 0.0f;
		
	out.m[_21] = 0.0f;
	out.m[_22] = (2.0f * near_) / (top - bottom);
	out.m[_23] = (top + bottom) / (top - bottom);
	out.m[_24] = 0.0f;
		
	out.m[_31] = 0.0f;
	out.m[_32] = 0.0f;
	out.m[_33] = -((far_ + near_)) / (far_ - near_);
	out.m[_34] = -((2.0f * far_ * near_)) / (far_ - near_);
		
	out.m[_41] = 0.0f;
	out.m[_42] = 0.0f;
	out.m[_43] = -1.0f;
	out.m[_44] = 0.0f;
		
	return out;
}

inline Matrix4x4 Matrix4x4::CreatePerspectiveFieldOfView(float fieldOfView, float aspectRatio, float near_, float far_)
{
	Matrix4x4 out;
		
	float f = 1.0f / tanf(fieldOfView / 2.0f);
		
	out.m[_11] = f / aspectRatio;
	out.m[_12] = 0.0f;
	out.m[_13] = 0.0f;
	out.m[_14] = 0.0f;
		
	out.m[_21] = 0.0f;
	out.m[_22] = f;
	out.m[_23] = 0.0f;
	out.m[_24] = 0.0f;
		
	out.m[_31] = 0.0f;
	out.m[_32] = 0.0f;
	out.m[_33] = (far_ + near_) / (near_ - far_);
	out.m[_34] = (2.0f * far_ * near_) / (near_ - far_);
		
	out.m[_41] = 0.0f;
	out.m[_42] = 0.0f;
	out.m[_43] = -1.0f;
	out.m[_44] = 0.0f;
		
	return out;
}

inline Matrix4x4 Matrix4x4::CreateRotation(float angle, const Vector3 &axis)
{
	Matrix4x4 out;
		
	float s = sinf(angle);
	float c = cosf(angle);
		
	out.m[_11] = (axis.x * axis.x) * (1.0f - c) + c;
	out.m[_12] = (axis.x * axis.y) * (1.0f - c) - (axis.z * s);
	out.m[_13] = (axis.x * axis.z) * (1.0f - c) + (axis.y * s);
	out.m[_14] = 0.0f;

	out.m[_21] = (axis.y * axis.x) * (1.0f - c) + (axis.z * s);
	out.m[_22] = (axis.y * axis.y) * (1.0f - c) + c;
	out.m[_23] = (axis.y * axis.z) * (1.0f - c) - (axis.x * s);
	out.m[_24] = 0.0f;

	out.m[_31] = (axis.z * axis.x) * (1.0f - c) - (axis.y * s);
	out.m[_32] = (axis.z * axis.y) * (1.0f - c) + (axis.x * s);
	out.m[_33] = (axis.z * axis.z) * (1.0f - c) + c;
	out.m[_34] = 0.0f;
		
	out.m[_41] = 0.0f;
	out.m[_42] = 0.0f;
	out.m[_43] = 0.0f;
	out.m[_44] = 1.0f;
		
	return out;
}

inline Matrix4x4 Matrix4x4::CreateRotationX(float angle)
{
	Matrix4x4 out;
		
	float s = sinf(angle);
	float c = cosf(angle);
	
	out.m[_11] = 1.0f;
	out.m[_12] = 0.0f;
	out.m[_13] = 0.0f;
	out.m[_14] = 0.0f;
		
	out.m[_21] = 0.0f;
	out.m[_22] = c;
	out.m[_23] = -s;
	out.m[_24] = 0.0f;
		
	out.m[_31] = 0.0f;
	out.m[_32] = s;
	out.m[_33] = c;
	out.m[_34] = 0.0f;
		
	out.m[_41] = 0.0f;
	out.m[_42] = 0.0f;
	out.m[_43] = 0.0f;
	out.m[_44] = 1.0f;
		
	return out;
}

inline Matrix4x4 Matrix4x4::CreateRotationY(float angle)
{
	Matrix4x4 out;

	float s = sinf(angle);
	float c = cosf(angle);
		
	out.m[_11] = c;
	out.m[_12] = 0.0f;
	out.m[_13] = s;
	out.m[_14] = 0.0f;
		
	out.m[_21] = 0.0f;
	out.m[_22] = 1.0f;
	out.m[_23] = 0.0f;
	out.m[_24] = 0.0f;
		
	out.m[_31] = -s;
	out.m[_32] = 0.0f;
	out.m[_33] = c;
	out.m[_34] = 0.0f;
		
	out.m[_41] = 0.0f;
	out.m[_42] = 0.0f;
	out.m[_43] = 0.0f;
	out.m[_44] = 1.0f;
		
	return out;
}

inline Matrix4x4 Matrix4x4::CreateRotationZ(float angle)
{
	Matrix4x4 out;

	float s = sinf(angle);
	float c = cosf(angle);
		
	out.m[_11] = c;
	out.m[_12] = -s;
	out.m[_13] = 0.0f;
	out.m[_14] = 0.0f;
		
	out.m[_21] = s;
	out.m[_22] = c;
	out.m[_23] = 0.0f;
	out.m[_24] = 0.0f;
		
	out.m[_31] = 0.0f;
	out.m[_32] = 0.0f;
	out.m[_33] = 1.0f;
	out.m[_34] = 0.0f;
		
	out.m[_41] = 0.0f;
	out.m[_42] = 0.0f;
	out.m[_43] = 0.0f;
	out.m[_44] = 1.0f;
		
	return out;
}

inline Matrix4x4 Matrix4x4::CreateScale(float x, float y, float z)
{
	Matrix4x4 out;
		
	out.m[_11] = x;
	out.m[_12] = 0.0f;
	out.m[_13] = 0.0f;
	out.m[_14] = 0.0f;
		
	out.m[_21] = 0.0f;
	out.m[_22] = y;
	out.m[_23] = 0.0f;
	out.m[_24] = 0.0f;
		
	out.m[_31] = 0.0f;
	out.m[_32] = 0.0f;
	out.m[_33] = z;
	out.m[_34] = 0.0f;
		
	out.m[_41] = 0.0f;
	out.m[_42] = 0.0f;
	out.m[_43] = 0.0f;
	out.m[_44] = 1.0f;
		
	return out;
}

inline Matrix4x4 Matrix4x4::CreateTranslation(float x, float y, float z)
{
	Matrix4x4 out;
	
	out.m[_11] = 1.0f;
	out.m[_12] = 0.0f;
	out.m[_13] = 0.0f;
	out.m[_14] = x;
		
	out.m[_21] = 0.0f;
	out.m[_22] = 1.0f;
	out.m[_23] = 0.0f;
	out.m[_24] = y;
		
	out.m[_31] = 0.0f;
	out.m[_32] = 0.0f;
	out.m[_33] = 1.0f;
	out.m[_34] = z;
		
	out.m[_41] = 0.0f;
	out.m[_42] = 0.0f;
	out.m[_43] = 0.0f;
	out.m[_44] = 1.0f;
		
	return out;
}

inline Matrix4x4 Matrix4x4::CreateWorld(const Vector3 &position, const Vector3 &forward, const Vector3 &up)
{
	Matrix4x4 out;

	Vector3 f = Vector3::Normalize(-forward);
	Vector3 l = Vector3::Normalize(Vector3::Cross(up, f));
	Vector3 u = Vector3::Cross(f, l);

	out.m[_11] = l.x;
	out.m[_21] = l.y;
	out.m[_31] = l.z;
	out.m[_41] = 0.0f;
		
	out.m[_12] = u.x;
	out.m[_22] = u.y;
	out.m[_32] = u.z;
	out.m[_42] = 0.0f;
		
	out.m[_13] = f.x;
	out.m[_23] = f.y;
	out.m[_33] = f.z;
	out.m[_43] = 0.0f;
		
	out.m[_14] = position.x;
	out.m[_24] = position.y;
	out.m[_34] = position.z;
	out.m[_44] = 1.0f;
		
	return out;
}

inline Matrix4x4 Matrix4x4::Inverse(const Matrix4x4 &m)
{
	float d = m.GetDeterminant();
	if (IsCloseEnough(d, 0.0f))
		return IDENTITY_MATRIX;
	else
	{
		Matrix4x4 out;
		
		d = 1.0f / d;
			
		out.m[_11] = d * (m.m[_22] * (m.m[_33] * m.m[_44] - m.m[_43] * m.m[_34]) + m.m[_32] * (m.m[_43] * m.m[_24] - m.m[_23] * m.m[_44]) + m.m[_42] * (m.m[_23] * m.m[_34] - m.m[_33] * m.m[_24]));
		out.m[_21] = d * (m.m[_23] * (m.m[_31] * m.m[_44] - m.m[_41] * m.m[_34]) + m.m[_33] * (m.m[_41] * m.m[_24] - m.m[_21] * m.m[_44]) + m.m[_43] * (m.m[_21] * m.m[_34] - m.m[_31] * m.m[_24]));
		out.m[_31] = d * (m.m[_24] * (m.m[_31] * m.m[_42] - m.m[_41] * m.m[_32]) + m.m[_34] * (m.m[_41] * m.m[_22] - m.m[_21] * m.m[_42]) + m.m[_44] * (m.m[_21] * m.m[_32] - m.m[_31] * m.m[_22]));
		out.m[_41] = d * (m.m[_21] * (m.m[_42] * m.m[_33] - m.m[_32] * m.m[_43]) + m.m[_31] * (m.m[_22] * m.m[_43] - m.m[_42] * m.m[_23]) + m.m[_41] * (m.m[_32] * m.m[_23] - m.m[_22] * m.m[_33]));
			
		out.m[_12] = d * (m.m[_32] * (m.m[_13] * m.m[_44] - m.m[_43] * m.m[_14]) + m.m[_42] * (m.m[_33] * m.m[_14] - m.m[_13] * m.m[_34]) + m.m[_12] * (m.m[_43] * m.m[_34] - m.m[_33] * m.m[_44]));
		out.m[_22] = d * (m.m[_33] * (m.m[_11] * m.m[_44] - m.m[_41] * m.m[_14]) + m.m[_43] * (m.m[_31] * m.m[_14] - m.m[_11] * m.m[_34]) + m.m[_13] * (m.m[_41] * m.m[_34] - m.m[_31] * m.m[_44]));
		out.m[_32] = d * (m.m[_34] * (m.m[_11] * m.m[_42] - m.m[_41] * m.m[_12]) + m.m[_44] * (m.m[_31] * m.m[_12] - m.m[_11] * m.m[_32]) + m.m[_14] * (m.m[_41] * m.m[_32] - m.m[_31] * m.m[_42]));
		out.m[_42] = d * (m.m[_31] * (m.m[_42] * m.m[_13] - m.m[_12] * m.m[_43]) + m.m[_41] * (m.m[_12] * m.m[_33] - m.m[_32] * m.m[_13]) + m.m[_11] * (m.m[_32] * m.m[_43] - m.m[_42] * m.m[_33]));
			
		out.m[_13] = d * (m.m[_42] * (m.m[_13] * m.m[_24] - m.m[_23] * m.m[_14]) + m.m[_12] * (m.m[_23] * m.m[_44] - m.m[_43] * m.m[_24]) + m.m[_22] * (m.m[_43] * m.m[_14] - m.m[_13] * m.m[_44]));
		out.m[_23] = d * (m.m[_43] * (m.m[_11] * m.m[_24] - m.m[_21] * m.m[_14]) + m.m[_13] * (m.m[_21] * m.m[_44] - m.m[_41] * m.m[_24]) + m.m[_23] * (m.m[_41] * m.m[_14] - m.m[_11] * m.m[_44]));
		out.m[_33] = d * (m.m[_44] * (m.m[_11] * m.m[_22] - m.m[_21] * m.m[_12]) + m.m[_14] * (m.m[_21] * m.m[_42] - m.m[_41] * m.m[_22]) + m.m[_24] * (m.m[_41] * m.m[_12] - m.m[_11] * m.m[_42]));
		out.m[_43] = d * (m.m[_41] * (m.m[_22] * m.m[_13] - m.m[_12] * m.m[_23]) + m.m[_11] * (m.m[_42] * m.m[_23] - m.m[_22] * m.m[_43]) + m.m[_21] * (m.m[_12] * m.m[_43] - m.m[_42] * m.m[_13]));
			
		out.m[_14] = d * (m.m[_12] * (m.m[_33] * m.m[_24] - m.m[_23] * m.m[_34]) + m.m[_22] * (m.m[_13] * m.m[_34] - m.m[_33] * m.m[_14]) + m.m[_32] * (m.m[_23] * m.m[_14] - m.m[_13] * m.m[_24]));
		out.m[_24] = d * (m.m[_13] * (m.m[_31] * m.m[_24] - m.m[_21] * m.m[_34]) + m.m[_23] * (m.m[_11] * m.m[_34] - m.m[_31] * m.m[_14]) + m.m[_33] * (m.m[_21] * m.m[_14] - m.m[_11] * m.m[_24]));
		out.m[_34] = d * (m.m[_14] * (m.m[_31] * m.m[_22] - m.m[_21] * m.m[_32]) + m.m[_24] * (m.m[_11] * m.m[_32] - m.m[_31] * m.m[_12]) + m.m[_34] * (m.m[_21] * m.m[_12] - m.m[_11] * m.m[_22]));
		out.m[_44] = d * (m.m[_11] * (m.m[_22] * m.m[_33] - m.m[_32] * m.m[_23]) + m.m[_21] * (m.m[_32] * m.m[_13] - m.m[_12] * m.m[_33]) + m.m[_31] * (m.m[_12] * m.m[_23] - m.m[_22] * m.m[_13]));
		
		return out;
	}
}

inline Matrix4x4 Matrix4x4::Transpose(const Matrix4x4 &m)
{
	Matrix4x4 out;
		
	out.m[_11] = m.m[_11];
	out.m[_12] = m.m[_21];
	out.m[_13] = m.m[_31];
	out.m[_14] = m.m[_41];

	out.m[_21] = m.m[_12];
	out.m[_22] = m.m[_22];
	out.m[_23] = m.m[_32];
	out.m[_24] = m.m[_42];

	out.m[_31] = m.m[_13];
	out.m[_32] = m.m[_23];
	out.m[_33] = m.m[_33];
	out.m[_34] = m.m[_43];

	out.m[_41] = m.m[_14];
	out.m[_42] = m.m[_24];
	out.m[_43] = m.m[_34];
	out.m[_44] = m.m[_44];
		
	return out;
}

inline Vector3 Matrix4x4::Transform(const Matrix4x4 &m, const Vector3 &v)
{
	Vector3 out;
	
	out.x = v.x * m.m[_11] + v.y * m.m[_12] + v.z * m.m[_13] + m.m[_14];
	out.y = v.x * m.m[_21] + v.y * m.m[_22] + v.z * m.m[_23] + m.m[_24];
	out.z = v.x * m.m[_31] + v.y * m.m[_32] + v.z * m.m[_33] + m.m[_34];
	
	return out;
}

inline Vector4 Matrix4x4::Transform(const Matrix4x4 &m, const Vector4 &v)
{
	Vector4 out;
	
	out.x = v.x * m.m[_11] + v.y * m.m[_12] + v.z * m.m[_13] + v.w * m.m[_14];
	out.y = v.x * m.m[_21] + v.y * m.m[_22] + v.z * m.m[_23] + v.w * m.m[_24];
	out.z = v.x * m.m[_31] + v.y * m.m[_32] + v.z * m.m[_33] + v.w * m.m[_34];
	out.w = v.x * m.m[_41] + v.y * m.m[_42] + v.z * m.m[_43] + v.w * m.m[_44];
	
	return out;
}

inline Vector3 Matrix4x4::TransformUsingRotationOnly(const Matrix4x4 &m, const Vector3 &v)
{
	Vector3 out;
	
	out.x = v.x * m.m[_11] + v.y * m.m[_12] + v.z * m.m[_13];
	out.y = v.x * m.m[_21] + v.y * m.m[_22] + v.z * m.m[_23];
	out.z = v.x * m.m[_31] + v.y * m.m[_32] + v.z * m.m[_33];
	
	return out;
}

inline Matrix4x4 operator+(const Matrix4x4 &left, const Matrix4x4 &right)
{
	Matrix4x4 result;

	result.m[_11] = left.m[_11] + right.m[_11]; 
	result.m[_12] = left.m[_12] + right.m[_12];
	result.m[_13] = left.m[_13] + right.m[_13];
	result.m[_14] = left.m[_14] + right.m[_14];
	result.m[_21] = left.m[_21] + right.m[_21];
	result.m[_22] = left.m[_22] + right.m[_22];
	result.m[_23] = left.m[_23] + right.m[_23];
	result.m[_24] = left.m[_24] + right.m[_24];
	result.m[_31] = left.m[_31] + right.m[_31];
	result.m[_32] = left.m[_32] + right.m[_32];
	result.m[_33] = left.m[_33] + right.m[_33];
	result.m[_34] = left.m[_34] + right.m[_34];
	result.m[_41] = left.m[_41] + right.m[_41];
	result.m[_42] = left.m[_42] + right.m[_42];
	result.m[_43] = left.m[_43] + right.m[_43];
	result.m[_44] = left.m[_44] + right.m[_44];

	return result;
}

inline Matrix4x4 &operator+=(Matrix4x4 &left, const Matrix4x4 &right)
{
	left = left + right;
	return left;
}

inline Matrix4x4 operator-(const Matrix4x4 &left, const Matrix4x4 &right)
{
	Matrix4x4 result;

	result.m[_11] = left.m[_11] - right.m[_11]; 
	result.m[_12] = left.m[_12] - right.m[_12];
	result.m[_13] = left.m[_13] - right.m[_13];
	result.m[_14] = left.m[_14] - right.m[_14];
	result.m[_21] = left.m[_21] - right.m[_21];
	result.m[_22] = left.m[_22] - right.m[_22];
	result.m[_23] = left.m[_23] - right.m[_23];
	result.m[_24] = left.m[_24] - right.m[_24];
	result.m[_31] = left.m[_31] - right.m[_31];
	result.m[_32] = left.m[_32] - right.m[_32];
	result.m[_33] = left.m[_33] - right.m[_33];
	result.m[_34] = left.m[_34] - right.m[_34];
	result.m[_41] = left.m[_41] - right.m[_41];
	result.m[_42] = left.m[_42] - right.m[_42];
	result.m[_43] = left.m[_43] - right.m[_43];
	result.m[_44] = left.m[_44] - right.m[_44];

	return result;
}

inline Matrix4x4 &operator-=(Matrix4x4 &left, const Matrix4x4 &right)
{
	left = left - right;
	return left;
}

inline Matrix4x4 operator*(const Matrix4x4 &left, const Matrix4x4 &right)
{
	Matrix4x4 result;

	result.m[_11] = left.m[_11] * right.m[_11] + left.m[_12] * right.m[_21] + left.m[_13] * right.m[_31] + left.m[_14] * right.m[_41];
	result.m[_12] = left.m[_11] * right.m[_12] + left.m[_12] * right.m[_22] + left.m[_13] * right.m[_32] + left.m[_14] * right.m[_42];
	result.m[_13] = left.m[_11] * right.m[_13] + left.m[_12] * right.m[_23] + left.m[_13] * right.m[_33] + left.m[_14] * right.m[_43];
	result.m[_14] = left.m[_11] * right.m[_14] + left.m[_12] * right.m[_24] + left.m[_13] * right.m[_34] + left.m[_14] * right.m[_44];

	result.m[_21] = left.m[_21] * right.m[_11] + left.m[_22] * right.m[_21] + left.m[_23] * right.m[_31] + left.m[_24] * right.m[_41];
	result.m[_22] = left.m[_21] * right.m[_12] + left.m[_22] * right.m[_22] + left.m[_23] * right.m[_32] + left.m[_24] * right.m[_42];
	result.m[_23] = left.m[_21] * right.m[_13] + left.m[_22] * right.m[_23] + left.m[_23] * right.m[_33] + left.m[_24] * right.m[_43];
	result.m[_24] = left.m[_21] * right.m[_14] + left.m[_22] * right.m[_24] + left.m[_23] * right.m[_34] + left.m[_24] * right.m[_44];

	result.m[_31] = left.m[_31] * right.m[_11] + left.m[_32] * right.m[_21] + left.m[_33] * right.m[_31] + left.m[_34] * right.m[_41];
	result.m[_32] = left.m[_31] * right.m[_12] + left.m[_32] * right.m[_22] + left.m[_33] * right.m[_32] + left.m[_34] * right.m[_42];
	result.m[_33] = left.m[_31] * right.m[_13] + left.m[_32] * right.m[_23] + left.m[_33] * right.m[_33] + left.m[_34] * right.m[_43];
	result.m[_34] = left.m[_31] * right.m[_14] + left.m[_32] * right.m[_24] + left.m[_33] * right.m[_34] + left.m[_34] * right.m[_44];

	result.m[_41] = left.m[_41] * right.m[_11] + left.m[_42] * right.m[_21] + left.m[_43] * right.m[_31] + left.m[_44] * right.m[_41];
	result.m[_42] = left.m[_41] * right.m[_12] + left.m[_42] * right.m[_22] + left.m[_43] * right.m[_32] + left.m[_44] * right.m[_42];
	result.m[_43] = left.m[_41] * right.m[_13] + left.m[_42] * right.m[_23] + left.m[_43] * right.m[_33] + left.m[_44] * right.m[_43];
	result.m[_44] = left.m[_41] * right.m[_14] + left.m[_42] * right.m[_24] + left.m[_43] * right.m[_34] + left.m[_44] * right.m[_44];

	return result;
}

inline Matrix4x4 &operator*=(Matrix4x4 &left, const Matrix4x4 &right)
{
	left = left * right;
	return left;
}

inline Matrix4x4 operator*(const Matrix4x4 &left, float right)
{
	Matrix4x4 result;

	result.m[_11] = left.m[_11] * right;
	result.m[_12] = left.m[_12] * right;
	result.m[_13] = left.m[_13] * right;
	result.m[_14] = left.m[_14] * right;
	result.m[_21] = left.m[_21] * right;
	result.m[_22] = left.m[_22] * right;
	result.m[_23] = left.m[_23] * right;
	result.m[_24] = left.m[_24] * right;
	result.m[_31] = left.m[_31] * right;
	result.m[_32] = left.m[_32] * right;
	result.m[_33] = left.m[_33] * right;
	result.m[_34] = left.m[_34] * right;
	result.m[_41] = left.m[_41] * right;
	result.m[_42] = left.m[_42] * right;
	result.m[_43] = left.m[_43] * right;
	result.m[_44] = left.m[_44] * right;

	return result;
}

inline Matrix4x4 &operator*=(Matrix4x4 &left, float right)
{
	left = left * right;
	return left;
}

inline Vector3 operator*(const Vector3 &left, const Matrix4x4 &right)
{
	return Matrix4x4::Transform(right, left);
}

inline Vector4 operator*(const Vector4 &left, const Matrix4x4 &right)
{
	return Matrix4x4::Transform(right, left);
}

#endif

