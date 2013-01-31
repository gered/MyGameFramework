#ifndef __TILEMAP_SIMPLETILEMAPLIGHTER_H_INCLUDED__
#define __TILEMAP_SIMPLETILEMAPLIGHTER_H_INCLUDED__

#include "../framework/common.h"

#include "tile.h"
#include "tilemaplighter.h"

class TileMap;

class SimpleTileMapLighter : public TileMapLighter
{
public:
	SimpleTileMapLighter();
	virtual ~SimpleTileMapLighter();

	void Light(TileMap *tileMap);

private:
	void ResetLightValues(TileMap *tileMap);
	void ApplySkyLight(TileMap *tileMap);
};

#endif

