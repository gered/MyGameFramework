#include "../framework/debug.h"

#include "positionandskytilemaplighter.h"
#include "tile.h"
#include "tilechunk.h"
#include "tilelightdefs.h"
#include "tilemap.h"
#include "tilemesh.h"
#include "tilemeshcollection.h"
#include "tilemeshdefs.h"

PositionAndSkyTileMapLighter::PositionAndSkyTileMapLighter()
{
	STACK_TRACE;
}

PositionAndSkyTileMapLighter::~PositionAndSkyTileMapLighter()
{
	STACK_TRACE;
}

void PositionAndSkyTileMapLighter::Light(TileMap *tileMap)
{
	STACK_TRACE;
	ResetLightValues(tileMap);
	SetupSkyLight(tileMap);
	ApplyLighting(tileMap);
}

void PositionAndSkyTileMapLighter::ResetLightValues(TileMap *tileMap)
{
	STACK_TRACE;
	for (uint32_t y = 0; y < tileMap->GetHeight(); ++y)
	{
		for (uint32_t z = 0; z < tileMap->GetDepth(); ++z)
		{
			for (uint32_t x = 0; x < tileMap->GetWidth(); ++x)
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

void PositionAndSkyTileMapLighter::SetupSkyLight(TileMap *tileMap)
{
	STACK_TRACE;
	// NOTE: ResetLightValues() clears sky light data in such a way that
	//       doesn't require us to flood-fill 0 light values here for everything
	//       that isn't sky-lit
	// go through each vertical column one at a time from top to bottom
	for (uint32_t x = 0; x < tileMap->GetWidth(); ++x)
	{
		for (uint32_t z = 0; z < tileMap->GetDepth(); ++z)
		{
			BOOL stillSkyLit = TRUE;
			TILE_LIGHT_VALUE currentSkyLightValue = tileMap->GetSkyLightValue();

			for (int32_t y = tileMap->GetHeight() - 1; y >= 0 && stillSkyLit; --y)
			{
				Tile *tile = tileMap->Get(x, y, z);
				const TileMesh *mesh = tileMap->GetMeshes()->Get(tile);
				if (mesh == NULL || (mesh != NULL && !mesh->IsOpaque(SIDE_TOP) && !mesh->IsOpaque(SIDE_BOTTOM)))
				{
					// tile is partially transparent or this tile is empty space
					SetBit(TILE_LIGHT_SKY, tile->flags);

					if (mesh != NULL)
						Tile::AdjustLightForTranslucency(currentSkyLightValue, mesh->GetTranslucency());
					
					tile->skyLight = currentSkyLightValue;
				}
				else
				{
					// tile is present and is fully solid, sky lighting stops
					// at the tile above this one
					stillSkyLit = FALSE;
				}
			}
		}
	}
}

void PositionAndSkyTileMapLighter::ApplyLighting(TileMap *tileMap)
{
	STACK_TRACE;
	// for each light source (sky or not), recursively go through and set
	// appropriate lighting for each adjacent tile
	for (uint32_t y = 0; y < tileMap->GetHeight(); ++y)
	{
		for (uint32_t z = 0; z < tileMap->GetDepth(); ++z)
		{
			for (uint32_t x = 0; x < tileMap->GetWidth(); ++x)
			{
				Tile *tile = tileMap->Get(x, y, z);
				if (tile->IsEmptySpace())
				{
					if (tile->IsSkyLit())
						SpreadSkyLight(x, y, z, tile, tile->skyLight, tileMap);
				}
				else
				{
					const TileMesh *mesh = tileMap->GetMeshes()->Get(tile);
					if (mesh->IsLightSource())
						SpreadTileLight(x, y, z, tile, mesh->GetLightValue(), tileMap);
				}
			}
		}
	}
}

void PositionAndSkyTileMapLighter::SpreadSkyLight(int32_t x, int32_t y, int32_t z, Tile *tile, TILE_LIGHT_VALUE light, TileMap *tileMap)
{
	if (light > 0)
	{
		tile->skyLight = light;
		--light;

		Tile *left = tileMap->GetSafe(x - 1, y, z);
		Tile *right = tileMap->GetSafe(x + 1, y, z);
		Tile *forward = tileMap->GetSafe(x, y, z - 1);
		Tile *backward = tileMap->GetSafe(x, y, z + 1);
		Tile *up = tileMap->GetSafe(x, y + 1, z);
		Tile *down = tileMap->GetSafe(x, y - 1, z);

		if (left != NULL && (left->IsEmptySpace() || !tileMap->GetMeshes()->Get(left)->IsOpaque(SIDE_RIGHT)) && left->skyLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!left->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(left)->GetTranslucency());
			SpreadSkyLight(x - 1, y, z, left, spreadLight, tileMap);
		}
		if (right != NULL && (right->IsEmptySpace() || !tileMap->GetMeshes()->Get(right)->IsOpaque(SIDE_LEFT)) && right->skyLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!right->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(right)->GetTranslucency());
			SpreadSkyLight(x + 1, y, z, right, spreadLight, tileMap);
		}
		if (forward != NULL && (forward->IsEmptySpace() || !tileMap->GetMeshes()->Get(forward)->IsOpaque(SIDE_BACK)) && forward->skyLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!forward->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(forward)->GetTranslucency());
			SpreadSkyLight(x, y, z - 1, forward, spreadLight, tileMap);
		}
		if (backward != NULL && (backward->IsEmptySpace() || !tileMap->GetMeshes()->Get(backward)->IsOpaque(SIDE_FRONT)) && backward->skyLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!backward->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(backward)->GetTranslucency());
			SpreadSkyLight(x, y, z + 1, backward, spreadLight, tileMap);
		}
		if (up != NULL && (up->IsEmptySpace() || !tileMap->GetMeshes()->Get(up)->IsOpaque(SIDE_BOTTOM)) && up->skyLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!up->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(up)->GetTranslucency());
			SpreadSkyLight(x, y + 1, z, up, spreadLight, tileMap);
		}
		if (down != NULL && (down->IsEmptySpace() || !tileMap->GetMeshes()->Get(down)->IsOpaque(SIDE_TOP)) && down->skyLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!down->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(down)->GetTranslucency());
			SpreadSkyLight(x, y - 1, z, down, spreadLight, tileMap);
		}
	}
}

void PositionAndSkyTileMapLighter::SpreadTileLight(int32_t x, int32_t y, int32_t z, Tile *tile, TILE_LIGHT_VALUE light, TileMap *tileMap)
{
	if (light > 0)
	{
		tile->tileLight = light;
		--light;

		Tile *left = tileMap->GetSafe(x - 1, y, z);
		Tile *right = tileMap->GetSafe(x + 1, y, z);
		Tile *forward = tileMap->GetSafe(x, y, z - 1);
		Tile *backward = tileMap->GetSafe(x, y, z + 1);
		Tile *up = tileMap->GetSafe(x, y + 1, z);
		Tile *down = tileMap->GetSafe(x, y - 1, z);

		if (left != NULL && (left->IsEmptySpace() || !tileMap->GetMeshes()->Get(left)->IsOpaque(SIDE_RIGHT)) && left->tileLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!left->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(left)->GetTranslucency());
			SpreadTileLight(x - 1, y, z, left, spreadLight, tileMap);
		}
		if (right != NULL && (right->IsEmptySpace() || !tileMap->GetMeshes()->Get(right)->IsOpaque(SIDE_LEFT)) && right->tileLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!right->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(right)->GetTranslucency());
			SpreadTileLight(x + 1, y, z, right, spreadLight, tileMap);
		}
		if (forward != NULL && (forward->IsEmptySpace() || !tileMap->GetMeshes()->Get(forward)->IsOpaque(SIDE_BACK)) && forward->tileLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!forward->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(forward)->GetTranslucency());
			SpreadTileLight(x, y, z - 1, forward, spreadLight, tileMap);
		}
		if (backward != NULL && (backward->IsEmptySpace() || !tileMap->GetMeshes()->Get(backward)->IsOpaque(SIDE_FRONT)) && backward->tileLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!backward->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(backward)->GetTranslucency());
			SpreadTileLight(x, y, z + 1, backward, spreadLight, tileMap);
		}
		if (up != NULL && (up->IsEmptySpace() || !tileMap->GetMeshes()->Get(up)->IsOpaque(SIDE_BOTTOM)) && up->tileLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!up->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(up)->GetTranslucency());
			SpreadTileLight(x, y + 1, z, up, spreadLight, tileMap);
		}
		if (down != NULL && (down->IsEmptySpace() || !tileMap->GetMeshes()->Get(down)->IsOpaque(SIDE_TOP)) && down->tileLight < light)
		{
			TILE_LIGHT_VALUE spreadLight = light;
			if (!down->IsEmptySpace())
				spreadLight = Tile::AdjustLightForTranslucency(spreadLight, tileMap->GetMeshes()->Get(down)->GetTranslucency());
			SpreadTileLight(x, y - 1, z, down, spreadLight, tileMap);
		}
	}
}

