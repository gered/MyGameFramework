#include "../debug.h"

#include "frustum.h"

#include "boundingbox.h"
#include "boundingsphere.h"
#include "matrix4x4.h"
#include "../graphics/viewcontext.h"
#include "../graphics/graphicsdevice.h"

Frustum::Frustum(ViewContext *viewContext)
{
	m_viewContext = viewContext;
	Calculate();
}

Frustum::~Frustum()
{
}

void Frustum::Calculate()
{
	Matrix4x4 projection = m_viewContext->GetProjectionMatrix();
	Matrix4x4 modelview = m_viewContext->GetModelViewMatrix();

	Matrix4x4 combined = projection * modelview;

	// Extract the sides of each of the 6 planes from this to get our viewing frustum
	m_planes[FRUSTUM_RIGHT].normal.x = combined.m[_41] - combined.m[_11];
	m_planes[FRUSTUM_RIGHT].normal.y = combined.m[_42] - combined.m[_12];
	m_planes[FRUSTUM_RIGHT].normal.z = combined.m[_43] - combined.m[_13];
	m_planes[FRUSTUM_RIGHT].d = combined.m[_44] - combined.m[_14];

	m_planes[FRUSTUM_LEFT].normal.x = combined.m[_41] + combined.m[_11];
	m_planes[FRUSTUM_LEFT].normal.y = combined.m[_42] + combined.m[_12];
	m_planes[FRUSTUM_LEFT].normal.z = combined.m[_43] + combined.m[_13];
	m_planes[FRUSTUM_LEFT].d = combined.m[_44] + combined.m[_14];

	m_planes[FRUSTUM_BOTTOM].normal.x = combined.m[_41] + combined.m[_21];
	m_planes[FRUSTUM_BOTTOM].normal.y = combined.m[_42] + combined.m[_22];
	m_planes[FRUSTUM_BOTTOM].normal.z = combined.m[_43] + combined.m[_23];
	m_planes[FRUSTUM_BOTTOM].d = combined.m[_44] + combined.m[_24];

	m_planes[FRUSTUM_TOP].normal.x = combined.m[_41] - combined.m[_21];
	m_planes[FRUSTUM_TOP].normal.y = combined.m[_42] - combined.m[_22];
	m_planes[FRUSTUM_TOP].normal.z = combined.m[_43] - combined.m[_23];
	m_planes[FRUSTUM_TOP].d = combined.m[_44] - combined.m[_24];

	m_planes[FRUSTUM_BACK].normal.x = combined.m[_41] - combined.m[_31];
	m_planes[FRUSTUM_BACK].normal.y = combined.m[_42] - combined.m[_32];
	m_planes[FRUSTUM_BACK].normal.z = combined.m[_43] - combined.m[_33];
	m_planes[FRUSTUM_BACK].d = combined.m[_44] - combined.m[_34];

	m_planes[FRUSTUM_FRONT].normal.x = combined.m[_41] + combined.m[_31];
	m_planes[FRUSTUM_FRONT].normal.y = combined.m[_42] + combined.m[_32];
	m_planes[FRUSTUM_FRONT].normal.z = combined.m[_43] + combined.m[_33];
	m_planes[FRUSTUM_FRONT].d = combined.m[_44] + combined.m[_34];

	m_planes[FRUSTUM_RIGHT] = Plane::Normalize(m_planes[FRUSTUM_RIGHT]);
	m_planes[FRUSTUM_LEFT] = Plane::Normalize(m_planes[FRUSTUM_LEFT]);
	m_planes[FRUSTUM_BOTTOM] = Plane::Normalize(m_planes[FRUSTUM_BOTTOM]);
	m_planes[FRUSTUM_TOP] = Plane::Normalize(m_planes[FRUSTUM_TOP]);
	m_planes[FRUSTUM_BACK] = Plane::Normalize(m_planes[FRUSTUM_BACK]);
	m_planes[FRUSTUM_FRONT] = Plane::Normalize(m_planes[FRUSTUM_FRONT]);
}

bool Frustum::Test(const Vector3 &point) const
{
	for (int p = 0; p < NUM_FRUSTUM_SIDES; ++p)
	{
		if (Plane::ClassifyPoint(m_planes[p], point) == BEHIND)
			return false;
	}

	return true;
}

bool Frustum::Test(const BoundingBox &box) const
{
	if (!TestPlaneAgainstBox(m_planes[FRUSTUM_RIGHT], box.min.x, box.min.y, box.min.z, box.GetWidth(), box.GetHeight(), box.GetDepth()))
		return false;
	if (!TestPlaneAgainstBox(m_planes[FRUSTUM_LEFT], box.min.x, box.min.y, box.min.z, box.GetWidth(), box.GetHeight(), box.GetDepth()))
		return false;
	if (!TestPlaneAgainstBox(m_planes[FRUSTUM_BOTTOM], box.min.x, box.min.y, box.min.z, box.GetWidth(), box.GetHeight(), box.GetDepth()))
		return false;
	if (!TestPlaneAgainstBox(m_planes[FRUSTUM_TOP], box.min.x, box.min.y, box.min.z, box.GetWidth(), box.GetHeight(), box.GetDepth()))
		return false;
	if (!TestPlaneAgainstBox(m_planes[FRUSTUM_BACK], box.min.x, box.min.y, box.min.z, box.GetWidth(), box.GetHeight(), box.GetDepth()))
		return false;
	if (!TestPlaneAgainstBox(m_planes[FRUSTUM_FRONT], box.min.x, box.min.y, box.min.z, box.GetWidth(), box.GetHeight(), box.GetDepth()))
		return false;

	return true;
}

bool Frustum::Test(const BoundingSphere &sphere) const
{
	if (!TestPlaneAgainstSphere(m_planes[FRUSTUM_RIGHT], sphere.center, sphere.radius))
		return false;
	if (!TestPlaneAgainstSphere(m_planes[FRUSTUM_LEFT], sphere.center, sphere.radius))
		return false;
	if (!TestPlaneAgainstSphere(m_planes[FRUSTUM_BOTTOM], sphere.center, sphere.radius))
		return false;
	if (!TestPlaneAgainstSphere(m_planes[FRUSTUM_TOP], sphere.center, sphere.radius))
		return false;
	if (!TestPlaneAgainstSphere(m_planes[FRUSTUM_BACK], sphere.center, sphere.radius))
		return false;
	if (!TestPlaneAgainstSphere(m_planes[FRUSTUM_FRONT], sphere.center, sphere.radius))
		return false;

	return true;
}

bool Frustum::TestPlaneAgainstBox(const Plane &plane, float minX, float minY, float minZ, float width, float height, float depth) const
{
	if (Plane::ClassifyPoint(plane, Vector3(minX,         minY,          minZ))         != BEHIND)
		return true;
	if (Plane::ClassifyPoint(plane, Vector3(minX,         minY,          minZ + depth)) != BEHIND)
		return true;
	if (Plane::ClassifyPoint(plane, Vector3(minX + width, minY,          minZ + depth)) != BEHIND)
		return true;
	if (Plane::ClassifyPoint(plane, Vector3(minX + width, minY,          minZ))         != BEHIND)
		return true;
	if (Plane::ClassifyPoint(plane, Vector3(minX,         minY + height, minZ))         != BEHIND)
		return true;
	if (Plane::ClassifyPoint(plane, Vector3(minX,         minY + height, minZ + depth)) != BEHIND)
		return true;
	if (Plane::ClassifyPoint(plane, Vector3(minX + width, minY + height, minZ + depth)) != BEHIND)
		return true;
	if (Plane::ClassifyPoint(plane, Vector3(minX + width, minY + height, minZ))         != BEHIND)
		return true;

	return false;
}

bool Frustum::TestPlaneAgainstSphere(const Plane &plane, const Vector3 &center, float radius) const
{
	float distance = Plane::DistanceBetween(plane, center);
	if (distance <= -radius)
		return false;
	else
		return true;
}
