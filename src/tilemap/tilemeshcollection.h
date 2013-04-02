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

	uint Add(const StaticMesh *mesh, uint textureIndex, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue = 0, bool alpha = false, float translucency = 1.0f, const Color &color = COLOR_WHITE);
	uint Add(const StaticMesh *mesh, const StaticMesh *collisionMesh, uint textureIndex, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue = 0, bool alpha = false, float translucency = 1.0f, const Color &color = COLOR_WHITE);
	uint Add(const StaticMesh *mesh, uint *textureIndexes, uint numIndexes, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue = 0, bool alpha = false, float translucency = 1.0f, const Color &color = COLOR_WHITE);
	uint Add(const StaticMesh *mesh, const StaticMesh *collisionMesh, uint *textureIndexes, uint numIndexes, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue = 0, bool alpha = false, float translucency = 1.0f, const Color &color = COLOR_WHITE);
	uint AddCube(uint textureIndex, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue = 0, bool alpha = false, float translucency = 1.0f, const Color &color = COLOR_WHITE);

	TileMesh* Get(const Tile *tile) const                  { return m_meshes[tile->tile]; }
	TileMesh* Get(uint index) const                        { return m_meshes[index]; }
	uint GetCount() const                                  { return m_meshes.size(); }

private:
	uint AddMesh(TileMesh *mesh);

	const TextureAtlas *m_textureAtlas;
	TileMeshList m_meshes;
};

#endif
