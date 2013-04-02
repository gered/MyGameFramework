#ifndef __FRAMEWORK_ASSETS_ANIMATION_KEYFRAMEMESH_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_KEYFRAMEMESH_H_INCLUDED__

#include "../../common.h"
#include <stl/string.h>
#include "../../support/animationsequence.h"
#include "../../content/content.h"
#include "../../util/typesystem.h"

class Keyframe;
class KeyframeMeshFile;
struct KeyframeMeshTriangle;
struct Vector2;
class VertexBuffer;

/**
 * Keyframe mesh object containing geometry that is animated via
 * interpolation of vertices contained in keyframes.
 */
class KeyframeMesh : public Content
{
public:
	TYPE_DEFINE(CONTENT_TYPE, "KeyframeMesh");

	/**
	 * Creates a keyframe mesh object.
	 * @param file the keyframe mesh file to load the mesh data from
	 */
	KeyframeMesh(const KeyframeMeshFile *file);

	virtual ~KeyframeMesh();

	/**
	 * @return the total number of keyframes contained in this mesh
	 */
	uint GetNumFrames() const                              { return m_numFrames; }

	/**
	 * @return pointer to the mesh's keyframes
	 */
	Keyframe** GetFrames() const                           { return m_frames; }

	/**
	 * @return the total number of texture coordinates contained in this mesh
	 */
	uint GetNumTexCoords() const                           { return m_numTexCoords; }

	/**
	 * @return pointer to the mesh's texture coordinates
	 */
	Vector2* GetTexCoords() const                          { return m_texCoords; }

	/**
	 * @return the total number of triangles contained in this mesh
	 */
	uint GetNumTriangles() const                           { return m_numTriangles; }

	/**
	 * @return pointer to the mesh's triangles
	 */
	KeyframeMeshTriangle* GetTriangles() const             { return m_triangles; }

	/**
	 * @return the number of vertices per keyframe in this mesh
	 */
	uint GetNumVerticesPerFrame() const                    { return m_numVerticesPerFrame; }

	/**
	 * Retrieves an animation sequence.
	 * @param name the name of the sequence to retrieve
	 * @return the matching sequence or NULL if one was not found
	 */
	const AnimationSequence* GetAnimation(const stl::string &name) const;

	/**
	 * @return a list of all of this mesh's the animation sequences.
	 */
	const AnimationList* GetAnimations() const             { return &m_animations; }

	/**
	 * @return the mesh's vertex buffer
	 */
	VertexBuffer* GetVertices() const                      { return m_vertices; }

private:
	uint m_numFrames;
	Keyframe **m_frames;
	uint m_numTexCoords;
	Vector2 *m_texCoords;
	uint m_numTriangles;
	KeyframeMeshTriangle *m_triangles;
	AnimationList m_animations;
	uint m_numVerticesPerFrame;
	VertexBuffer *m_vertices;
};

#endif
