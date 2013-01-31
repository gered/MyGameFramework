#ifndef __TILEMAP_STATICTILEMESH_H_INCLUDED__
#define __TILEMAP_STATICTILEMESH_H_INCLUDED__

#include "../framework/common.h"
#include "tilelightdefs.h"
#include "tilemesh.h"
#include "tilemeshdefs.h"
#include "../framework/graphics/color.h"

class StaticMesh;
class VertexBuffer;
struct RectF;
struct Vector3;

class StaticTileMesh : public TileMesh
{
public:
	StaticTileMesh(const StaticMesh *mesh, const RectF *textureAtlasTileBoundaries, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, BOOL alpha, float translucency = 1.0f, const Color &color = COLOR_WHITE, const StaticMesh *collisionMesh = NULL);
	StaticTileMesh(const StaticMesh *mesh, const RectF *textureAtlasTileBoundaries, uint32_t numTiles, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, BOOL alpha, float translucency = 1.0f, const Color &color = COLOR_WHITE, const StaticMesh *collisionMesh = NULL);
	virtual ~StaticTileMesh();

	VertexBuffer* GetBuffer() const                        { return m_vertices; }
	uint32_t GetNumCollisionVertices() const               { return m_numCollisionVertices; }
	const Vector3* GetCollisionVertices() const            { return m_collisionVertices; }

	TILEMESH_TYPE GetType() const                          { return TILEMESH_STATIC; }

private:
	void SetupCollisionVertices(const StaticMesh *collisionMesh);

	VertexBuffer *m_vertices;
	uint32_t m_numCollisionVertices;
	Vector3 *m_collisionVertices;
};

#endif

