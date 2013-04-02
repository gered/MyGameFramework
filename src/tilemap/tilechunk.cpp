#include "../framework/debug.h"

#include "tilechunk.h"

#include "chunkvertexgenerator.h"
#include "tilemap.h"
#include "tilemesh.h"
#include "tilemeshcollection.h"

#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/vertexbuffer.h"
#include "../framework/math/intersectiontester.h"
#include "../framework/math/mathhelpers.h"
#include "../framework/math/ray.h"
#include "../framework/math/vector3.h"

#include <math.h>

TileChunk::TileChunk(uint x, uint y, uint z, uint width, uint height, uint depth, const TileMap *tileMap, GraphicsDevice *graphicsDevice)
{
	ASSERT(tileMap != NULL);
	ASSERT(graphicsDevice != NULL);

	m_tileMap = tileMap;
	m_graphicsDevice = graphicsDevice;

	m_width = width;
	m_height = height;
	m_depth = depth;
	m_x = x;
	m_y = y;
	m_z = z;
	m_position = Vector3((float)m_x, (float)m_y, (float)m_z);

	m_bounds.min = Vector3((float)m_x, (float)m_y, (float)m_z);
	m_bounds.max = Vector3((float)(m_x + m_width), (float)(m_y + m_height), (float)(m_z + m_depth));

	m_data = new Tile[width * height * depth];
	ASSERT(m_data != NULL);
	
	VERTEX_ATTRIBS attribs[] = {
		VERTEX_POS_3D,
		VERTEX_NORMAL,
		VERTEX_TEXCOORD,
		VERTEX_COLOR
	};

	// TODO: is 16 a good starting default size?
	m_vertices = new VertexBuffer();
	ASSERT(m_vertices != NULL);
	m_vertices->Initialize(attribs, 4, 16, BUFFEROBJECT_USAGE_STATIC);
	m_numVertices = 0;

	// start off assuming we don't have any alpha vertices
	m_alphaVertices = NULL;
	m_numAlphaVertices = 0;
}

TileChunk::~TileChunk()
{
	m_graphicsDevice->UnregisterManagedResource(m_vertices);
	if (m_alphaVertices != NULL)
		m_graphicsDevice->UnregisterManagedResource(m_alphaVertices);

	SAFE_DELETE_ARRAY(m_data);
	SAFE_DELETE(m_vertices);
	SAFE_DELETE(m_alphaVertices);
}

void TileChunk::GetBoundingBoxFor(uint x, uint y, uint z, BoundingBox *box) const
{
	ASSERT(box != NULL);

	// "chunk space"
	box->min = Vector3((float)x, (float)y, (float)z);
	box->max = Vector3(x + 1.0f, y + 1.0f, z + 1.0f);   // 1.0f = tile width

	// move to world space ("tilemap space" maybe .. ?)
	box->min += m_bounds.min;
	box->max += m_bounds.min;
}

BoundingBox TileChunk::GetBoundingBoxFor(uint x, uint y, uint z) const
{
	BoundingBox box;

	// "chunk space"
	box.min = Vector3((float)x, (float)y, (float)z);
	box.max = Vector3(x + 1.0f, y + 1.0f, z + 1.0f);   // 1.0f = tile width

	// move to world space ("tilemap space" maybe .. ?)
	box.min += m_bounds.min;
	box.max += m_bounds.min;

	return box;
}

bool TileChunk::CheckForCollision(const Ray &ray, uint &x, uint &y, uint &z) const
{
	// make sure that the ray and this chunk can actually collide in the first place
	Vector3 position;
	if (!IntersectionTester::Test(ray, m_bounds, &position))
		return false;

	// convert initial chunk collision point to tile coords (this is in "tilemap space")
	int currentX = (int)position.x;
	int currentY = (int)position.y;
	int currentZ = (int)position.z;

	// make sure the coords are inrange of this chunk. due to some floating
	// point errors / decimal truncating from the above conversion we could
	// end up with one or more that are very slightly out of bounds.
	// this is still in "tilemap space"
	currentX = Clamp(currentX, (int)GetX(), (int)(GetX() + GetWidth() - 1));
	currentY = Clamp(currentY, (int)GetY(), (int)(GetY() + GetHeight() - 1));
	currentZ = Clamp(currentZ, (int)GetZ(), (int)(GetZ() + GetDepth() - 1));

	// convert to "chunk space"
	currentX -= (int)GetX();
	currentY -= (int)GetY();
	currentZ -= (int)GetZ();

	// is the start position colliding with a solid tile?
	Tile *startTile = Get(currentX, currentY, currentZ);
	if (IsBitSet(TILE_COLLIDABLE, startTile->flags))
	{
		// collision found, set the tile coords of the collision
		x = currentX;
		y = currentY;
		z = currentZ;

		// and we're done
		return true;
	}

	// no collision initially, continue on with the rest ...

	// step increments in "chunk tile" units
	int stepX = (int)Sign(ray.direction.x);
	int stepY = (int)Sign(ray.direction.y);
	int stepZ = (int)Sign(ray.direction.z);

	// tile boundary (needs to be in "tilemap space")
	int tileBoundaryX = (int)GetX() + currentX + (stepX > 0 ? 1 : 0);
	int tileBoundaryY = (int)GetY() + currentY + (stepY > 0 ? 1 : 0);
	int tileBoundaryZ = (int)GetZ() + currentZ + (stepZ > 0 ? 1 : 0);

	// HACK: for the tMax and tDelta initial calculations below, if any of the
	//       components of ray.direction are zero, it will result in "inf"
	//       components in tMax or tDelta. This is fine, but it has to be
	//       *positive* "inf", not negative. What I found was that sometimes
	//       they would be negative, sometimes positive. So, we force them to be
	//       positive below. Additionally, "nan" components (which will happen
	//       if both sides of the divisions are zero) are bad, and we need to 
	//       change that up for "inf" as well.
	// TODO: check that this is compatible with Visual C++

	// determine how far we can travel along the ray before we hit a tile boundary
	Vector3 tMax = Vector3(
		(tileBoundaryX - ray.position.x) / ray.direction.x,
		(tileBoundaryY - ray.position.y) / ray.direction.y,
		(tileBoundaryZ - ray.position.z) / ray.direction.z
		);
	if (tMax.x == -INFINITY)
		tMax.x = INFINITY;
	if (tMax.y == -INFINITY)
		tMax.y = INFINITY;
	if (tMax.z == -INFINITY)
		tMax.z = INFINITY;
	if (isnan(tMax.x))
		tMax.x = INFINITY;
	if (isnan(tMax.y))
		tMax.y = INFINITY;
	if (isnan(tMax.z))
		tMax.z = INFINITY;

	// determine how far we must travel along the ray before we cross a grid cell
	Vector3 tDelta = Vector3(
		stepX / ray.direction.x,
		stepY / ray.direction.y,
		stepZ / ray.direction.z
		);
	if (tDelta.x == -INFINITY)
		tDelta.x = INFINITY;
	if (tDelta.y == -INFINITY)
		tDelta.y = INFINITY;
	if (tDelta.z == -INFINITY)
		tDelta.z = INFINITY;
	if (isnan(tDelta.x))
		tDelta.x = INFINITY;
	if (isnan(tDelta.y))
		tDelta.y = INFINITY;
	if (isnan(tDelta.z))
		tDelta.z = INFINITY;

	bool collided = false;
	bool outOfChunk = false;
	while (!outOfChunk)
	{
		// step up to the next tile using the lowest step value
		// (in other words, we figure out on which axis, X, Y, or Z, the next
		// tile that lies on the ray is closest, and use that axis step increment
		// to move us up to get to the next tile location)
		if (tMax.x < tMax.y && tMax.x < tMax.z)
		{
			// tMax.x is lowest, the YZ tile boundary plane is closest
			currentX += stepX;
			tMax.x += tDelta.x;
		}
		else if (tMax.y < tMax.z)
		{
			// tMax.y is lowest, the XZ tile boundary plane is closest
			currentY += stepY;
			tMax.y += tDelta.y;
		}
		else
		{
			// tMax.z is lowest, the XY tile boundary plane is closest
			currentZ += stepZ;
			tMax.z += tDelta.z;
		}

		// need to figure out if this new position is still inside the bounds of
		// the chunk before we can attempt to determine if the current tile is 
		// solid
		if (
				currentX < 0 || currentX >= (int)GetWidth() ||
				currentY < 0 || currentY >= (int)GetHeight() ||
				currentZ < 0 || currentZ >= (int)GetDepth()
			)
			outOfChunk = true;
		else
		{
			// still inside and at the next position, test for a solid tile
			Tile *tile = Get(currentX, currentY, currentZ);
			if (IsBitSet(TILE_COLLIDABLE, tile->flags))
			{
				collided = true;

				// set the tile coords of the collision
				x = currentX;
				y = currentY;
				z = currentZ;

				break;
			}
		}
	}

	return collided;
}

bool TileChunk::CheckForCollision(const Ray &ray, Vector3 &point, uint &x, uint &y, uint &z) const
{
	// if the ray doesn't collide with any solid tiles in the first place, then
	// we can skip this more expensive triangle collision check...
	if (!CheckForCollision(ray, x, y, z))
		return false;

	// now perform the per-triangle collision check against the tile position
	// where the ray ended up at the end of the above CheckForCollision() call
	return CheckForCollisionWithTile(ray, point, x, y, z);
}

bool TileChunk::CheckForCollisionWithTile(const Ray &ray, Vector3 &point, uint x, uint y, uint z) const
{
	const Tile *tile = Get(x, y, z);
	const TileMesh *mesh = m_tileMap->GetMeshes()->Get(tile);

	uint numVertices = mesh->GetNumCollisionVertices();
	const Vector3 *vertices = mesh->GetCollisionVertices();

	// world position of this tile, will be used to move each
	// mesh triangle into world space
	Vector3 tileWorldPosition = Vector3((float)x, (float)y, (float)z);
	
	float closestSquaredDistance = FLT_MAX;
	bool collided = false;
	Vector3 collisionPoint = ZERO_VECTOR;

	for (uint i = 0; i < numVertices; i += 3)
	{
		// get the vertices making up this triangle
		Vector3 a = vertices[i];
		Vector3 b = vertices[i + 1];
		Vector3 c = vertices[i + 2];

		// move these vertices into world space
		a += tileWorldPosition;
		b += tileWorldPosition;
		c += tileWorldPosition;

		if (IntersectionTester::Test(ray, a, b, c, &collisionPoint))
		{
			collided = true;

			// if this is the closest collision yet, then keep the distance
			// and point of collision
			float squaredDistance = Vector3::LengthSquared(collisionPoint - ray.position);
			if (squaredDistance < closestSquaredDistance)
			{
				closestSquaredDistance = squaredDistance;
				point = collisionPoint;
			}
		}
	}

	return collided;
}

bool TileChunk::GetOverlappedTiles(const BoundingBox &box, uint &x1, uint &y1, uint &z1, uint &x2, uint &y2, uint &z2) const
{
	// make sure the given box actually intersects with this chunk in the first place
	if (!IntersectionTester::Test(m_bounds, box))
		return false;

	// convert to tile coords (these will be in "tilemap space")
	// HACK: ceil() calls and "-1"'s keep us from picking up too many/too few
	// blocks. these were arrived at through observation
	int minX = (int)box.min.x;
	int minY = (int)box.min.y;
	int minZ = (int)box.min.z;
	int maxX = (int)ceil(box.max.x);
	int maxY = (int)ceil(box.max.y - 1.0f);
	int maxZ = (int)ceil(box.max.z);

	// trim off the excess bounds so that we end up with a min-to-max area
	// that is completely within the bounds of this chunk
	// HACK: "-1"'s keep us from picking up too many blocks. these were arrived
	// at through observation
	minX = Clamp(minX, (int)GetX(), (int)(GetX() + GetWidth()));
	minY = Clamp(minY, (int)GetY(), (int)(GetY() + GetHeight() - 1));
	minZ = Clamp(minZ, (int)GetZ(), (int)(GetZ() + GetDepth()));
	maxX = Clamp(maxX, (int)GetX(), (int)(GetX() + GetWidth()));
	maxY = Clamp(maxY, (int)GetY(), (int)(GetY() + GetHeight() - 1));
	maxZ = Clamp(maxZ, (int)GetZ(), (int)(GetZ() + GetDepth()));

	// return the leftover area, converted to "chunk space"
	x1 = minX - (int)GetX();
	y1 = minY - (int)GetY();
	z1 = minZ - (int)GetZ();
	x2 = maxX - (int)GetX();
	y2 = maxY - (int)GetY();
	z2 = maxZ - (int)GetZ();

	return true;
}

Tile* TileChunk::GetWithinSelfOrNeighbour(int x, int y, int z) const
{
	int checkX = (int)GetX() + x;
	int checkY = (int)GetY() + y;
	int checkZ = (int)GetZ() + z;
	return m_tileMap->Get(checkX, checkY, checkZ);
}

Tile* TileChunk::GetWithinSelfOrNeighbourSafe(int x, int y, int z) const
{
	int checkX = (int)GetX() + x;
	int checkY = (int)GetY() + y;
	int checkZ = (int)GetZ() + z;
	if (!m_tileMap->IsWithinBounds(checkX, checkY, checkZ))
		return NULL;
	else
		return m_tileMap->Get(checkX, checkY, checkZ);
}

void TileChunk::EnableAlphaVertices(bool enable)
{
	if (enable)
	{
		if (m_alphaVertices != NULL)
			return;
		
		VERTEX_ATTRIBS attribs[] = {
			VERTEX_POS_3D,
			VERTEX_NORMAL,
			VERTEX_TEXCOORD,
			VERTEX_COLOR
		};

		// need to create the vertex buffer
		// TODO: is '16' a good default size? it probably isn't likely that
		//       chunks will have a lot of these. has to be non-zero anyway...
		m_alphaVertices = new VertexBuffer();
		ASSERT(m_alphaVertices != NULL);
		m_alphaVertices->Initialize(attribs, 4, 16, BUFFEROBJECT_USAGE_STATIC);
		m_numAlphaVertices = 0;
	}
	else
	{
		if (m_alphaVertices == NULL)
			return;

		// need to free the vertex buffer
		SAFE_DELETE(m_alphaVertices);
		m_numAlphaVertices = 0;
	}
}

void TileChunk::UpdateVertices(ChunkVertexGenerator *vertexGenerator)
{
	vertexGenerator->Generate(this, m_numVertices, m_numAlphaVertices);
}

