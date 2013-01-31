#ifndef __TILEMAP_TILECHUNK_H_INCLUDED__
#define __TILEMAP_TILECHUNK_H_INCLUDED__

#include "../framework/common.h"

#include "tile.h"
#include "../framework/math/boundingbox.h"

class ChunkVertexGenerator;
class GraphicsDevice;
class TileMap;
class VertexBuffer;
struct Ray;
struct Vector3;

class TileChunk
{
public:
	TileChunk(uint32_t x, uint32_t y, uint32_t z, uint32_t width, uint32_t height, uint32_t depth, const TileMap *tileMap, GraphicsDevice *graphicsDevice);
	virtual ~TileChunk();

	void EnableAlphaVertices(BOOL enable);
	BOOL IsAlphaEnabled() const                            { return m_alphaVertices != NULL; }
	void UpdateVertices(ChunkVertexGenerator *vertexGenerator);

	Tile* Get(uint32_t x, uint32_t y, uint32_t z) const;
	Tile* GetSafe(uint32_t x, uint32_t y, uint32_t z) const;
	Tile* GetWithinSelfOrNeighbour(int32_t x, int32_t y, int32_t z) const;
	Tile* GetWithinSelfOrNeighbourSafe(int32_t x, int32_t y, int32_t z) const;
	void GetBoundingBoxFor(uint32_t x, uint32_t y, uint32_t z, BoundingBox *box) const;
	BoundingBox GetBoundingBoxFor(uint32_t x, uint32_t y, uint32_t z) const;

	BOOL CheckForCollision(const Ray &ray, uint32_t &x, uint32_t &y, uint32_t &z) const;
	BOOL CheckForCollision(const Ray &ray, Vector3 &point, uint32_t &x, uint32_t &y, uint32_t &z) const;
	BOOL CheckForCollisionWithTile(const Ray &ray, Vector3 &point, uint32_t x, uint32_t y, uint32_t z) const;
	BOOL GetOverlappedTiles(const BoundingBox &box, uint32_t &x1, uint32_t &y1, uint32_t &z1, uint32_t &x2, uint32_t &y2, uint32_t &z2) const;

	BOOL IsWithinBounds(int32_t x, int32_t y, int32_t z) const;
	BOOL IsWithinLocalBounds(int32_t x, int32_t y, int32_t z) const;

	uint32_t GetX() const                                  { return m_x; }
	uint32_t GetY() const                                  { return m_y; }
	uint32_t GetZ() const                                  { return m_z; }
	const Vector3& GetPosition() const                     { return m_position; }
	uint32_t GetWidth() const                              { return m_width; }
	uint32_t GetHeight() const                             { return m_height; }
	uint32_t GetDepth() const                              { return m_depth; }
	const BoundingBox& GetBounds() const                   { return m_bounds; }

	const TileMap* GetTileMap() const                      { return m_tileMap; }
	VertexBuffer* GetVertices() const                      { return m_vertices; }
	VertexBuffer* GetAlphaVertices() const                 { return m_alphaVertices; }
	uint32_t GetNumVertices() const                        { return m_numVertices; }
	uint32_t GetNumAlphaVertices() const                   { return m_numAlphaVertices; }

private:
	uint32_t GetIndexOf(uint32_t x, uint32_t y, uint32_t z) const;

	Tile *m_data;
	const TileMap *m_tileMap;
	GraphicsDevice *m_graphicsDevice;
	VertexBuffer *m_vertices;
	VertexBuffer *m_alphaVertices;
	uint32_t m_numVertices;
	uint32_t m_numAlphaVertices;

	uint32_t m_x;
	uint32_t m_y;
	uint32_t m_z;
	uint32_t m_width;
	uint32_t m_height;
	uint32_t m_depth;
	BoundingBox m_bounds;
	Vector3 m_position;
};

inline Tile* TileChunk::Get(uint32_t x, uint32_t y, uint32_t z) const
{
	uint32_t index = GetIndexOf(x, y, z);
	return &m_data[index];
}

inline Tile* TileChunk::GetSafe(uint32_t x, uint32_t y, uint32_t z) const
{
	if (!IsWithinLocalBounds((int32_t)x, (int32_t)y, (int32_t)z))
		return NULL;
	else
		return Get(x, y, z);
}

inline uint32_t TileChunk::GetIndexOf(uint32_t x, uint32_t y, uint32_t z) const
{
	return (y * m_width * m_depth) + (z * m_width) + x;
}

inline BOOL TileChunk::IsWithinBounds(int32_t x, int32_t y, int32_t z) const
{
	if (x < (int32_t)GetX() || x >= (int32_t)(GetX() + m_width))
		return FALSE;
	if (y < (int32_t)GetY() || y >= (int32_t)(GetY() + m_height))
		return FALSE;
	if (z < (int32_t)GetZ() || z >= (int32_t)(GetZ() + m_depth))
		return FALSE;

	return TRUE;
}

inline BOOL TileChunk::IsWithinLocalBounds(int32_t x, int32_t y, int32_t z) const
{
	if (x < 0 || x >= (int32_t)m_width)
		return FALSE;
	if (y < 0 || y >= (int32_t)m_height)
		return FALSE;
	if (z < 0 || z >= (int32_t)m_depth)
		return FALSE;

	return TRUE;
}

#endif

