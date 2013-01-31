#ifndef __TILEMAP_TILEMESHCOLLECTION_H_INCLUDED__
#define __TILEMAP_TILEMESHCOLLECTION_H_INCLUDED__

#include "../framework/common.h"
#include "../framework/graphics/color.h"
#include "tile.h"
#include "tilelightdefs.h"
#include "tilemeshdefs.h"
#include <stl/vector.h>

class StaticMesh;
class TextureAtlas;
class TileMesh;

typedef stl::vector<TileMesh*> TileMeshList;

class TileMeshCollection
{
public:
	TileMeshCollection(const TextureAtlas *textureAtlas);
	virtual ~TileMeshCollection();

	const TextureAtlas* GetTextureAtlas() const            { return m_textureAtlas; }

	uint32_t Add(const StaticMesh *mesh, uint32_t textureIndex, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue = 0, BOOL alpha = FALSE, float translucency = 1.0f, const Color &color = COLOR_WHITE);
	uint32_t Add(const StaticMesh *mesh, const StaticMesh *collisionMesh, uint32_t textureIndex, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue = 0, BOOL alpha = FALSE, float translucency = 1.0f, const Color &color = COLOR_WHITE);
	uint32_t Add(const StaticMesh *mesh, uint32_t *textureIndexes, uint32_t numIndexes, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue = 0, BOOL alpha = FALSE, float translucency = 1.0f, const Color &color = COLOR_WHITE);
	uint32_t Add(const StaticMesh *mesh, const StaticMesh *collisionMesh, uint32_t *textureIndexes, uint32_t numIndexes, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue = 0, BOOL alpha = FALSE, float translucency = 1.0f, const Color &color = COLOR_WHITE);
	uint32_t AddCube(uint32_t textureIndex, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue = 0, BOOL alpha = FALSE, float translucency = 1.0f, const Color &color = COLOR_WHITE);

	TileMesh* Get(const Tile *tile) const                  { return m_meshes[tile->tile]; }
	TileMesh* Get(uint32_t index) const                    { return m_meshes[index]; }
	uint32_t GetCount() const                              { return m_meshes.size(); }

private:
	uint32_t AddMesh(TileMesh *mesh);

	const TextureAtlas *m_textureAtlas;
	TileMeshList m_meshes;
};

#endif
