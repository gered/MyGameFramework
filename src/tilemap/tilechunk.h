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
	TileChunk(uint x, uint y, uint z, uint width, uint height, uint depth, const TileMap *tileMap, GraphicsDevice *graphicsDevice);
	virtual ~TileChunk();

	void EnableAlphaVertices(BOOL enable);
	BOOL IsAlphaEnabled() const                            { return m_alphaVertices != NULL; }
	void UpdateVertices(ChunkVertexGenerator *vertexGenerator);

	Tile* Get(uint x, uint y, uint z) const;
	Tile* GetSafe(uint x, uint y, uint z) const;
	Tile* GetWithinSelfOrNeighbour(int x, int y, int z) const;
	Tile* GetWithinSelfOrNeighbourSafe(int x, int y, int z) const;
	void GetBoundingBoxFor(uint x, uint y, uint z, BoundingBox *box) const;
	BoundingBox GetBoundingBoxFor(uint x, uint y, uint z) const;

	BOOL CheckForCollision(const Ray &ray, uint &x, uint &y, uint &z) const;
	BOOL CheckForCollision(const Ray &ray, Vector3 &point, uint &x, uint &y, uint &z) const;
	BOOL CheckForCollisionWithTile(const Ray &ray, Vector3 &point, uint x, uint y, uint z) const;
	BOOL GetOverlappedTiles(const BoundingBox &box, uint &x1, uint &y1, uint &z1, uint &x2, uint &y2, uint &z2) const;

	BOOL IsWithinBounds(int x, int y, int z) const;
	BOOL IsWithinLocalBounds(int x, int y, int z) const;

	uint GetX() const                                      { return m_x; }
	uint GetY() const                                      { return m_y; }
	uint GetZ() const                                      { return m_z; }
	const Vector3& GetPosition() const                     { return m_position; }
	uint GetWidth() const                                  { return m_width; }
	uint GetHeight() const                                 { return m_height; }
	uint GetDepth() const                                  { return m_depth; }
	const BoundingBox& GetBounds() const                   { return m_bounds; }

	const TileMap* GetTileMap() const                      { return m_tileMap; }
	VertexBuffer* GetVertices() const                      { return m_vertices; }
	VertexBuffer* GetAlphaVertices() const                 { return m_alphaVertices; }
	uint GetNumVertices() const                            { return m_numVertices; }
	uint GetNumAlphaVertices() const                       { return m_numAlphaVertices; }

private:
	uint GetIndexOf(uint x, uint y, uint z) const;

	Tile *m_data;
	const TileMap *m_tileMap;
	GraphicsDevice *m_graphicsDevice;
	VertexBuffer *m_vertices;
	VertexBuffer *m_alphaVertices;
	uint m_numVertices;
	uint m_numAlphaVertices;

	uint m_x;
	uint m_y;
	uint m_z;
	uint m_width;
	uint m_height;
	uint m_depth;
	BoundingBox m_bounds;
	Vector3 m_position;
};

inline Tile* TileChunk::Get(uint x, uint y, uint z) const
{
	uint index = GetIndexOf(x, y, z);
	return &m_data[index];
}

inline Tile* TileChunk::GetSafe(uint x, uint y, uint z) const
{
	if (!IsWithinLocalBounds((int)x, (int)y, (int)z))
		return NULL;
	else
		return Get(x, y, z);
}

inline uint TileChunk::GetIndexOf(uint x, uint y, uint z) const
{
	return (y * m_width * m_depth) + (z * m_width) + x;
}

inline BOOL TileChunk::IsWithinBounds(int x, int y, int z) const
{
	if (x < (int)GetX() || x >= (int)(GetX() + m_width))
		return FALSE;
	if (y < (int)GetY() || y >= (int)(GetY() + m_height))
		return FALSE;
	if (z < (int)GetZ() || z >= (int)(GetZ() + m_depth))
		return FALSE;

	return TRUE;
}

inline BOOL TileChunk::IsWithinLocalBounds(int x, int y, int z) const
{
	if (x < 0 || x >= (int)m_width)
		return FALSE;
	if (y < 0 || y >= (int)m_height)
		return FALSE;
	if (z < 0 || z >= (int)m_depth)
		return FALSE;

	return TRUE;
}

#endif

