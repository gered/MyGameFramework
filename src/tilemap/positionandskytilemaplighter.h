#ifndef __TILEMAP_POSITIONANDSKYTILEMAPLIGHTER_H_INCLUDED__
#define __TILEMAP_POSITIONANDSKYTILEMAPLIGHTER_H_INCLUDED__

#include "../framework/common.h"

#include "tile.h"
#include "tilelightdefs.h"
#include "tilemaplighter.h"

class TileMap;

class PositionAndSkyTileMapLighter : public TileMapLighter
{
public:
	PositionAndSkyTileMapLighter();
	virtual ~PositionAndSkyTileMapLighter();

	void Light(TileMap *tileMap);

private:
	void ResetLightValues(TileMap *tileMap);
	void SetupSkyLight(TileMap *tileMap);
	void ApplyLighting(TileMap *tileMap);
	void SpreadSkyLight(int x, int y, int z, Tile *tile, TILE_LIGHT_VALUE light, TileMap *tileMap);
	void SpreadTileLight(int x, int y, int z, Tile *tile, TILE_LIGHT_VALUE light, TileMap *tileMap);
};

#endif

