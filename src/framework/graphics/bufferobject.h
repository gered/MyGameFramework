#ifndef __FRAMEWORK_GRAPHICS_BUFFEROBJECT_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_BUFFEROBJECT_H_INCLUDED__

#include "../common.h"
#include "graphicscontextresource.h"

class GraphicsDevice;

enum BUFFEROBJECT_TYPE
{
	BUFFEROBJECT_TYPE_VERTEX = 0,
	BUFFEROBJECT_TYPE_INDEX = 1
};

enum BUFFEROBJECT_USAGE
{
	BUFFEROBJECT_USAGE_STATIC = 0,
	BUFFEROBJECT_USAGE_STREAM = 1,
	BUFFEROBJECT_USAGE_DYNAMIC = 2
};

/**
 * Base class for buffer objects that are able to store their data in a
 * buffer object in video memory.
 */
class BufferObject : public GraphicsContextResource
{
public:
	/**
	 * Initializes buffer object handling state
	 * @param type the type of buffer object describing the kind of data that
	 *             will be stored in it
	 * @param usage the expected usage pattern of this buffer object
	 */
	BufferObject(BUFFEROBJECT_TYPE type, BUFFEROBJECT_USAGE usage);

	/**
	 * Initializes buffer object handling state
	 * @param graphicsDevice the graphics device this buffer object is for
	 * @param type the type of buffer object describing the kind of data that
	 *             will be stored in it
	 * @param usage the expected usage pattern of this buffer object
	 */
	BufferObject(GraphicsDevice *graphicsDevice, BUFFEROBJECT_TYPE type, BUFFEROBJECT_USAGE usage);

	virtual ~BufferObject();

	/**
	 * @return TRUE if this buffer object is holding client side data only
	 *              (hasn't been created as a buffer object in video memory)
	 */
	BOOL IsClientSideBuffer() const                        { return m_bufferId == 0; }

	/**
	 * @return the type of data this buffer holds
	 */
	BUFFEROBJECT_TYPE GetType() const                      { return m_type; }

	/**
	 * @return the expected usage pattern of this buffer object
	 */
	BUFFEROBJECT_USAGE GetUsage() const                    { return m_usage; }

	/**
	 * @return the size in bytes of this buffer object (including it's data)
	 */
	size_t GetSizeInBytes() const                          { return m_sizeInBytes; }

	/**
	 * @return the number of elements contained in this buffer object
	 */
	virtual uint32_t GetNumElements() const = 0;

	/**
	 * @return the size in bytes of each element of data in this buffer object
	 */
	virtual size_t GetElementWidthInBytes() const = 0;

	/**
	 * @return TRUE if some or all of the buffer data has been changed since
	 *              the last Update() call
	 */
	BOOL IsDirty() const                                   { return m_isDirty; }

	/**
	 * Uploads the current buffer data to video memory.
	 */
	void Update();

	/**
	 * @return pointer to this buffer object's raw data
	 */
	virtual const void* GetBuffer() const = 0;

	/**
	 * @return the OpenGL buffer object ID for this buffer
	 */
	uint32_t GetBufferId() const                           { return m_bufferId; }

	void OnNewContext();
	void OnLostContext();

protected:
	/**
	 * Creates a buffer object on the GPU but does not upload this 
	 * buffer's data there yet. This only allocates the buffer object in
	 * video memory.
	 */
	void CreateOnGpu();
	
	/**
	 * Recreates the buffer object the GPU but does not upload this
	 * buffer's data there yet. This only reallocates the buffer object in
	 * video memory.
	 */
	void RecreateOnGpu();
	
	/**
	 * Frees the buffer object from video memory.
	 */
	void FreeFromGpu();

	void CreateBufferObject();
	void FreeBufferObject();
	void SizeBufferObject();
	void SetDirty()                                        { m_isDirty = TRUE; }

private:

	BUFFEROBJECT_TYPE m_type;
	BUFFEROBJECT_USAGE m_usage;
	uint32_t m_bufferId;
	BOOL m_isDirty;
	size_t m_sizeInBytes;
};

#endif
