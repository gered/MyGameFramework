#include "simpletilemaplighter.h"
#include "tile.h"
#include "tilechunk.h"
#include "tilelightdefs.h"
#include "tilemap.h"
#include "tilemesh.h"
#include "tilemeshcollection.h"
#include "tilemeshdefs.h"

SimpleTileMapLighter::SimpleTileMapLighter()
{
}

SimpleTileMapLighter::~SimpleTileMapLighter()
{
}

void SimpleTileMapLighter::Light(TileMap *tileMap)
{
	ResetLightValues(tileMap);
	ApplySkyLight(tileMap);
}

void SimpleTileMapLighter::ResetLightValues(TileMap *tileMap)
{
	for (uint y = 0; y < tileMap->GetHeight(); ++y)
	{
		for (uint z = 0; z < tileMap->GetDepth(); ++z)
		{
			for (uint x = 0; x < tileMap->GetWidth(); ++x)
			{
				Tile *tile = tileMap->Get(x, y, z);

				// sky lighting will be recalculated, and other types of light sources
				// info stays as they were
				ClearBit(TILE_LIGHT_SKY, tile->flags);
				tile->skyLight = 0;
				tile->tileLight = tileMap->GetAmbientLightValue();
			}
		}
	}
}

void SimpleTileMapLighter::ApplySkyLight(TileMap *tileMap)
{
	// NOTE: ResetLightValues() clears sky light data in such a way that
	//       doesn't require us to flood-fill 0 light values here for everything
	//       that isn't sky-lit
	// go through each vertical column one at a time from top to bottom
	for (uint x = 0; x < tileMap->GetWidth(); ++x)
	{
		for (uint z = 0; z < tileMap->GetDepth(); ++z)
		{
			bool stillSkyLit = true;
			TILE_LIGHT_VALUE currentSkyLightValue = tileMap->GetSkyLightValue();

			for (int y = tileMap->GetHeight() - 1; y >= 0 && stillSkyLit; --y)
			{
				Tile *tile = tileMap->Get(x, y, z);
				const TileMesh *mesh = tileMap->GetMeshes()->Get(tile);
				if (mesh == NULL || (mesh != NULL && !mesh->IsOpaque(SIDE_TOP) && !mesh->IsOpaque(SIDE_BOTTOM)))
				{
					// tile is partially transparent or this tile is empty space
					SetBit(TILE_LIGHT_SKY, tile->flags);

					if (mesh != NULL)
						Tile::AdjustLightForTranslucency(currentSkyLightValue, mesh->GetTranslucency());

					tile->skyLight = tileMap->GetSkyLightValue();
				}
				else
				{
					// tile is present and is fully solid, sky lighting stops
					// at the tile above this one
					stillSkyLit = false;
				}
			}
		}
	}
}
