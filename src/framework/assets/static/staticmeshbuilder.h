#ifndef __FRAMEWORK_ASSETS_STATIC_STATICMESHBUILDER_H_INCLUDED__
#define __FRAMEWORK_ASSETS_STATIC_STATICMESHBUILDER_H_INCLUDED__

#include "../../common.h"

#include "../../math/matrix4x4.h"

#include <stl/vector.h>

class StaticMesh;
class StaticMeshSubset;
class Texture;
class VertexBuffer;
struct Vector2;
struct Vector3;

/**
 * Helper class to simplify the task of dynamically building up a StaticMesh
 * object via code.
 */
class StaticMeshBuilder
{
public:
	/**
	 * Creates a static mesh builder.
	 */
	StaticMeshBuilder();

	virtual ~StaticMeshBuilder();

	/**
	 * Resets all generated subset information, clearing out all information
	 * so this builder is ready to generate a new mesh from scratch.
	 */
	void Reset();

	/**
	 * Sets a transformation matrix that every subsequently generated vertex
	 * will have applied to it.
	 * @param transform the transformation matrix
	 */
	void SetTransform(const Matrix4x4 &transform)          { m_transform = transform; }

	/**
	 * Adds a new subset to the mesh currently being generated.
	 * @param numTriangles the number of triangles in this subset
	 * @param texture the texture to be applied to this subset, or NULL to use
	 *                no texture with it
	 */
	uint AddSubset(uint numTriangles, Texture *texture);

	/**
	 * Sets a triangle on the specified subset.
	 * @param subsetIndex the index of the subset to set the triangle on
	 * @param triangle the index of the triangle to set in the subset
	 */
	void SetTriangle(
		uint subsetIndex,
		uint triangle,
		const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, 
		const Vector3 &n1, const Vector3 &n2, const Vector3 &n3, 
		const Vector2 &t1, const Vector2 &t2, const Vector2 &t3
		);

	/**
	 * Sets a triangle on the specified subset.
	 * @param subsetIndex the index of the subset to set the triangle on
	 * @param triangle the index of the triangle to set in the subset
	 */
	void SetTriangle(
		uint subsetIndex,
		uint triangle,
		const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, 
		const Vector2 &t1, const Vector2 &t2, const Vector2 &t3
		);

	/**
	 * Sets a quad on the specified subset. This is generated as two
	 * triangles.
	 * @param subsetIndex the index of the subset to set the quad on
	 * @param firstTriangle the index of the first triangle of two that
	 *                      the quad's vertices will be set in
	 */
	void SetQuad(
		uint subsetIndex,
		uint firstTriangle,
		const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4, 
		const Vector3 &n1, const Vector3 &n2, const Vector3 &n3, const Vector3 &n4, 
		const Vector2 &t1, const Vector2 &t2, const Vector2 &t3, const Vector2 &t4
		);

	/**
	 * Sets a quad on the specified subset. This is generated as two
	 * triangles.
	 * @param subsetIndex the index of the subset to set the quad on
	 * @param firstTriangle the index of the first triangle of two that
	 *                      the quad's vertices will be set in
	 */
	void SetQuad(
		uint subsetIndex,
		uint firstTriangle,
		const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, const Vector3 &v4, 
		const Vector2 &t1, const Vector2 &t2, const Vector2 &t3, const Vector2 &t4
		);

	/**
	 * Sets a box on the specified subset. This is generated as 12
	 * triangles.
	 * @param subsetIndex the index of the subset to set the box on
	 * @param firstTriangle the index of the first triangle of two that
	 *                      the box's vertices will be set in
	 */
	void SetBox(
		uint subsetIndex,
		uint firstTriangle,
		const Vector3 &center, float width, float height, float depth
		);

	/**
	 * Sets a box on the specified subset. This is generated as 12
	 * triangles.
	 * @param subsetIndex the index of the subset to set the box on
	 * @param firstTriangle the index of the first triangle of two that
	 *                      the box's vertices will be set in
	 */
	void SetBox(
		uint subsetIndex,
		uint firstTriangle,
		const Vector3 &min, const Vector3 &max
		);
	
	/**
	 * Automatically generates normals for every vertex in all subsets. This
	 * will overwrite any normals that may have been directly provided already.
	 */
	void GenerateNormals();

	/**
	 * Constructs a StaticMesh object, returns it, then frees the subsets from
	 * the builder so that it is ready for use in generating another mesh. The
	 * returned mesh should be manually freed by the calling code when no
	 * longer needed.
	 */
	StaticMesh* BuildMesh();

private:
	void SetTriangleInternal(
		VertexBuffer *buffer, 
		uint bufferIndex,
		const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, 
		const Vector3 &n1, const Vector3 &n2, const Vector3 &n3, 
		const Vector2 &t1, const Vector2 &t2, const Vector2 &t3
		);
	void SetTriangleInternal(
		VertexBuffer *buffer, 
		uint bufferIndex,
		const Vector3 &v1, const Vector3 &v2, const Vector3 &v3, 
		const Vector2 &t1, const Vector2 &t2, const Vector2 &t3
		);

	Matrix4x4 m_transform;
	stl::vector<StaticMeshSubset*> m_subsets;
};

#endif
