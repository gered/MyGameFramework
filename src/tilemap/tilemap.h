#ifndef __TILEMAP_TILEMAP_H_INCLUDED__
#define __TILEMAP_TILEMAP_H_INCLUDED__

#include "../framework/common.h"

#include "tile.h"
#include "tilechunk.h"
#include "tilelightdefs.h"
#include "../framework/math/boundingbox.h"
#include "../framework/math/vector3.h"

#include <stl/vector.h>

class ChunkVertexGenerator;
class GraphicsDevice;
class TileChunk;
class TileMapLighter;
class TileMesh;
class TileMeshCollection;
struct RectF;
struct Ray;
struct Tile;

class TileMap
{
public:
	TileMap(TileMeshCollection *tileMeshes, ChunkVertexGenerator *vertexGenerator, TileMapLighter *lighter, GraphicsDevice *graphicsDevice);
	virtual ~TileMap();

	void SetSize(uint32_t numChunksX, uint32_t numChunksY, uint32_t numChunksZ, uint32_t chunkSizeX, uint32_t chunkSizeY, uint32_t chunkSizeZ);

	TileMeshCollection* GetMeshes() const              { return m_tileMeshes; }
	ChunkVertexGenerator* GetVertexGenerator() const   { return m_vertexGenerator; }
	TileMapLighter* GetLighter() const                 { return m_lighter; }

	Tile* Get(uint32_t x, uint32_t y, uint32_t z) const;
	Tile* GetSafe(uint32_t x, uint32_t y, uint32_t z) const;
	TileChunk* GetChunk(uint32_t chunkX, uint32_t chunkY, uint32_t chunkZ) const;
	TileChunk* GetChunkSafe(uint32_t chunkX, uint32_t chunkY, uint32_t chunkZ) const;
	TileChunk* GetChunkNextTo(TileChunk *chunk, int32_t offsetX, int32_t offsetY, int32_t offsetZ) const;
	TileChunk* GetChunkContaining(uint32_t x, uint32_t y, uint32_t z) const;

	void GetBoundingBoxFor(uint32_t x, uint32_t y, uint32_t z, BoundingBox *box) const;
	BoundingBox GetBoundingBoxFor(uint32_t x, uint32_t y, uint32_t z) const;

	BOOL CheckForCollision(const Ray &ray, uint32_t &x, uint32_t &y, uint32_t &z) const;
	BOOL CheckForCollision(const Ray &ray, Vector3 &point, uint32_t &x, uint32_t &y, uint32_t &z) const;
	BOOL CheckForCollisionWithTile(const Ray &ray, Vector3 &point, uint32_t x, uint32_t y, uint32_t z) const;
	BOOL GetOverlappedTiles(const BoundingBox &box, uint32_t &x1, uint32_t &y1, uint32_t &z1, uint32_t &x2, uint32_t &y2, uint32_t &z2) const;
	BOOL GetOverlappedChunks(const BoundingBox &box, uint32_t &x1, uint32_t &y1, uint32_t &z1, uint32_t &x2, uint32_t &y2, uint32_t &z2) const;

	void UpdateChunkVertices(uint32_t chunkX, uint32_t chunkY, uint32_t chunkZ);
	void UpdateVertices();

	void UpdateLighting();

	BOOL IsWithinBounds(int32_t x, int32_t y, int32_t z) const;

	uint32_t GetWidth() const                              { return m_widthInChunks * m_chunkWidth; }
	uint32_t GetHeight() const                             { return m_heightInChunks * m_chunkHeight; }
	uint32_t GetDepth() const                              { return m_depthInChunks * m_chunkDepth; }

	uint32_t GetChunkWidth() const                         { return m_chunkWidth; }
	uint32_t GetChunkHeight() const                        { return m_chunkHeight; }
	uint32_t GetChunkDepth() const                         { return m_chunkDepth; }

	uint32_t GetWidthInChunks() const                      { return m_widthInChunks; }
	uint32_t GetHeightInChunks() const                     { return m_heightInChunks; }
	uint32_t GetDepthInChunks() const                      { return m_depthInChunks; }
	const BoundingBox& GetBounds() const                   { return m_bounds; }

	uint32_t GetNumChunks() const                          { return m_numChunks; }

	void SetAmbientLightValue(TILE_LIGHT_VALUE value)      { m_ambientLightValue = value; }
	TILE_LIGHT_VALUE GetAmbientLightValue() const          { return m_ambientLightValue; }
	void SetSkyLightValue(TILE_LIGHT_VALUE value)          { m_skyLightValue = value; }
	TILE_LIGHT_VALUE GetSkyLightValue() const              { return m_skyLightValue; }

private:
	void Clear();

	uint32_t GetChunkIndexAt(uint32_t x, uint32_t y, uint32_t z) const;
	uint32_t GetChunkIndex(uint32_t chunkX, uint32_t chunkY, uint32_t chunkZ) const;

	void UpdateChunkVertices(TileChunk *chunk);

	TileMeshCollection *m_tileMeshes;
	TileChunk **m_chunks;
	GraphicsDevice *m_graphicsDevice;
	ChunkVertexGenerator *m_vertexGenerator;
	TileMapLighter *m_lighter;

	uint32_t m_chunkWidth;
	uint32_t m_chunkHeight;
	uint32_t m_chunkDepth;

	uint32_t m_widthInChunks;
	uint32_t m_heightInChunks;
	uint32_t m_depthInChunks;
	BoundingBox m_bounds;

	uint32_t m_numChunks;

	TILE_LIGHT_VALUE m_ambientLightValue;
	TILE_LIGHT_VALUE m_skyLightValue;
};

inline Tile* TileMap::Get(uint32_t x, uint32_t y, uint32_t z) const
{
	TileChunk *chunk = GetChunkContaining(x, y, z);
	uint32_t chunkX = x - chunk->GetX();
	uint32_t chunkY = y - chunk->GetY();
	uint32_t chunkZ = z - chunk->GetZ();

	return chunk->Get(chunkX, chunkY, chunkZ);
}

inline Tile* TileMap::GetSafe(uint32_t x, uint32_t y, uint32_t z) const
{
	if (!IsWithinBounds((int32_t)x, (int32_t)y, (int32_t)z))
		return NULL;
	else
		return Get(x, y, z);
}

inline TileChunk* TileMap::GetChunk(uint32_t chunkX, uint32_t chunkY, uint32_t chunkZ) const
{
	uint32_t index = GetChunkIndex(chunkX, chunkY, chunkZ);
	return m_chunks[index];
}

inline TileChunk* TileMap::GetChunkSafe(uint32_t chunkX, uint32_t chunkY, uint32_t chunkZ) const
{
	if (
		(chunkX >= m_widthInChunks) ||
		(chunkY >= m_heightInChunks) ||
		(chunkZ >= m_depthInChunks)
		)
		return NULL;
	else
		return GetChunk(chunkX, chunkY, chunkZ);
}

inline TileChunk* TileMap::GetChunkNextTo(TileChunk *chunk, int32_t offsetX, int32_t offsetY, int32_t offsetZ) const
{
	int32_t checkX = chunk->GetX() + offsetX;
	int32_t checkY = chunk->GetY() + offsetY;
	int32_t checkZ = chunk->GetZ() + offsetZ;

	if (
		(checkX < 0 || (uint32_t)checkX >= m_widthInChunks) ||
		(checkY < 0 || (uint32_t)checkY >= m_heightInChunks) ||
		(checkZ < 0 || (uint32_t)checkZ >= m_depthInChunks)
		)
		return NULL;
	else
		return GetChunk(checkX, checkY, checkZ);
}

inline TileChunk* TileMap::GetChunkContaining(uint32_t x, uint32_t y, uint32_t z) const
{
	uint32_t index = GetChunkIndexAt(x, y, z);
	return m_chunks[index];
}

inline void TileMap::GetBoundingBoxFor(uint32_t x, uint32_t y, uint32_t z, BoundingBox *box) const
{
	TileChunk *chunk = GetChunkContaining(x, y, z);
	uint32_t chunkX = x - chunk->GetX();
	uint32_t chunkY = y - chunk->GetY();
	uint32_t chunkZ = z - chunk->GetZ();

	chunk->GetBoundingBoxFor(chunkX, chunkY, chunkZ, box);
}

inline BoundingBox TileMap::GetBoundingBoxFor(uint32_t x, uint32_t y, uint32_t z) const
{
	BoundingBox box;
	GetBoundingBoxFor(x, y, z, &box);
	return box;
}

inline void TileMap::UpdateChunkVertices(uint32_t chunkX, uint32_t chunkY, uint32_t chunkZ)
{
	uint32_t index = GetChunkIndex(chunkX, chunkY, chunkZ);
	TileChunk *chunk = m_chunks[index];
	UpdateChunkVertices(chunk);
}

inline uint32_t TileMap::GetChunkIndexAt(uint32_t x, uint32_t y, uint32_t z) const
{
	return GetChunkIndex(x / m_chunkWidth, y / m_chunkHeight, z / m_chunkDepth);
}

inline uint32_t TileMap::GetChunkIndex(uint32_t chunkX, uint32_t chunkY, uint32_t chunkZ) const
{
	return (chunkY * m_widthInChunks * m_depthInChunks) + (chunkZ * m_widthInChunks) + chunkX;
}

inline BOOL TileMap::IsWithinBounds(int32_t x, int32_t y, int32_t z) const
{
	if (x < 0 || x >= (int32_t)GetWidth())
		return FALSE;
	if (y < 0 || y >= (int32_t)GetHeight())
		return FALSE;
	if (z < 0 || z >= (int32_t)GetDepth())
		return FALSE;

	return TRUE;
}
#endif

