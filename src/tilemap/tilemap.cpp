#include "../framework/debug.h"

#include "tilemap.h"

#include "chunkvertexgenerator.h"
#include "tile.h"
#include "tilemaplighter.h"
#include "tilemesh.h"
#include "tilemeshcollection.h"
#include "tilemeshdefs.h"
#include "../framework/math/intersectiontester.h"
#include "../framework/math/mathhelpers.h"
#include "../framework/math/ray.h"
#include "../framework/math/rectf.h"
#include "../framework/math/vector3.h"

TileMap::TileMap(TileMeshCollection *tileMeshes, ChunkVertexGenerator *vertexGenerator, TileMapLighter *lighter, GraphicsDevice *graphicsDevice)
{
	ASSERT(tileMeshes != NULL);
	ASSERT(vertexGenerator != NULL);
	ASSERT(graphicsDevice != NULL);

	m_tileMeshes = tileMeshes;
	m_vertexGenerator = vertexGenerator;
	m_lighter = lighter;
	m_graphicsDevice = graphicsDevice;

	m_numChunks = 0;
	m_widthInChunks = 0;
	m_heightInChunks = 0;
	m_depthInChunks = 0;
	m_chunkWidth = 0;
	m_chunkHeight = 0;
	m_chunkDepth = 0;
	m_chunks = NULL;

	m_ambientLightValue = 0;
	m_skyLightValue = TILE_LIGHT_VALUE_SKY;

	m_bounds.min = ZERO_VECTOR;
	m_bounds.max = ZERO_VECTOR;
}

TileMap::~TileMap()
{
	Clear();
}

void TileMap::SetSize(uint32_t numChunksX, uint32_t numChunksY, uint32_t numChunksZ, uint32_t chunkSizeX, uint32_t chunkSizeY, uint32_t chunkSizeZ)
{
	ASSERT(numChunksX > 0);
	ASSERT(numChunksY > 0);
	ASSERT(numChunksZ > 0);
	ASSERT(chunkSizeX > 0);
	ASSERT(chunkSizeY > 0);
	ASSERT(chunkSizeZ > 0);

	if (m_numChunks > 0)
		Clear();

	m_numChunks = numChunksX * numChunksY * numChunksZ;
	m_widthInChunks = numChunksX;
	m_heightInChunks = numChunksY;
	m_depthInChunks = numChunksZ;
	m_chunkWidth = chunkSizeX;
	m_chunkHeight = chunkSizeY;
	m_chunkDepth = chunkSizeZ;

	m_chunks = new TileChunk*[m_numChunks];
	ASSERT(m_chunks != NULL);

	// set each one up
	for (uint32_t y = 0; y < numChunksY; ++y)
	{
		for (uint32_t z = 0; z < numChunksZ; ++z)
		{
			for (uint32_t x = 0; x < numChunksX; ++x)
			{
				TileChunk *chunk = new TileChunk(x * m_chunkWidth, y * m_chunkHeight, z * m_chunkDepth, m_chunkWidth, m_chunkHeight, m_chunkDepth, this, m_graphicsDevice);
				ASSERT(chunk != NULL);

				uint32_t index = GetChunkIndex(x, y, z);
				m_chunks[index] = chunk;
			}
		}
	}

	m_bounds.min = ZERO_VECTOR;
	m_bounds.max = Vector3((float)GetWidth(), (float)GetHeight(), (float)GetDepth());
}

void TileMap::Clear()
{
	for (uint32_t i = 0; i < m_numChunks; ++i)
	{
		TileChunk *chunk = m_chunks[i];
		SAFE_DELETE(chunk);
	}
	SAFE_DELETE_ARRAY(m_chunks);

	m_numChunks = 0;
	m_widthInChunks = 0;
	m_heightInChunks = 0;
	m_depthInChunks = 0;
	m_chunkWidth = 0;
	m_chunkHeight = 0;
	m_chunkDepth = 0;
}

BOOL TileMap::CheckForCollision(const Ray &ray, uint32_t &x, uint32_t &y, uint32_t &z) const
{
	// make sure that the ray can actually collide with this map in the first place
	Vector3 position;
	if (!IntersectionTester::Test(ray, m_bounds, &position))
		return FALSE;

	// convert initial tilemap collision point to tile coords. this is in 
	// "tilemap space" which is how we want it for the rest of this method
	int32_t currentX = (int32_t)position.x;
	int32_t currentY = (int32_t)position.y;
	int32_t currentZ = (int32_t)position.z;

	// make sure the coords are inrange of the map bounds. due to some floating
	// point errors / decimal truncating from the above conversion we could
	// end up with one or more that are very slightly out of bounds.
	currentX = Clamp(currentX, 0, (int32_t)(GetWidth() - 1));
	currentY = Clamp(currentY, 0, (int32_t)(GetHeight() - 1));
	currentZ = Clamp(currentZ, 0, (int32_t)(GetDepth() - 1));

	// is the start position colliding with a solid tile?
	Tile *startTile = Get(currentX, currentY, currentZ);
	if (IsBitSet(TILE_COLLIDABLE, startTile->flags))
	{
		// collision found set the tile coords of the collision
		x = currentX;
		y = currentY;
		z = currentZ;

		// and we're done
		return TRUE;
	}

	// no collision initially, continue on with the rest ...

	// step increments in whole tile units
	int32_t stepX = (int32_t)Sign(ray.direction.x);
	int32_t stepY = (int32_t)Sign(ray.direction.y);
	int32_t stepZ = (int32_t)Sign(ray.direction.z);

	// tile boundary
	int32_t tileBoundaryX = currentX + (stepX > 0 ? 1 : 0);
	int32_t tileBoundaryY = currentY + (stepY > 0 ? 1 : 0);
	int32_t tileBoundaryZ = currentZ + (stepZ > 0 ? 1 : 0);

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

	BOOL collided = FALSE;
	BOOL outOfMap = FALSE;
	while (!outOfMap)
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
		// the map before we can attempt to determine if the current tile is 
		// solid
		if (
				currentX < 0 || currentX >= (int32_t)GetWidth() ||
				currentY < 0 || currentY >= (int32_t)GetHeight() ||
				currentZ < 0 || currentZ >= (int32_t)GetDepth()
			)
			outOfMap = TRUE;
		else
		{
			// still inside and at the next position, test for a solid tile
			Tile *tile = Get(currentX, currentY, currentZ);
			if (IsBitSet(TILE_COLLIDABLE, tile->flags))
			{
				collided = TRUE;

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

BOOL TileMap::CheckForCollision(const Ray &ray, Vector3 &point, uint32_t &x, uint32_t &y, uint32_t &z) const
{
	// if the ray doesn't collide with any solid tiles in the first place, then
	// we can skip this more expensive triangle collision check...
	if (!CheckForCollision(ray, x, y, z))
		return FALSE;

	// now perform the per-triangle collision check against the tile position
	// where the ray ended up at the end of the above CheckForCollision() call
	return CheckForCollisionWithTile(ray, point, x, y, z);
}

BOOL TileMap::CheckForCollisionWithTile(const Ray &ray, Vector3 &point, uint32_t x, uint32_t y, uint32_t z) const
{
	const Tile *tile = Get(x, y, z);
	const TileMesh *mesh = m_tileMeshes->Get(tile);

	uint32_t numVertices = mesh->GetNumCollisionVertices();
	const Vector3 *vertices = mesh->GetCollisionVertices();

	// world position of this tile, will be used to move each
	// mesh triangle into world space
	Vector3 tileWorldPosition = Vector3((float)x, (float)y, (float)z);
	
	float closestSquaredDistance = FLT_MAX;
	BOOL collided = FALSE;
	Vector3 collisionPoint = ZERO_VECTOR;

	for (uint32_t i = 0; i < numVertices; i += 3)
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
			collided = TRUE;

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

BOOL TileMap::GetOverlappedTiles(const BoundingBox &box, uint32_t &x1, uint32_t &y1, uint32_t &z1, uint32_t &x2, uint32_t &y2, uint32_t &z2) const
{
	// make sure the given box actually intersects with the map in the first place
	if (!IntersectionTester::Test(m_bounds, box))
		return FALSE;

	// convert to tile coords. this is in "tilemap space" which is how we want it
	// HACK: ceil() calls and "-1"'s keep us from picking up too many/too few
	// blocks. these were arrived at through observation
	int32_t minX = (int32_t)box.min.x;
	int32_t minY = (int32_t)box.min.y;
	int32_t minZ = (int32_t)box.min.z;
	int32_t maxX = (int32_t)ceil(box.max.x);
	int32_t maxY = (int32_t)ceil(box.max.y - 1.0f);
	int32_t maxZ = (int32_t)ceil(box.max.z);

	// trim off the excess bounds so that we end up with a min-to-max area
	// that is completely within the bounds of the map
	// HACK: "-1"'s keep us from picking up too many blocks. these were arrived
	// at through observation
	minX = Clamp(minX, 0, (int32_t)GetWidth());
	minY = Clamp(minY, 0, (int32_t)(GetHeight() - 1));
	minZ = Clamp(minZ, 0, (int32_t)GetDepth());
	maxX = Clamp(maxX, 0, (int32_t)GetWidth());
	maxY = Clamp(maxY, 0, (int32_t)(GetHeight() - 1));
	maxZ = Clamp(maxZ, 0, (int32_t)GetDepth());

	// return the leftover area
	x1 = minX;
	y1 = minY;
	z1 = minZ;
	x2 = maxX;
	y2 = maxY;
	z2 = maxZ;

	return TRUE;
}

BOOL TileMap::GetOverlappedChunks(const BoundingBox &box, uint32_t &x1, uint32_t &y1, uint32_t &z1, uint32_t &x2, uint32_t &y2, uint32_t &z2) const
{
	// make sure the given box actually intersects with the map in the first place
	if (!IntersectionTester::Test(m_bounds, box))
		return FALSE;

	// convert to tile coords. this is in "tilemap space" which is how we want it
	// HACK: ceil() calls and "-1"'s keep us from picking up too many/too few
	// blocks. these were arrived at through observation
	int32_t minX = (int32_t)box.min.x;
	int32_t minY = (int32_t)box.min.y;
	int32_t minZ = (int32_t)box.min.z;
	int32_t maxX = (int32_t)ceil(box.max.x);
	int32_t maxY = (int32_t)ceil(box.max.y - 1.0f);
	int32_t maxZ = (int32_t)ceil(box.max.z);

	// now convert to chunk coords
	int32_t minChunkX = minX / (int32_t)GetChunkWidth();
	int32_t minChunkY = minY / (int32_t)GetChunkHeight();
	int32_t minChunkZ = minZ / (int32_t)GetChunkDepth();
	int32_t maxChunkX = maxX / (int32_t)GetChunkWidth();
	int32_t maxChunkY = maxY / (int32_t)GetChunkHeight();
	int32_t maxChunkZ = maxZ / (int32_t)GetChunkDepth();

	// trim off the excess bounds so that we end up with a min-to-max area
	// that is completely within the chunk bounds of the map
	// HACK: "-1"'s keep us from picking up too many chunks. these were arrived
	// at through observation
	minChunkX = Clamp(minChunkX, 0, (int32_t)GetWidthInChunks());
	minChunkY = Clamp(minChunkY, 0, (int32_t)(GetHeightInChunks() - 1));
	minChunkZ = Clamp(minChunkZ, 0, (int32_t)GetDepthInChunks());
	maxChunkX = Clamp(maxChunkX, 0, (int32_t)GetWidthInChunks());
	maxChunkY = Clamp(maxChunkY, 0, (int32_t)(GetHeightInChunks() - 1));
	maxChunkZ = Clamp(maxChunkZ, 0, (int32_t)GetDepthInChunks());

	// return the leftover area
	x1 = minChunkX;
	y1 = minChunkY;
	z1 = minChunkZ;
	x2 = maxChunkX;
	y2 = maxChunkY;
	z2 = maxChunkZ;

	return TRUE;
}

void TileMap::UpdateVertices()
{
	ASSERT(m_numChunks > 0);

	for (uint32_t i = 0; i < m_numChunks; ++i)
	{
		TileChunk *chunk = m_chunks[i];
		UpdateChunkVertices(chunk);
	}
}

void TileMap::UpdateChunkVertices(TileChunk *chunk)
{
	ASSERT(m_numChunks > 0);
	ASSERT(chunk != NULL);

	chunk->UpdateVertices(m_vertexGenerator);
}

void TileMap::UpdateLighting()
{
	ASSERT(m_numChunks > 0);

	if (m_lighter != NULL)
		m_lighter->Light(this);
}
