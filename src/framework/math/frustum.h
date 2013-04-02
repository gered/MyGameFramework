#ifndef __FRAMEWORK_MATH_FRUSTUM_H_INCLUDED__
#define __FRAMEWORK_MATH_FRUSTUM_H_INCLUDED__

#include "../common.h"
#include "plane.h"

class ViewContext;
struct BoundingBox;
struct BoundingSphere;

enum FRUSTUM_SIDES
{
	FRUSTUM_RIGHT = 0,
	FRUSTUM_LEFT = 1,
	FRUSTUM_BOTTOM = 2,
	FRUSTUM_TOP = 3,
	FRUSTUM_BACK = 4,
	FRUSTUM_FRONT = 5
};

const int NUM_FRUSTUM_SIDES = 6;

/**
 * A viewing frustum that can be used to test for geometry visibility
 * given the current camera modelview and projection matrices.
 */
class Frustum
{
public:
	/**
	 * Creates a frustum object.
	 * @param viewContext the view context this frustum is associated with
	 */
	Frustum(ViewContext *viewContext);

	virtual ~Frustum();

	/**
	 * Recalculates the viewing frustum from the current camera matrices.
	 */
	void Calculate();

	/**
	 * Tests a point for visibility.
	 * @param point the point to be tested
	 * @return bool true if visible, false if not
	 */
	bool Test(const Vector3 &point) const;

	/**
	 * Tests a box for visibility.
	 * @param box the box to be tested
	 * @return bool true if at least partially visible, false if entirely
	 *              outside the viewing frustum
	 */
	bool Test(const BoundingBox &box) const;

	/**
	 * Tests a sphere for visibility.
	 * @param sphere the sphere to be tested
	 * @return bool true if at least partially visible, false if entirely
	 *              outside the viewing frustum
	 */
	bool Test(const BoundingSphere &sphere) const;

private:
	bool TestPlaneAgainstBox(const Plane &plane, float minX, float minY, float minZ, float width, float height, float depth) const;
	bool TestPlaneAgainstSphere(const Plane &plane, const Vector3 &center, float radius) const;

	ViewContext *m_viewContext;
	Plane m_planes[NUM_FRUSTUM_SIDES];
};

#endif
