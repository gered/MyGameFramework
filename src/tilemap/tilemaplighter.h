#ifndef __TILEMAP_TILEMAPLIGHTER_H_INCLUDED__
#define __TILEMAP_TILEMAPLIGHTER_H_INCLUDED__

class TileMap;

class TileMapLighter
{
public:
	TileMapLighter()                                       {}
	virtual ~TileMapLighter()                              {}

	virtual void Light(TileMap *tileMap) = 0;
};

#endif

