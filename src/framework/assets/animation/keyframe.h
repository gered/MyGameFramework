#ifndef __FRAMEWORK_ASSETS_ANIMATION_KEYFRAME_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_KEYFRAME_H_INCLUDED__

#include "../../common.h"
#include <stl/string.h>

struct Vector3;

/**
 * Holds vertex and normal information for a single keyframe.
 */
class Keyframe
{
public:
	/**
	 * Creates a new keyframe.
	 * @param numVertices the number of vertices in this keyframe
	 */
	Keyframe(uint numVertices);

	/**
	 * Creates a new keyframe.
	 * @param name the unique name for this keyframe
	 * @param numVertices the number of vertices in this keyframe
	 */
	Keyframe(const stl::string &name, uint numVertices);

	virtual ~Keyframe();

	/**
	 * @return the unique name of this keyframe
	 */
	const stl::string& GetName() const                     { return m_name; }

	/**
	 * @return the number of vertices in this keyframe
	 */
	uint GetNumVertices() const                            { return m_numVertices; }

	/**
	 * @return pointer to the vertices in this keyframe
	 */
	Vector3* GetVertices()                                 { return m_vertices; }

	/**
	 * @return pointer to the normals in this keyframe
	 */
	Vector3* GetNormals()                                  { return m_normals; }

private:
	void AllocateMemory(uint numVertices);

	stl::string m_name;
	uint m_numVertices;
	Vector3 *m_vertices;
	Vector3 *m_normals;
};

#endif
