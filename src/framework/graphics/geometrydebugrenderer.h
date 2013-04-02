#ifndef __FRAMEWORK_GRAPHICS_GEOMETRYDEBUGRENDERER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_GEOMETRYDEBUGRENDERER_H_INCLUDED__

#include "../common.h"

#include "color.h"

class GraphicsDevice;
class RenderState;
class VertexBuffer;
struct BoundingBox;
struct BoundingSphere;
struct LineSegment;
struct Point3;
struct Ray;
struct Vector3;

/**
 * Helper class to render geometric primitives as wireframe shapes 
 * for debugging purposes.
 */
class GeometryDebugRenderer
{
public:
	/**
	 * Creates a geometry debug renderer.
	 * @param graphicsDevice the graphics device to use for rendering
	 * @param depthTesting TRUE to enable depth testing for all geometry rendering
	 */
	GeometryDebugRenderer(GraphicsDevice *graphicsDevice, BOOL depthTesting = FALSE);

	virtual ~GeometryDebugRenderer();

	/**
	 * Begins a rendering block. All rendering should be performed after
	 * this and then completed with a call to End().
	 */
	void Begin();

	/**
	 * Renders a wireframe box.
	 * @param box the box to render
	 */
	void Render(const BoundingBox &box);

	/**
	 * Renders a wireframe box.
	 * @param box the box to render
	 * @param color the color to render with
	 */
	void Render(const BoundingBox &box, const Color &color);

	void Render(const Point3 &boxMin, const Point3 &boxMax);
	void Render(const Point3 &boxMin, const Point3 &boxMax, const Color &color);

	/**
	 * Renders a wireframe sphere.
	 * @param sphere the sphere to render
	 */
	void Render(const BoundingSphere &sphere);

	/**
	 * Renders a wireframe sphere.
	 * @param sphere the sphere to render
	 * @param color the color to render with
	 */
	void Render(const BoundingSphere &sphere, const Color &color);

	/**
	 * Renders a wireframe ray up to a given length along the ray's direction.
	 * @param ray the ray to render
	 * @param length the length along the ray's direction to render
	 */
	void Render(const Ray &ray, float length);

	/**
	 * Renders a wireframe ray up to a given length along the ray's direction.
	 * @param ray the ray to render
	 * @param length the length along the ray's direction to render
	 * @param color1 the color to render the ray's origin with
	 * @param color2 the color to render the end point with
	 */
	void Render(const Ray &ray, float length, const Color &color1, const Color &color2);

	/**
	 * Renders a wireframe line segment.
	 * @param line the line segment to render
	 */
	void Render(const LineSegment &line);

	/**
	 * Renders a wireframe line segment.
	 * @param line the line segment to render
	 * @param color the color to render with
	 */
	void Render(const LineSegment &line, const Color &color);

	/**
	 * Renders a wireframe triangle.
	 * @param a the first point of the triangle
	 * @param b the second point of the triangle
	 * @param c the third point of the triangle
	 */
	void Render(const Vector3 &a, const Vector3 &b, const Vector3 &c);

	/**
	 * Renders a wireframe triangle.
	 * @param a the first point of the triangle
	 * @param b the second point of the triangle
	 * @param c the third point of the triangle
	 * @param color the color to render with
	 */
	void Render(const Vector3 &a, const Vector3 &b, const Vector3 &c, const Color &color);

	/**
	 * Ends a rendering block, flushing out all Render() calls made
	 * to the video card.
	 */
	void End();

private:
	GraphicsDevice *m_graphicsDevice;
	RenderState *m_renderState;
	Color m_color1;
	Color m_color2;
	VertexBuffer *m_vertices;
	uint m_currentVertex;

	BOOL m_begunRendering;
};

inline void GeometryDebugRenderer::Render(const BoundingBox &box)
{
	Render(box, m_color1);
}

inline void GeometryDebugRenderer::Render(const Point3 &boxMin, const Point3 &boxMax)
{
	Render(boxMin, boxMax, m_color1);
}

inline void GeometryDebugRenderer::Render(const BoundingSphere &sphere)
{
	Render(sphere, m_color1);
}

inline void GeometryDebugRenderer::Render(const Ray &ray, float length)
{
	Render(ray, length, m_color1, m_color2);
}

inline void GeometryDebugRenderer::Render(const LineSegment &line)
{
	Render(line, m_color1);
}

inline void GeometryDebugRenderer::Render(const Vector3 &a, const Vector3 &b, const Vector3 &c)
{
	Render(a, b, c, m_color1);
}

#endif
