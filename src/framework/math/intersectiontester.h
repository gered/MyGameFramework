#ifndef __FRAMEWORK_MATH_INTERSECTIONTESTER_H_INCLUDED__
#define __FRAMEWORK_MATH_INTERSECTIONTESTER_H_INCLUDED__

#include "../common.h"

struct BoundingBox;
struct BoundingSphere;
struct CollisionPacket;
struct Plane;
struct Ray;
struct Vector3;

/**
 * Contains a number of utility methods for testing for collisions
 * between basic geometric entities.
 */
class IntersectionTester
{
public:
	/**
	 * Tests for a collision between a box and a point.
	 * @param box the box to test
	 * @param point the point to test
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const BoundingBox &box, const Vector3 &point);

	/** 
	 * Tests for a collision between a sphere and a point.
	 * @param sphere the sphere to test
	 * @param point the point to test
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const BoundingSphere &sphere, const Vector3 &point);

	/** 
	 * Tests for a collision between a box and a set of points.
	 * @param box the box to test
	 * @param vertices an array of points
	 * @param numVertices the number of points in the array
	 * @param firstIntersection if not NULL, will contain the first intersection
	 *                          point if an intersection is found at all
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const BoundingBox &box, const Vector3 *vertices, uint numVertices, Vector3 *firstIntersection);

	/**
	 * Tests for a collision between a sphere and a set of points.
	 * @param sphere the sphere to test
	 * @param vertices an array of points
	 * @param numVertices the number of points in the array
	 * @param firstIntersection if not NULL, will contain the first intersection
	 *                          point if an intersection is found at all
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const BoundingSphere &sphere, const Vector3 *vertices, uint numVertices, Vector3 *firstIntersection);

	/**
	 * Tests for a collision between two boxes.
	 * @param a the first box to test
	 * @param b the second box to test
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const BoundingBox &a, const BoundingBox &b);

	/**
	 * Tests for a collision between two spheres.
	 * @param a the first sphere to test
	 * @param b the second sphere to test
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const BoundingSphere &a, const BoundingSphere &b);

	/**
	 * Tests for a collision between a sphere and a plane.
	 * @param sphere the sphere to test
	 * @param plane the plane to test
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const BoundingSphere &sphere, const Plane &plane);

	/**
	 * Tests for a collision between a box and a plane.
	 * @param box the box to test
	 * @param plane the plane to test
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const BoundingBox &box, const Plane &plane);

	/**
	 * Tests for a collision between a ray and a plane.
	 * @param ray the ray to test
	 * @param plane the plane to test
	 * @param intersection if not NULL, will contain the point of intersection
	 *                     if found at all
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const Ray &ray, const Plane &plane, Vector3 *intersection);

	/**
	 * Tests for a collision between a ray and a sphere.
	 * @param ray the ray to test
	 * @param sphere the sphere to test
	 * @param intersection if not NULL, will contain the first point of intersection
	 *                     if found at all
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const Ray &ray, const BoundingSphere &sphere, Vector3 *intersection);

	/**
	 * Tests for a collision between a ray and a box.
	 * @param ray the ray to test
	 * @param box the box to test
	 * @param intersection if not NULL, will contain the point of intersection
	 *                     if found at all
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const Ray &ray, const BoundingBox &box, Vector3 *intersection);

	/**
	 * Tests for a collision between a box and a sphere.
	 * @param box the box to test
	 * @param sphere the sphere to test
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const BoundingBox &box, const BoundingSphere &sphere);

	/**
	 * Tests for a collision between a ray and a triangle.
	 * @param ray the ray to test
	 * @param a first point of the triangle to test
	 * @param b second point of the triangle to test
	 * @param c third point of the triangle to test
	 * @param intersection if not NULL, will contain the point of interestion
	 *                     if found at all
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(const Ray &ray, const Vector3 &a, const Vector3 &b, const Vector3 &c, Vector3 *intersection);

	/** 
	 * Tests for collision between a "swept sphere" and a triangle.
	 * @param packet pre-filled packet containing details about the sphere to be tested
	 * @param v1 first point of the triangle to test
	 * @param v2 second point of the triangle to test
	 * @param v3 third point of the triangle to test
	 * @return bool true if a collision is found, false if not
	 */
	static bool Test(CollisionPacket &packet, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3);
};

#endif
