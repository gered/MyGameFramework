#ifndef __FRAMEWORK_GRAPHICS_VERTEXBUFFER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_VERTEXBUFFER_H_INCLUDED__

#include <stdlib.h>
#include "../common.h"

#include "bufferobject.h"
#include "color.h"
#include "../math/matrix3x3.h"
#include "../math/matrix4x4.h"
#include "../math/vector2.h"
#include "../math/vector3.h"
#include <string.h>

#include <stl/vector.h>

#include "vertexattribs.h"

class GraphicsDevice;

/**
 * Wraps management of an array of vertices stored either completely
 * in main memory (client-side) or in video memory.
 */
class VertexBuffer : public BufferObject
{
public:
	VertexBuffer();
	virtual ~VertexBuffer()                                                     { Release(); }
	
	/**
	 * Releases all resources associated with this vertex buffer.
	 */
	virtual void Release();
	
	/**
	 * Initializes the vertex buffer.
	 * @param attributes the vertex attributes this buffer will contain
	 * @param numAttributes the number of vertex attributes
	 * @param numVertices the initial number of vertices this buffer will contain
	 * @param usage the expected usage pattern of this vertex buffer
	 * @return TRUE if successful, FALSE if not
	 */
	BOOL Initialize(const VERTEX_ATTRIBS *attributes, uint numAttributes, uint numVertices, BUFFEROBJECT_USAGE usage);

	/**
	 * Initializes the vertex buffer.
	 * @param graphicsDevice the graphics device to use to create this buffer
	 *                       on the GPU
	 * @param attributes the vertex attributes this buffer will contain
	 * @param numAttributes the number of vertex attributes
	 * @param numVertices the initial number of vertices this buffer will contain
	 * @param usage the expected usage pattern of this vertex buffer
	 * @return TRUE if successful, FALSE if not
	 */
	BOOL Initialize(GraphicsDevice *graphicsDevice, const VERTEX_ATTRIBS *attributes, uint numAttributes, uint numVertices, BUFFEROBJECT_USAGE usage);
	
	/**
	 * Initializes the vertex buffer.
	 * @param source the source buffer to create this buffer from. it's
	 *               properties and vertex data will be used to create this one
	 * @return TRUE if successful, FALSE if not
	 */
	BOOL Initialize(const VertexBuffer *source);

	/**
	 * Initializes the vertex buffer.
	 * @param graphicsDevice the graphics device to use to create this buffer
	 *                       on the GPU
	 * @param source the source buffer to create this buffer from. it's
	 *               properties and vertex data will be used to create this one
	 * @return TRUE if successful, FALSE if not
	 */
	BOOL Initialize(GraphicsDevice *graphicsDevice, const VertexBuffer *source);

	/**
	 * @return the number of attributes in this vertex buffer
	 */
	uint GetNumAttributes() const                                               { return m_numAttributes; }

	/**
	 * Returns information about the specified attribute.
	 * @param index the index of the attribute to get information about
	 * @return the attribute's information
	 */
	const VertexBufferAttribute* GetAttributeInfo(uint index) const             { return &m_attribs[index]; }

	/**
	 * @return the standard attributes included in this buffer's data. Note
	 *         that this is not necessarily all of the attributes contained in
	 *         this buffer.
	 */
	uint GetStandardAttribs() const                                             { return m_standardTypeAttribs; }

	/**
	 * Checks whether this buffer's vertex data includes the specified standard
	 * attribute.
	 * @param standardAttrib the standard attribute to test
	 * @return TRUE if the vertex data in this buffer contains this standard
	 *              attribute
	 */
	BOOL HasStandardAttrib(VERTEX_STANDARD_ATTRIBS standardAttrib) const        { return (m_standardTypeAttribs & (uint)standardAttrib) > 0; }

	/**
	 * Returns the index of the specified standard attribute.
	 * @param standardAttrib the standard attribute to get the index of
	 * @return the index of the attribute, or -1 if it is not present
	 */
	int GetIndexOfStandardAttrib(VERTEX_STANDARD_ATTRIBS standardAttrib) const;

	/**
	 * @return the offset from the start of a single vertex's data that the 
	 * color attribute starts at (in floats)
	 */
	uint GetColorOffset() const                                                 { return m_colorOffset; }

	/**
	 * @return the offset from the start of a single vertex's data that the 
	 * 3D position attribute starts at (in floats)
	 */
	uint GetPosition3Offset() const                                             { return m_position3Offset; }

	/**
	 * @return the offset from the start of a single vertex's data that the 
	 * 2D position attribute starts at (in floats)
	 */
	uint GetPosition2Offset() const                                             { return m_position2Offset; }

	/**
	 * @return the offset from the start of a single vertex's data that the 
	 * normal attribute starts at (in floats)
	 */
	uint GetNormalOffset() const                                                { return m_normalOffset; }

	/**
	 * @return the offset from the start of a single vertex's data that the 
	 * texture coordinate attribute starts at (in floats)
	 */
	uint GetTexCoordOffset() const                                              { return m_texCoordOffset; }

	/**
	 * @return the offset from the start of a single vertex's data that the
	 * color attribute starts at (in bytes)
	 */
	uint GetColorOffsetBytes() const                                            { return m_colorOffset * sizeof(float); }

	/**
	 * @return the offset from the start of a single vertex's data that the
	 * 3D position attribute starts at (in bytes)
	 */
	uint GetPosition3OffsetBytes() const                                        { return m_position3Offset * sizeof(float); }

	/**
	 * @return the offset from the start of a single vertex's data that the
	 * 2D position attribute starts at (in bytes)
	 */
	uint GetPosition2OffsetBytes() const                                        { return m_position2Offset * sizeof(float); }

	/**
	 * @return the offset from the start of a single vertex's data that the
	 * normal attribute starts at (in bytes)
	 */
	uint GetNormalOffsetBytes() const                                           { return m_normalOffset * sizeof(float); }

	/**
	 * @return the offset from the start of a single vertex's data that the
	 * texture coordinate attribute starts at (in bytes)
	 */
	uint GetTexCoordOffsetBytes() const                                         { return m_texCoordOffset * sizeof(float); }

	/**
	 * @param index the vertex to get the color data for
	 * @return the vertex's color data
	 */
	Color GetColor(uint index) const;

	/**
	 * @param index the vertex to get the position data for
	 * @return the vertex's 3D position data
	 */
	Vector3 GetPosition3(uint index) const;

	/**
	 * @param index the vertex to get the position data for
	 * @return the vertex's 2D position data
	 */
	Vector2 GetPosition2(uint index) const;

	/**
	 * @param index the vertex to get the normal data for
	 * @return the vertex's normal data
	 */
	Vector3 GetNormal(uint index) const;

	/**
	 * @param index the vertex to get the texture coordinate data for
	 * @return the vertex's texture coordinate data
	 */
	Vector2 GetTexCoord(uint index) const;

	void Get1f(uint attrib, uint index, float &x) const;
	float Get1f(uint attrib, uint index) const;
	void Get2f(uint attrib, uint index, float &x, float &y) const;
	void Get2f(uint attrib, uint index, Vector2 &v) const;
	Vector2 Get2f(uint attrib, uint index) const;
	void Get3f(uint attrib, uint index, float &x, float &y, float &z) const;
	void Get3f(uint attrib, uint index, Vector3 &v) const;
	Vector3 Get3f(uint attrib, uint index) const;
	void Get4f(uint attrib, uint index, float &x, float &y, float &z, float &w) const;
	void Get4f(uint attrib, uint index, Color &c) const;
	Color Get4f(uint attrib, uint index) const;
	void Get9f(uint attrib, uint index, Matrix3x3 &m) const;
	Matrix3x3 Get9f(uint attrib, uint index) const;
	void Get16f(uint attrib, uint index, Matrix4x4 &m) const;
	Matrix4x4 Get16f(uint attrib, uint index) const;

	/**
	 * @return the current vertex's color data
	 */
	Color GetCurrentColor() const                                               { return GetColor(GetCurrentPosition()); }

	/**
	 * @return the current vertex's 3D position data
	 */
	Vector3 GetCurrentPosition3() const                                         { return GetPosition3(GetCurrentPosition()); }

	/**
	 * @return the current vertex's 2D position data
	 */
	Vector2 GetCurrentPosition2() const                                         { return GetPosition2(GetCurrentPosition()); }

	/**
	 * @return the current vertex's normal data
	 */
	Vector3 GetCurrentNormal() const                                            { return GetNormal(GetCurrentPosition()); }

	/**
	 * @return the current vertex's texture coordinate data
	 */
	Vector2 GetCurrentTexCoord() const                                          { return GetTexCoord(GetCurrentPosition()); }

	void GetCurrent1f(uint attrib, float &x) const                               { Get1f(attrib, GetCurrentPosition(), x); }
	float GetCurrent1f(uint attrib) const                                        { return Get1f(attrib, GetCurrentPosition()); }
	void GetCurrent2f(uint attrib, float &x, float &y) const                     { Get2f(attrib, GetCurrentPosition(), x, y); }
	void GetCurrent2f(uint attrib, Vector2 &v) const                             { Get2f(attrib, GetCurrentPosition(), v); }
	Vector2 GetCurrent2f(uint attrib) const                                      { return Get2f(attrib, GetCurrentPosition()); }
	void GetCurrent3f(uint attrib, float &x, float &y, float &z) const           { Get3f(attrib, GetCurrentPosition(), x, y, z); }
	void GetCurrent3f(uint attrib, Vector3 &v) const                             { Get3f(attrib, GetCurrentPosition(), v); }
	Vector3 GetCurrent3f(uint attrib) const                                      { return Get3f(attrib, GetCurrentPosition()); }
	void GetCurrent4f(uint attrib, float &x, float &y, float &z, float &w) const { Get4f(attrib, GetCurrentPosition(), x, y, z, w); }
	void GetCurrent4f(uint attrib, Color &c) const                               { Get4f(attrib, GetCurrentPosition(), c); }
	Color GetCurrent4f(uint attrib) const                                        { return Get4f(attrib, GetCurrentPosition()); }
	void GetCurrent9f(uint attrib, Matrix3x3 &m) const                           { Get9f(attrib, GetCurrentPosition(), m); }
	Matrix3x3 GetCurrent9f(uint attrib) const                                    { return Get9f(attrib, GetCurrentPosition()); }
	void GetCurrent16f(uint attrib, Matrix4x4 &m) const                          { Get16f(attrib, GetCurrentPosition(), m); }
	Matrix4x4 GetCurrent16f(uint attrib) const                                   { return Get16f(attrib, GetCurrentPosition()); }

	/**
	 * Sets a vertex's color attribute.
	 * @param index the vertex to set
	 * @param color the new color attribute to set
	 */
	void SetColor(uint index, const Color &color);

	/**
	 * Sets a vertex's color attribute.
	 * @param index the vertex to set
	 * @param r red component of the color attribute to set
	 * @param g green component of the color attribute to set
	 * @param b blue component of the color attribute to set
	 */
	void SetColor(uint index, float r, float g, float b);

	/**
	 * Sets a vertex's color attribute.
	 * @param index the vertex to set
	 * @param r red component of the color attribute to set
	 * @param g green component of the color attribute to set
	 * @param b blue component of the color attribute to set
	 * @param a alpha component of the color attribute to set
	 */
	void SetColor(uint index, float r, float g, float b, float a);

	/**
	 * Sets a vertex's 3D position attribute.
	 * @param index the vertex to set
	 * @param position the new 3D position attribute to set
	 */
	void SetPosition3(uint index, const Vector3 &position);

	/**
	 * Sets a vertex's 3D position attribute.
	 * @param index the vertex to set
	 * @param x X coordinate of the 3D position attribute to set
	 * @param y Y coordinate of the 3D position attribute to set
	 * @param z Z coordinate of the 3D position attribute to set
	 */
	void SetPosition3(uint index, float x, float y, float z);

	/**
	 * Sets a vertex's 2D position attribute.
	 * @param index the vertex to set
	 * @param position the new 2D position attribute to set
	 */
	void SetPosition2(uint index, const Vector2 &position);

	/**
	 * Sets a vertex's 2D position attribute.
	 * @param index the vertex to set
	 * @param x X coordinate of the 2D position attribute to set
	 * @param y Y coordinate of the 2D position attribute to set
	 */
	void SetPosition2(uint index, float x, float y);

	/**
	 * Sets a vertex's normal attribute.
	 * @param index the vertex to set
	 * @param position the new normal attribute to set
	 */
	void SetNormal(uint index, const Vector3 &normal);

	/**
	 * Sets a vertex's normal attribute.
	 * @param index the vertex to set
	 * @param x X coordinate of the normal attribute to set
	 * @param y Y coordinate of the normal attribute to set
	 * @param z Z coordinate of the normal attribute to set
	 */
	void SetNormal(uint index, float x, float y, float z);

	/**
	 * Sets a vertex's texture coordinate attribute.
	 * @param index the vertex to set
	 * @param texCoord the new texture coordinate attribute to set
	 */
	void SetTexCoord(uint index, const Vector2 &texCoord);

	/**
	 * Sets a vertex's texture coordinate attribute.
	 * @param index the vertex to set
	 * @param x U coordinate of the texture coordinate attribute to set
	 * @param y V coordinate of the texture coordinate attribute to set
	 */
	void SetTexCoord(uint index, float x, float y);

	void Set1f(uint attrib, uint index, float x);
	void Set2f(uint attrib, uint index, float x, float y);
	void Set2f(uint attrib, uint index, const Vector2 &v);
	void Set3f(uint attrib, uint index, float x, float y, float z);
	void Set3f(uint attrib, uint index, const Vector3 &v);
	void Set4f(uint attrib, uint index, float x, float y, float z, float w);
	void Set4f(uint attrib, uint index, const Color &c);
	void Set9f(uint attrib, uint index, const Matrix3x3 &m);
	void Set16f(uint attrib, uint index, const Matrix4x4 &m);

	/**
	 * Sets the current vertex's color attribute.
	 * @param color the new color attribute to set
	 */
	void SetCurrentColor(const Color &color)                                    { SetColor(GetCurrentPosition(), color); }

	/**
	 * Sets the current vertex's color attribute.
	 * @param r red component of the color attribute to set
	 * @param g green component of the color attribute to set
	 * @param b blue component of the color attribute to set
	 */
	void SetCurrentColor(float r, float g, float b)                             { SetColor(GetCurrentPosition(), r, g, b); }

	/**
	 * Sets the current vertex's color attribute.
	 * @param r red component of the color attribute to set
	 * @param g green component of the color attribute to set
	 * @param b blue component of the color attribute to set
	 * @param a alpha component of the color attribute to set
	 */
	void SetCurrentColor(float r, float g, float b, float a)                    { SetColor(GetCurrentPosition(), r, g, b, a); }

	/**
	 * Sets the current vertex's 3D position attribute.
	 * @param position the new 3D position attribute to set
	 */
	void SetCurrentPosition3(const Vector3 &position)                           { SetPosition3(GetCurrentPosition(), position); }

	/**
	 * Sets the current vertex's 3D position attribute.
	 * @param x X coordinate of the 3D position attribute to set
	 * @param y Y coordinate of the 3D position attribute to set
	 * @param z Z coordinate of the 3D position attribute to set
	 */
	void SetCurrentPosition3(float x, float y, float z)                         { SetPosition3(GetCurrentPosition(), x, y, z); }

	/**
	 * Sets the current vertex's 2D position attribute.
	 * @param position the new 2D position attribute to set
	 */
	void SetCurrentPosition2(const Vector2 &position)                           { SetPosition2(GetCurrentPosition(), position); }

	/**
	 * Sets the current vertex's 2D position attribute.
	 * @param x X coordinate of the 2D position attribute to set
	 * @param y Y coordinate of the 2D position attribute to set
	 */
	void SetCurrentPosition2(float x, float y)                                  { SetPosition2(GetCurrentPosition(), x, y); }

	/**
	 * Sets the current vertex's normal attribute.
	 * @param position the new normal attribute to set
	 */
	void SetCurrentNormal(const Vector3 &normal)                                { SetNormal(GetCurrentPosition(), normal); }

	/**
	 * Sets the current vertex's normal attribute.
	 * @param x X coordinate of the normal attribute to set
	 * @param y Y coordinate of the normal attribute to set
	 * @param z Z coordinate of the normal attribute to set
	 */
	void SetCurrentNormal(float x, float y, float z)                            { SetNormal(GetCurrentPosition(), x, y, z); }

	/**
	 * Sets the current vertex's texture coordinate attribute.
	 * @param texCoord the new texture coordinate attribute to set
	 */
	void SetCurrentTexCoord(const Vector2 &texCoord)                            { SetTexCoord(GetCurrentPosition(), texCoord); }

	/**
	 * Sets the current vertex's texture coordinate attribute.
	 * @param x U coordinate of the texture coordinate attribute to set
	 * @param y V coordinate of the texture coordinate attribute to set
	 */
	void SetCurrentTexCoord(float x, float y)                                   { SetTexCoord(GetCurrentPosition(), x, y); }

	void SetCurrent1f(uint attrib, float x)                                     { Set1f(attrib, GetCurrentPosition(), x); }
	void SetCurrent2f(uint attrib, float x, float y)                            { Set2f(attrib, GetCurrentPosition(), x, y); }
	void SetCurrent2f(uint attrib, const Vector2 &v)                            { Set2f(attrib, GetCurrentPosition(), v); }
	void SetCurrent3f(uint attrib, float x, float y, float z)                   { Set3f(attrib, GetCurrentPosition(), x, y, z); }
	void SetCurrent3f(uint attrib, const Vector3 &v)                            { Set3f(attrib, GetCurrentPosition(), v); }
	void SetCurrent4f(uint attrib, float x, float y, float z, float w)          { Set4f(attrib, GetCurrentPosition(), x, y, z, w); }
	void SetCurrent4f(uint attrib, const Color &c)                              { Set4f(attrib, GetCurrentPosition(), c); }
	void SetCurrent9f(uint attrib, const Matrix3x3 &m)                          { Set9f(attrib, GetCurrentPosition(), m); }
	void SetCurrent16f(uint attrib, const Matrix4x4 &m)                         { Set16f(attrib, GetCurrentPosition(), m); }

	/**
	 * Moves the current vertex position to the next position.
	 * @return TRUE if the move succeeded, FALSE if there is no more vertices
	 *              to move to after this one
	 */
	BOOL MoveNext();

	/**
	 * Moves the current vertex position to the previous position.
	 * @return TRUE if the move succeeded, FALSE if there is no more vertices
	 *              to move to before the current one
	 */
	BOOL MovePrevious();

	/**
	 * Moves the current vertex position by the specified amount relative
	 * to the current position.
	 * @param numVertices the number of vertices to move. If this amount is
	 *                    greater then the remaining space left in the same
	 *                    direction, then it will be shrunk so as not to move
	 *                    out of bounds.
	 */
	void Move(int numVertices);

	/**
	 * Moves the current vertex position to the beginning of the buffer.
	 */
	void MoveToStart()                                                          { m_currentVertex = 0; }

	/**
	 * Moves the current vertex position to the end of the buffer.
	 */
	void MoveToEnd()                                                            { m_currentVertex = GetNumElements() - 1; }

	/**
	 * Moves the current vertex position to the position specified.
	 * @param index the position to move to
	 */
	void MoveTo(uint index)                                                 { m_currentVertex = index; }

	/**
	 * Resizes the buffer capacity to hold the specified number of vertices.
	 * @param numVertices the amount of vertices the buffer should be resized 
	 *                   to hold
	 */
	void Resize(uint numVertices);

	/**
	 * Extends the buffer capacity to hold the specified number of extra vertices.
	 * @param amount the amount of vertices to extend the buffer by
	 */
	void Extend(uint amount);

	/**
	 * Copies vertices from a source buffer to this one. The copied vertices
	 * will be placed in this buffer beginning at the provided offset.
	 * @param source the source buffer to copy from
	 * @param destIndex the index of the vertex position in this buffer to
	 *                  start copying the vertices to
	 */
	void Copy(const VertexBuffer *source, uint destIndex);

	/**
	 * @return the number of vertices contained in this buffer
	 */
	uint GetNumElements() const                                                 { return m_numVertices; }

	/**
	 * @return the size in bytes of each vertex in this buffer object
	 */
	size_t GetElementWidthInBytes() const                                       { return m_elementWidth * sizeof(float); }

	/**
	 * @return pointer to this buffer object's raw data
	 */
	const void* GetBuffer() const                                               { return &m_buffer[0]; }

	/**
	 * @return the current position in the buffer
	 */
	uint GetCurrentPosition() const                                             { return m_currentVertex; }

	/**
	 * @return the number of vertex spaces left between the current position
	 *         and the end of the buffer
	 */
	uint GetRemainingSpace() const                                              { return (GetNumElements() - 1) - GetCurrentPosition(); }

private:
	BOOL SetSizesAndOffsets(const VERTEX_ATTRIBS *attributes, uint numAttributes);
	
	BOOL IsBufferAllocated() const                                              { return m_buffer.size() > 0; }

	uint GetVertexPosition(uint index) const                                    { return (index * m_elementWidth); }
	uint GetColorBufferPosition(uint index) const                               { return GetVertexPosition(index) + m_colorOffset; }
	uint GetPosition3BufferPosition(uint index) const                           { return GetVertexPosition(index) + m_position3Offset; }
	uint GetPosition2BufferPosition(uint index) const                           { return GetVertexPosition(index) + m_position2Offset; }
	uint GetNormalBufferPosition(uint index) const                              { return GetVertexPosition(index) + m_normalOffset; }
	uint GetTexCoordBufferPosition(uint index) const                            { return GetVertexPosition(index) + m_texCoordOffset; }
	uint GetGenericBufferPosition(uint attrib, uint index) const                { return GetVertexPosition(index) + m_attribs[attrib].offset; }

	uint m_numVertices;
	uint m_currentVertex;
	uint m_standardTypeAttribs;

	// the below are all in terms of floats. multiply by sizeof(float) to get in bytes!
	uint m_elementWidth;
	uint m_colorOffset;
	uint m_position3Offset;
	uint m_position2Offset;
	uint m_normalOffset;
	uint m_texCoordOffset;
	// ---

	VertexBufferAttribute *m_attribs;
	uint m_numAttributes;
	uint m_numGPUAttributeSlotsUsed;
	stl::vector<float> m_buffer;
};

inline Color VertexBuffer::GetColor(uint index) const
{
	uint p = GetColorBufferPosition(index);
	return Color(
		m_buffer[p], 
		m_buffer[p + 1], 
		m_buffer[p + 2], 
		m_buffer[p + 3]
		);
}

inline Vector3 VertexBuffer::GetPosition3(uint index) const
{
	uint p = GetPosition3BufferPosition(index);
	return Vector3(
		m_buffer[p], 
		m_buffer[p + 1], 
		m_buffer[p + 2]
		);
}

inline Vector2 VertexBuffer::GetPosition2(uint index) const
{
	uint p = GetPosition2BufferPosition(index);
	return Vector2(
		m_buffer[p], 
		m_buffer[p + 1]
		);
}

inline Vector3 VertexBuffer::GetNormal(uint index) const
{
	uint p = GetNormalBufferPosition(index);
	return Vector3(
		m_buffer[p], 
		m_buffer[p + 1], 
		m_buffer[p + 2]
		);
}

inline Vector2 VertexBuffer::GetTexCoord(uint index) const
{
	uint p = GetTexCoordBufferPosition(index);
	return Vector2(
		m_buffer[p], 
		m_buffer[p + 1]
		);
}

inline void VertexBuffer::Get1f(uint attrib, uint index, float &x) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	x = m_buffer[p];
}

inline float VertexBuffer::Get1f(uint attrib, uint index) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	return m_buffer[p];
}

inline void VertexBuffer::Get2f(uint attrib, uint index, float &x, float &y) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	x = m_buffer[p];
	y = m_buffer[p + 1];
}

inline void VertexBuffer::Get2f(uint attrib, uint index, Vector2 &v) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	v.x = m_buffer[p];
	v.y = m_buffer[p + 1];
}

inline Vector2 VertexBuffer::Get2f(uint attrib, uint index) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	return Vector2(
		m_buffer[p], 
		m_buffer[p + 1]
		);
}

inline void VertexBuffer::Get3f(uint attrib, uint index, float &x, float &y, float &z) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	x = m_buffer[p];
	y = m_buffer[p + 1];
	z = m_buffer[p + 2];
}

inline void VertexBuffer::Get3f(uint attrib, uint index, Vector3 &v) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	v.x = m_buffer[p];
	v.y = m_buffer[p + 1];
	v.z = m_buffer[p + 2];
}

inline Vector3 VertexBuffer::Get3f(uint attrib, uint index) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	return Vector3(
		m_buffer[p], 
		m_buffer[p + 1], 
		m_buffer[p + 2]
		);
}

inline void VertexBuffer::Get4f(uint attrib, uint index, float &x, float &y, float &z, float &w) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	x = m_buffer[p];
	y = m_buffer[p + 1];
	z = m_buffer[p + 2];
	w = m_buffer[p + 3];
}

inline void VertexBuffer::Get4f(uint attrib, uint index, Color &c) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	c.r = m_buffer[p];
	c.g = m_buffer[p + 1];
	c.b = m_buffer[p + 2];
	c.a = m_buffer[p + 3];
}

inline Color VertexBuffer::Get4f(uint attrib, uint index) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	return Color(
		m_buffer[p], 
		m_buffer[p + 1], 
		m_buffer[p + 2], 
		m_buffer[p + 3]
		);
}

inline void VertexBuffer::Get9f(uint attrib, uint index, Matrix3x3 &m) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	void *dest = m.m;
	const void *src = &m_buffer[p];
	memcpy(dest, src, sizeof(float) * 9);
}

inline Matrix3x3 VertexBuffer::Get9f(uint attrib, uint index) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	Matrix3x3 m;
	void *dest = m.m;
	const void *src = &m_buffer[p];
	memcpy(dest, src, sizeof(float) * 9);
	return m;
}

inline void VertexBuffer::Get16f(uint attrib, uint index, Matrix4x4 &m) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	void *dest = m.m;
	const void *src = &m_buffer[p];
	memcpy(dest, src, sizeof(float) * 16);
}

inline Matrix4x4 VertexBuffer::Get16f(uint attrib, uint index) const
{
	uint p = GetGenericBufferPosition(attrib, index);
	Matrix4x4 m;
	void *dest = m.m;
	const void *src = &m_buffer[p];
	memcpy(dest, src, sizeof(float) * 16);
	return m;
}

inline void VertexBuffer::SetColor(uint index, const Color &color)
{
	uint p = GetColorBufferPosition(index);
	m_buffer[p] = color.r;
	m_buffer[p + 1] = color.g;
	m_buffer[p + 2] = color.b;
	m_buffer[p + 3] = color.a;
	SetDirty();
}

inline void VertexBuffer::SetColor(uint index, float r, float g, float b)
{
	uint p = GetColorBufferPosition(index);
	m_buffer[p] = r;
	m_buffer[p + 1] = g;
	m_buffer[p + 2] = b;
	m_buffer[p + 3] = COLOR_ALPHA_OPAQUE;
	SetDirty();
}

inline void VertexBuffer::SetColor(uint index, float r, float g, float b, float a)
{
	uint p = GetColorBufferPosition(index);
	m_buffer[p] = r;
	m_buffer[p + 1] = g;
	m_buffer[p + 2] = b;
	m_buffer[p + 3] = a;
	SetDirty();
}

inline void VertexBuffer::SetPosition3(uint index, const Vector3 &position)
{
	uint p = GetPosition3BufferPosition(index);
	m_buffer[p] = position.x;
	m_buffer[p + 1] = position.y;
	m_buffer[p + 2] = position.z;
	SetDirty();
}

inline void VertexBuffer::SetPosition3(uint index, float x, float y, float z)
{
	uint p = GetPosition3BufferPosition(index);
	m_buffer[p] = x;
	m_buffer[p + 1] = y;
	m_buffer[p + 2] = z;
	SetDirty();
}

inline void VertexBuffer::SetPosition2(uint index, const Vector2 &position)
{
	uint p = GetPosition2BufferPosition(index);
	m_buffer[p] = position.x;
	m_buffer[p + 1] = position.y;
	SetDirty();
}

inline void VertexBuffer::SetPosition2(uint index, float x, float y)
{
	uint p = GetPosition2BufferPosition(index);
	m_buffer[p] = x;
	m_buffer[p + 1] = y;
	SetDirty();
}

inline void VertexBuffer::SetNormal(uint index, const Vector3 &normal)
{
	uint p = GetNormalBufferPosition(index);
	m_buffer[p] = normal.x;
	m_buffer[p + 1] = normal.y;
	m_buffer[p + 2] = normal.z;
	SetDirty();
}

inline void VertexBuffer::SetNormal(uint index, float x, float y, float z)
{
	uint p = GetNormalBufferPosition(index);
	m_buffer[p] = x;
	m_buffer[p + 1] = y;
	m_buffer[p + 2] = z;
	SetDirty();
}

inline void VertexBuffer::SetTexCoord(uint index, const Vector2 &texCoord)
{
	uint p = GetTexCoordBufferPosition(index);
	m_buffer[p] = texCoord.x;
	m_buffer[p + 1] = texCoord.y;
	SetDirty();
}

inline void VertexBuffer::SetTexCoord(uint index, float x, float y)
{
	uint p = GetTexCoordBufferPosition(index);
	m_buffer[p] = x;
	m_buffer[p + 1] = y;
	SetDirty();
}

inline void VertexBuffer::Set1f(uint attrib, uint index, float x)
{
	uint p = GetGenericBufferPosition(attrib, index);
	m_buffer[p] = x;
	SetDirty();
}

inline void VertexBuffer::Set2f(uint attrib, uint index, float x, float y)
{
	uint p = GetGenericBufferPosition(attrib, index);
	m_buffer[p] = x;
	m_buffer[p + 1] = y;
	SetDirty();
}

inline void VertexBuffer::Set2f(uint attrib, uint index, const Vector2 &v)
{
	uint p = GetGenericBufferPosition(attrib, index);
	m_buffer[p] = v.x;
	m_buffer[p + 1] = v.y;
	SetDirty();
}

inline void VertexBuffer::Set3f(uint attrib, uint index, float x, float y, float z)
{
	uint p = GetGenericBufferPosition(attrib, index);
	m_buffer[p] = x;
	m_buffer[p + 1] = y;
	m_buffer[p + 2] = z;
	SetDirty();
}

inline void VertexBuffer::Set3f(uint attrib, uint index, const Vector3 &v)
{
	uint p = GetGenericBufferPosition(attrib, index);
	m_buffer[p] = v.x;
	m_buffer[p + 1] = v.y;
	m_buffer[p + 2] = v.z;
	SetDirty();
}

inline void VertexBuffer::Set4f(uint attrib, uint index, float x, float y, float z, float w)
{
	uint p = GetGenericBufferPosition(attrib, index);
	m_buffer[p] = x;
	m_buffer[p + 1] = y;
	m_buffer[p + 2] = z;
	m_buffer[p + 3] = w;
	SetDirty();
}

inline void VertexBuffer::Set4f(uint attrib, uint index, const Color &c)
{
	uint p = GetGenericBufferPosition(attrib, index);
	m_buffer[p] = c.r;
	m_buffer[p + 1] = c.g;
	m_buffer[p + 2] = c.b;
	m_buffer[p + 3] = c.a;
	SetDirty();
}

inline void VertexBuffer::Set9f(uint attrib, uint index, const Matrix3x3 &m)
{
	uint p = GetGenericBufferPosition(attrib, index);
	void *dest = &m_buffer[p];
	const void *src = m.m;
	memcpy(dest, src, sizeof(float) * 9);
	SetDirty();
}

inline void VertexBuffer::Set16f(uint attrib, uint index, const Matrix4x4 &m)
{
	uint p = GetGenericBufferPosition(attrib, index);
	void *dest = &m_buffer[p];
	const void *src = m.m;
	memcpy(dest, src, sizeof(float) * 16);
	SetDirty();
}

inline BOOL VertexBuffer::MoveNext()
{
	++m_currentVertex;
	if (m_currentVertex >= GetNumElements())
	{
		--m_currentVertex;
		return FALSE;
	}
	else
		return TRUE;
}

inline BOOL VertexBuffer::MovePrevious()
{
	if (m_currentVertex == 0)
		return FALSE;
	else
	{
		--m_currentVertex;
		return TRUE;
	}
}

inline void VertexBuffer::Move(int numVertices)
{
	// m_currentVertex is unsigned, so detect when we would go negative beforehand
	if (numVertices < 0 && (uint)abs(numVertices) > m_currentVertex)
		m_currentVertex = 0;
	else
	{
		m_currentVertex += numVertices;
		if (m_currentVertex >= GetNumElements())
			m_currentVertex = GetNumElements() - 1;
	}
}

#endif
