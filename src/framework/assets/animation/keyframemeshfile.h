#ifndef __FRAMEWORK_ASSETS_ANIMATION_KEYFRAMEMESHFILE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_KEYFRAMEMESHFILE_H_INCLUDED__

#include "../../common.h"
#include <stl/string.h>
#include "../meshfile.h"
#include "../../support/animationsequence.h"

class File;
class Keyframe;
struct KeyframeMeshTriangle;
struct Vector2;

/**
 * A keyframe mesh file which is loaded using a custom "chunked" file format.
 */
class KeyframeMeshFile : public MeshFile
{
public:
	/**
	 * Creates a keyframe mesh file object by loading the mesh information
	 * from the provided file.
	 * @param file the file that will be loaded as a keyframe mesh
	 */
	KeyframeMeshFile(File *file);

	virtual ~KeyframeMeshFile();

	/**
	 * @return the total number of keyframes this mesh contains
	 */
	uint32_t GetNumFrames() const                          { return m_numFrames; }

	/**
	 * @return pointer to the mesh's keyframes
	 */
	Keyframe** GetFrames() const                           { return m_frames; }

	/**
	 * @return the total number of texture coordinates this mesh contains
	 */
	uint32_t GetNumTexCoords() const                       { return m_numTexCoords; }

	/**
	 * @return pointer to the mesh's texture coordinates
	 */
	Vector2* GetTexCoords() const                          { return m_texCoords; }

	/**
	 * @return the total number of triangles this mesh contains
	 */
	uint32_t GetNumTriangles() const                       { return m_numTriangles; }

	/**
	 * @return pointer to the mesh's triangles
	 */
	KeyframeMeshTriangle* GetTriangles() const             { return m_triangles; }

	/**
	 * @return the mesh's animation sequences
	 */
	const AnimationList& GetAnimations() const             { return m_animations; }

	/**
	 * @return the number of vertices each keyframe contains
	 */
	uint32_t GetNumVerticesPerFrame() const                { return m_numVerticesPerFrame; }

private:
	void Load();

	uint32_t m_numFrames;
	Keyframe **m_frames;
	uint32_t m_numTexCoords;
	Vector2 *m_texCoords;
	uint32_t m_numTriangles;
	KeyframeMeshTriangle *m_triangles;
	AnimationList m_animations;
	uint32_t m_numVerticesPerFrame;
};

#endif
