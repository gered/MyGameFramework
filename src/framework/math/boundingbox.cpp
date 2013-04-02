#include "../debug.h"

#include "boundingbox.h"

#include "../common.h"

BoundingBox::BoundingBox(const Vector3 *vertices, int numVertices)
{
	ASSERT(vertices != NULL);
	ASSERT(numVertices > 0);
	float minX = 0.0f;
	float minY = 0.0f;
	float minZ = 0.0f;
	float maxX = 0.0f;
	float maxY = 0.0f;
	float maxZ = 0.0f;

	for (int i = 0; i < numVertices; ++i)
	{
		minX = Min(vertices[i].x, minX);
		minY = Min(vertices[i].y, minY);
		minZ = Min(vertices[i].z, minZ);
		maxX = Max(vertices[i].x, maxX);
		maxY = Max(vertices[i].y, maxY);
		maxZ = Max(vertices[i].z, maxZ);
	}

	this->min = Vector3(minX, minY, minZ);
	this->max = Vector3(maxX, maxY, maxZ);
}

float BoundingBox::GetSquaredDistanceFromPointToBox(const Vector3 &point, const BoundingBox &box)
{
	float distanceSq = 0.0f;
	float v;

	v = point.x;
	if (v < box.min.x)
		distanceSq += (box.min.x - v) * (box.min.x - v);
	if (v > box.max.x)
		distanceSq += (v - box.max.x) * (v - box.max.x);

	v = point.y;
	if (v < box.min.y)
		distanceSq += (box.min.y - v) * (box.min.y - v);
	if (v > box.max.y)
		distanceSq += (v - box.max.y) * (v - box.max.y);

	v = point.z;
	if (v < box.min.z)
		distanceSq += (box.min.z - v) * (box.min.z - v);
	if (v > box.max.z)
		distanceSq += (v - box.max.z) * (v - box.max.z);

	return distanceSq;
}

float BoundingBox::GetDistanceFromPointToBox(const Vector3 &point, const BoundingBox &box)
{
	return sqrtf(GetSquaredDistanceFromPointToBox(point, box));
}
