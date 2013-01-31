#include "../debug.h"

#include "boundingsphere.h"

#include <math.h>

BoundingSphere::BoundingSphere(const Vector3 *vertices, int numVertices)
{
	STACK_TRACE;
	ASSERT(vertices != NULL);
	ASSERT(numVertices > 0);

	int min;
	int max;

	int minX = 0;
	int minY = 0;
	int minZ = 0;
	int maxX = 0;
	int maxY = 0;
	int maxZ = 0;

	// find min & max points for x, y and z
	for (int i = 0; i < numVertices; ++i)
	{
		if (vertices[i].x < vertices[minX].x)
			minX = i;
		if (vertices[i].x > vertices[maxX].x)
			maxX = i;
		if (vertices[i].y < vertices[minY].y)
			minY = i;
		if (vertices[i].y > vertices[maxY].y)
			maxY = i;
		if (vertices[i].z < vertices[minZ].z)
			minZ = i;
		if (vertices[i].z > vertices[maxZ].z)
			maxZ = i;
	}

	// distances between these extremes for x, y and z
	float distanceSqX = Vector3::Dot(vertices[maxX] - vertices[minX], vertices[maxX] - vertices[minX]);
	float distanceSqY = Vector3::Dot(vertices[maxY] - vertices[minY], vertices[maxY] - vertices[minY]);
	float distanceSqZ = Vector3::Dot(vertices[maxZ] - vertices[minZ], vertices[maxZ] - vertices[minZ]);

	// get the pair of points representing the most distance points from each other
	min = minX;
	max = maxX;
	if (distanceSqY > distanceSqX && distanceSqY > distanceSqZ)
	{
		min = minY;
		max = maxY;
	}
	if (distanceSqZ > distanceSqX && distanceSqZ > distanceSqY)
	{
		min = minZ;
		max = maxZ;
	}

	// we now have enough info to set the initial sphere properties
	center = (vertices[min] + vertices[max]) / 2.0f;
	radius = sqrtf(Vector3::Dot(vertices[max] - center, vertices[max] - center));

	// now expand the sphere to make sure it encompasses all the points (if it doesn't already)
	Vector3 d;
	for (int i = 0; i < numVertices; ++i)
	{
		d = vertices[i] - center;
		float distanceSq = Vector3::Dot(d, d);
		if (distanceSq > (radius * radius))
		{
			float distance = sqrtf(distanceSq);
			float newRadius = (radius + distance) * 0.5f;
			float k = (newRadius - radius) / distance;
			radius = newRadius;
			d = d * k;
			center = center + d;
		}
	}
}

