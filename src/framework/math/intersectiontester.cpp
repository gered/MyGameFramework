#include "../debug.h"

#include "intersectiontester.h"

#include <math.h>
#include <cfloat>

#include "collisionpacket.h"
#include "boundingbox.h"
#include "boundingsphere.h"
#include "mathhelpers.h"
#include "plane.h"
#include "ray.h"
#include "vector3.h"

BOOL IntersectionTester::Test(const BoundingBox &box, const Vector3 &point)
{
	STACK_TRACE;
	if ((point.x >= box.min.x && point.x <= box.max.x) &&
		(point.y >= box.min.y && point.y <= box.max.y) && 
		(point.z >= box.min.z && point.z <= box.max.z))
		return TRUE;
	else
		return FALSE;
}

BOOL IntersectionTester::Test(const BoundingSphere &sphere, const Vector3 &point)
{
	STACK_TRACE;
	if (fabsf(Vector3::Distance(point, sphere.center)) < sphere.radius)
		return TRUE;
	else
		return FALSE;
}

BOOL IntersectionTester::Test(const BoundingBox &box, const Vector3 *vertices, int numVertices, Vector3 *firstIntersection)
{
	STACK_TRACE;
	for (int i = 0; i < numVertices; ++i)
	{
		if ((vertices[i].x >= box.min.x && vertices[i].x <= box.max.x) &&
			(vertices[i].y >= box.min.y && vertices[i].y <= box.max.y) && 
			(vertices[i].z >= box.min.z && vertices[i].z <= box.max.z))
		{
			if (firstIntersection)
				*firstIntersection = vertices[i];
			return TRUE;
		}
	}

	return FALSE;
}

BOOL IntersectionTester::Test(const BoundingSphere &sphere, const Vector3 *vertices, int numVertices, Vector3 *firstIntersection)
{
	STACK_TRACE;
	for (int i = 0; i < numVertices; ++i)
	{
		if (fabsf(Vector3::Distance(vertices[i], sphere.center)) < sphere.radius)
		{
			if (firstIntersection)
				*firstIntersection = vertices[i];
			return TRUE;
		}
	}

	return FALSE;
}

BOOL IntersectionTester::Test(const BoundingBox &a, const BoundingBox &b)
{
	STACK_TRACE;
	if (a.max.x < b.min.x || a.min.x > b.max.x)
		return FALSE;
	if (a.max.y < b.min.y || a.min.y > b.max.y)
		return FALSE;
	if (a.max.z < b.min.z || a.min.z > b.max.z)
		return FALSE;

	return TRUE;
}

BOOL IntersectionTester::Test(const BoundingSphere &a, const BoundingSphere &b)
{
	STACK_TRACE;
	Vector3 temp = a.center - b.center;
	float distanceSquared = Vector3::Dot(temp, temp);

	float radiusSum = a.radius + b.radius;
	if (distanceSquared <= radiusSum * radiusSum)
		return TRUE;
	else
		return FALSE;
}

BOOL IntersectionTester::Test(const BoundingSphere &sphere, const Plane &plane)
{
	STACK_TRACE;
	float distance = Vector3::Dot(sphere.center, plane.normal) - plane.d;
	if (fabsf(distance) <= sphere.radius)
		return TRUE;
	else
		return FALSE;
}

BOOL IntersectionTester::Test(const BoundingBox &box, const Plane &plane)
{
	STACK_TRACE;
	Vector3 temp1 = (box.max + box.min) / 2.0f;
	Vector3 temp2 = box.max - temp1;

	float radius = (temp2.x * fabsf(plane.normal.x)) + (temp2.y * fabsf(plane.normal.y)) + (temp2.z * fabsf(plane.normal.z));

	float distance = Vector3::Dot(plane.normal, temp1) - plane.d;

	if (fabsf(distance) <= radius)
		return TRUE;
	else
		return FALSE;
}

BOOL IntersectionTester::Test(const Ray &ray, const Plane &plane, Vector3 *intersection)
{
	STACK_TRACE;
	float denominator = Vector3::Dot(ray.direction, plane.normal);
	if (denominator == 0.0f)
		return FALSE;

	float t = ((-plane.d - Vector3::Dot(ray.position, plane.normal)) / denominator);
	if (t < 0.0f)
		return FALSE;

	if (intersection)
	{
		Vector3 temp1 = ray.GetPositionAt(t);
		intersection->x = temp1.x;
		intersection->y = temp1.y;
		intersection->z = temp1.z;
	}

	return TRUE;
}

BOOL IntersectionTester::Test(const Ray &ray, const BoundingSphere &sphere, Vector3 *intersection)
{
	STACK_TRACE;
	Vector3 temp1 = ray.position - sphere.center;

	float b = Vector3::Dot(temp1, ray.direction);
	float c = Vector3::Dot(temp1, temp1) - (sphere.radius * sphere.radius);

	if (c > 0.0f && b > 0.0f)
		return FALSE;

	float discriminant = b * b - c;
	if (discriminant < 0.0f)
		return FALSE;

	float t = -b - sqrtf(discriminant);
	if (t < 0.0f)
		t = 0.0f;

	if (intersection)
	{
		Vector3 temp2 = ray.GetPositionAt(t);
		intersection->x = temp2.x;
		intersection->y = temp2.y;
		intersection->z = temp2.z;
	}

	return TRUE;
}

BOOL IntersectionTester::Test(const Ray &ray, const BoundingBox &box, Vector3 *intersection)
{
	STACK_TRACE;
	float tmin = 0.0f;
	float tmax = FLT_MAX;

	if (fabsf(ray.direction.x) < EPSILON)
	{
		if (ray.position.x < box.min.x || ray.position.x > box.max.x)
			return FALSE;
	}
	else
	{
		float invD = 1.0f / ray.direction.x;
		float t1 = (box.min.x - ray.position.x) * invD;
		float t2 = (box.max.x - ray.position.x) * invD;

		if (t1 > t2)
		{
			float tswap = t1;
			t1 = t2;
			t2 = tswap;
		}

		tmin = Max(tmin, t1);
		tmax = Min(tmax, t2);

		if (tmin > tmax)
			return FALSE;
	}

	if (fabsf(ray.direction.y) < EPSILON)
	{
		if (ray.position.y < box.min.y || ray.position.y > box.max.y)
			return FALSE;
	}
	else
	{
		float invD = 1.0f / ray.direction.y;
		float t1 = (box.min.y - ray.position.y) * invD;
		float t2 = (box.max.y - ray.position.y) * invD;

		if (t1 > t2)
		{
			float tswap = t1;
			t1 = t2;
			t2 = tswap;
		}

		tmin = Max(tmin, t1);
		tmax = Min(tmax, t2);

		if (tmin > tmax)
			return FALSE;
	}

	if (fabsf(ray.direction.z) < EPSILON)
	{
		if (ray.position.z < box.min.z || ray.position.z > box.max.z)
			return FALSE;
	}
	else
	{
		float invD = 1.0f / ray.direction.z;
		float t1 = (box.min.z - ray.position.z) * invD;
		float t2 = (box.max.z - ray.position.z) * invD;

		if (t1 > t2)
		{
			float tswap = t1;
			t1 = t2;
			t2 = tswap;
		}

		tmin = Max(tmin, t1);
		tmax = Min(tmax, t2);

		if (tmin > tmax)
			return FALSE;
	}

	if (intersection)
	{
		Vector3 temp1 = ray.GetPositionAt(tmin);
		intersection->x = temp1.x;
		intersection->y = temp1.y;
		intersection->z = temp1.z;
	}

	return TRUE;
}

BOOL IntersectionTester::Test(const BoundingBox &box, const BoundingSphere &sphere)
{
	STACK_TRACE;
	float distanceSq = BoundingBox::GetSquaredDistanceFromPointToBox(sphere.center, box);

	if (distanceSq <= (sphere.radius * sphere.radius))
		return TRUE;
	else
		return FALSE;
}

BOOL IntersectionTester::Test(const Ray &ray, const Vector3 &a, const Vector3 &b, const Vector3 &c, Vector3 *intersection)
{
	STACK_TRACE;
	float r, num1, num2;

	Vector3 temp1 = Vector3::Cross(b - a, c - a);
	if (temp1.x == 0.0f && temp1.y == 0.0f && temp1.z == 0.0f)
		return FALSE;

	Vector3 temp2 = ray.position - a;
	num1 = -Vector3::Dot(temp1, temp2);
	num2 = Vector3::Dot(temp1, ray.direction);
	if (fabsf(num2) < EPSILON)
	{
		if (num1 == 0.0f)
		{
			if (intersection)
				*intersection = ray.position;
			return TRUE;
		}
		else
			return FALSE;
	}

	r = num1 / num2;
	if (r < 0.0f)
		return FALSE;

	Vector3 temp3 = ray.GetPositionAt(r);
	if (Vector3::IsPointInTriangle(temp3, a, b, c))
	{
		if (intersection)
			*intersection = temp3;
		return TRUE;
	}
	else
		return FALSE;
}

BOOL IntersectionTester::Test(CollisionPacket &packet, const Vector3 &v1, const Vector3 &v2, const Vector3 &v3)
{
	STACK_TRACE;
	BOOL foundCollision = FALSE;

	Vector3 p1 = v1 / packet.ellipsoidRadius;
	Vector3 p2 = v2 / packet.ellipsoidRadius;
	Vector3 p3 = v3 / packet.ellipsoidRadius;

	Plane trianglePlane(p1, p2, p3);

	// Is the triangle front-facing to the entity's velocity?
	if (Plane::IsFrontFacingTo(trianglePlane, packet.esNormalizedVelocity))
	{
		float t0;
		float t1;
		BOOL embeddedInPlane = FALSE;
		float distToTrianglePlane = Plane::DistanceBetween(trianglePlane, packet.esPosition);
		float normalDotVelocity = Vector3::Dot(trianglePlane.normal, packet.esVelocity);

		// Is the sphere travelling parallel to the plane?
		if (normalDotVelocity == 0.0f)
		{
			if (fabsf(distToTrianglePlane) >= 1.0f)
			{
				// Sphere is not embedded in the plane, no collision possible
				return FALSE;
			}
			else
			{
				// Sphere is embedded in the plane, it intersects throughout the whole time period
				embeddedInPlane = TRUE;
				t0 = 0.0f;
				t1 = 1.0f;
			}
		}
		else
		{
			// Not travelling parallel to the plane
			t0 = (-1.0f - distToTrianglePlane) / normalDotVelocity;
			t1 = (1.0f - distToTrianglePlane) / normalDotVelocity;

			// Swap so t0 < t1
			if (t0 > t1)
			{
				float temp = t1;
				t1 = t0;
				t0 = temp;
			}

			// Check that at least one result is within range
			if (t0 > 1.0f || t1 < 0.0f)
			{
				// Both values outside the range [0,1], no collision possible
				return FALSE;
			}

			t0 = Clamp(t0, 0.0f, 1.0f);
			t1 = Clamp(t1, 0.0f, 1.0f);
		}

		// At this point, we have two time values (t0, t1) between which the
		// swept sphere intersects with the triangle plane
		Vector3 collisionPoint;
		float t = 1.0f;

		// First, check for a collision inside the triangle. This will happen
		// at time t0 if at all as this is when the sphere rests on the front
		// side of the triangle plane.
		if (!embeddedInPlane)
		{
			Vector3 planeIntersectionPoint = (packet.esPosition - trianglePlane.normal) + packet.esVelocity * t0;

			if (Vector3::IsPointInTriangle(planeIntersectionPoint, p1, p2, p3))
			{
				foundCollision = TRUE;
				t = t0;
				collisionPoint = planeIntersectionPoint;
			}
		}

		// If we haven't found a collision at this point, we need to check the 
		// points and edges of the triangle
		if (foundCollision == FALSE)
		{
			Vector3 velocity = packet.esVelocity;
			Vector3 base = packet.esPosition;
			float velocitySquaredLength = Vector3::LengthSquared(velocity);
			float a, b, c;
			float newT;

			// For each vertex or edge, we have a quadratic equation to be solved
			// Check against the points first

			a = velocitySquaredLength;

			// P1
			b = 2.0f * Vector3::Dot(velocity, base - p1);
			c = Vector3::LengthSquared(p1 - base) - 1.0f;
			if (GetLowestQuadraticRoot(a, b, c, t, newT))
			{
				t = newT;
				foundCollision = TRUE;
				collisionPoint = p1;
			}

			// P2
			b = 2.0f * Vector3::Dot(velocity, base - p2);
			c = Vector3::LengthSquared(p2 - base) - 1.0f;
			if (GetLowestQuadraticRoot(a, b, c, t, newT))
			{
				t = newT;
				foundCollision = TRUE;
				collisionPoint = p2;
			}

			// P3
			b = 2.0f * Vector3::Dot(velocity, base - p3);
			c = Vector3::LengthSquared(p3 - base) - 1.0f;
			if (GetLowestQuadraticRoot(a, b, c, t, newT))
			{
				t = newT;
				foundCollision = TRUE;
				collisionPoint = p3;
			}

			// Now check against the edges

			// P1 -> P2
			Vector3 edge = p2 - p1;
			Vector3 baseToVertex = p1 - base;
			float edgeSquaredLength = Vector3::LengthSquared(edge);
			float edgeDotVelocity = Vector3::Dot(edge, velocity);
			float edgeDotBaseToVertex = Vector3::Dot(edge, baseToVertex);
			
			a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * Vector3::Dot(velocity, baseToVertex)) - 2.0f * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1.0f - Vector3::LengthSquared(baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

			if (GetLowestQuadraticRoot(a, b, c, t, newT))
			{
				// Check if intersection is within line segment
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
				if (f >= 0.0f && f <= 1.0f)
				{
					// Intersection took place within the segment
					t = newT;
					foundCollision = TRUE;
					collisionPoint = p1 + edge * f;
				}
			}

			// P2 -> P3
			edge = p3 - p2;
			baseToVertex = p2 - base;
			edgeSquaredLength = Vector3::LengthSquared(edge);
			edgeDotVelocity = Vector3::Dot(edge, velocity);
			edgeDotBaseToVertex = Vector3::Dot(edge, baseToVertex);
			
			a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * Vector3::Dot(velocity, baseToVertex)) - 2.0f * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1.0f - Vector3::LengthSquared(baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

			if (GetLowestQuadraticRoot(a, b, c, t, newT))
			{
				// Check if intersection is within line segment
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
				if (f >= 0.0f && f <= 1.0f)
				{
					// Intersection took place within the segment
					t = newT;
					foundCollision = TRUE;
					collisionPoint = p2 + edge * f;
				}
			}

			// P3 -> P1
			edge = p1 - p3;
			baseToVertex = p3 - base;
			edgeSquaredLength = Vector3::LengthSquared(edge);
			edgeDotVelocity = Vector3::Dot(edge, velocity);
			edgeDotBaseToVertex = Vector3::Dot(edge, baseToVertex);
			
			a = edgeSquaredLength * -velocitySquaredLength + edgeDotVelocity * edgeDotVelocity;
			b = edgeSquaredLength * (2.0f * Vector3::Dot(velocity, baseToVertex)) - 2.0f * edgeDotVelocity * edgeDotBaseToVertex;
			c = edgeSquaredLength * (1.0f - Vector3::LengthSquared(baseToVertex)) + edgeDotBaseToVertex * edgeDotBaseToVertex;

			if (GetLowestQuadraticRoot(a, b, c, t, newT))
			{
				// Check if intersection is within line segment
				float f = (edgeDotVelocity * newT - edgeDotBaseToVertex) / edgeSquaredLength;
				if (f >= 0.0f && f <= 1.0f)
				{
					// Intersection took place within the segment
					t = newT;
					foundCollision = TRUE;
					collisionPoint = p3 + edge * f;
				}
			}
		}

		// Set result of test
		if (foundCollision == TRUE)
		{
			float distanceToCollision = t * Vector3::Length(packet.esVelocity);

			// Does this triangle qualify for the closest collision?
			if (packet.foundCollision == FALSE || distanceToCollision < packet.nearestDistance)
			{
				packet.nearestDistance = distanceToCollision;
				packet.esIntersectionPoint = collisionPoint;
				packet.foundCollision = TRUE;
			}
		}
	}

	return foundCollision;
}

