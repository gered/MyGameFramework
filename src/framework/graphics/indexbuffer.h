#ifndef __FRAMEWORK_GRAPHICS_INDEXBUFFER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_INDEXBUFFER_H_INCLUDED__

#include "../common.h"
#include "bufferobject.h"

#include <stdlib.h>        // for abs()
#include <stl/vector.h>

class GraphicsDevice;

/**
 * Wraps management of an array of vertex indexes to be used for
 * optimized rendering of vertices.
 */
class IndexBuffer : public BufferObject
{
public:
	IndexBuffer();
	virtual ~IndexBuffer()                                                      { Release(); }
	
	/**
	 * Releases all resources associated with this index buffer.
	 */
	virtual void Release();
	
	/**
	 * Initializes the index buffer.
	 * @param numIndices the initial number of indices the buffer should hold
	 * @param usage the expected usage pattern of this index buffer
	 * @return TRUE if successful, FALSE if not
	 */
	BOOL Initialize(uint32_t numIndices, BUFFEROBJECT_USAGE usage);
	
	/**
	 * Initializes the index buffer.
	 * @param graphicsDevice the graphics device to use to create this buffer
	 *                       on the GPU
	 * @param numIndices the initial number of indices the buffer should hold
	 * @param usage the expected usage pattern of this index buffer
	 * @return TRUE if successful, FALSE if not
	 */
	BOOL Initialize(GraphicsDevice *graphicsDevice, uint32_t numIndices, BUFFEROBJECT_USAGE usage);
	
	/**
	 * Initializes the index buffer.
	 * @param source the source buffer to copy during creation of this buffer
	 * @return TRUE if successful, FALSE if not
	 */
	BOOL Initialize(const IndexBuffer *source);
	
	/**
	 * Initializes the index buffer.
	 * @param graphicsDevice the graphics device to use to create this buffer
	 *                       on the GPU
	 * @param source the source buffer to copy during creation of this buffer
	 * @return TRUE if successful, FALSE if not
	 */
	BOOL Initialize(GraphicsDevice *graphicsDevice, const IndexBuffer *source);

	/**
	 * Copies the source indices over top of this buffer's existing indices.
	 * @param indices the source index data to copy
	 * @param numIndices the number of indices to copy from the source
	 */
	void Set(const uint16_t *indices, uint32_t numIndices);

	/**
	 * Sets an index to a new value.
	 * @param index the position of the index in this buffer to set
	 * @param value the new value to set
	 */
	void SetIndex(uint32_t index, uint16_t value);

	/**
	 * Moves the current index position to the next position.
	 * @return TRUE if the move succeeded, FALSE if there is no more indices
	 *              to move to after this one
	 */
	BOOL MoveNext();

	/**
	 * Moves the current index position to the previous position.
	 * @return TRUE if the move succeeded, FALSE if there is no more indices
	 *              to move to before the current one
	 */
	BOOL MovePrevious();

	/**
	 * Moves the current index position by the specified amount relative
	 * to the current position.
	 * @param numIndices the number of indices to move. If this amount is
	 *                   greater then the remaining space left in the same
	 *                   direction, then it will be shrunk so as not to move
	 *                   out of bounds.
	 */
	void Move(int32_t numIndices);

	/**
	 * Moves the current index position to the beginning of the buffer.
	 */
	void MoveToStart()                                                          { m_currentIndex = 0; }

	/**
	 * Moves the current index position to the end of the buffer.
	 */
	void MoveToEnd()                                                            { m_currentIndex = GetNumElements() - 1; }

	/**
	 * Moves the current index position to the position specified.
	 * @param index the position to move to
	 */
	void MoveTo(uint32_t index)                                                 { m_currentIndex = index; }

	/**
	 * Sets the index at the current position to a new value.
	 * @param index the new value to set
	 */
	void SetCurrent(uint16_t index)                                             { SetIndex(m_currentIndex, index); }

	/**
	 * Resizes the buffer capacity to hold the specified number of indices.
	 * @param numIndices the amount of indices the buffer should be resized 
	 *                   to hold
	 */
	void Resize(uint32_t numIndices);

	/**
	 * Extends the buffer capacity to hold the specified number of extra indices.
	 * @param amount the amount of indices to extend the buffer by
	 */
	void Extend(uint32_t amount);

	/**
	 * @return the number of indices contained in this buffer
	 */
	uint32_t GetNumElements() const                                             { return m_buffer.size(); }

	/**
	 * @return the size in bytes of each index in this buffer object
	 */
	size_t GetElementWidthInBytes() const                                       { return sizeof(uint16_t); }

	/**
	 * @return pointer to this buffer object's raw data
	 */
	const void* GetBuffer() const                                               { return &m_buffer[0]; }

	/**
	 * @return the current position in the buffer
	 */
	uint32_t GetCurrentPosition() const                                         { return m_currentIndex; }

	/**
	 * @return the number of index spaces left between the current position
	 *         and the end of the buffer
	 */
	uint32_t GetRemainingSpace() const                                          { return (GetNumElements() - 1) - GetCurrentPosition(); }

private:
	stl::vector<uint16_t> m_buffer;

	uint32_t m_currentIndex;
};

inline void IndexBuffer::SetIndex(uint32_t index, uint16_t value)
{
	m_buffer[index] = value;
}

inline BOOL IndexBuffer::MoveNext()
{
	++m_currentIndex;
	if (m_currentIndex >= GetNumElements())
	{
		--m_currentIndex;
		return FALSE;
	}
	else
		return TRUE;
}

inline BOOL IndexBuffer::MovePrevious()
{
	if (m_currentIndex == 0)
		return FALSE;
	else
	{
		--m_currentIndex;
		return TRUE;
	}
}

inline void IndexBuffer::Move(int32_t numIndices)
{
	// m_currentIndex is unsigned, so detect when we would go negative beforehand
	if (numIndices < 0 && (uint32_t)abs(numIndices) > m_currentIndex)
		m_currentIndex = 0;
	else
	{
		m_currentIndex += numIndices;
		if (m_currentIndex >= GetNumElements())
			m_currentIndex = GetNumElements() - 1;
	}
}
#endif
