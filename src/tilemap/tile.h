#ifndef __TILEMAP_TILE_H_INCLUDED__
#define __TILEMAP_TILE_H_INCLUDED__

#include "../framework/common.h"
#include "tilelightdefs.h"
#include "../framework/graphics/color.h"
#include "../framework/math/mathhelpers.h"
#include <math.h>

struct Matrix4x4;

typedef uint16_t TILE_INDEX;
typedef uint16_t TILE_FLAG_BITS;

const TILE_INDEX NO_TILE = 0;

enum TILE_FLAGS
{
	TILE_COLLIDABLE = 1,
	TILE_FACE_NORTH = 2,
	TILE_FACE_EAST = 4,
	TILE_FACE_SOUTH = 8,
	TILE_FACE_WEST = 16,
	TILE_CUSTOM_COLOR = 32,
	TILE_FRICTION_SLIPPERY = 64,
	TILE_LIGHT_SKY = 128,
	TILE_WALKABLE_SURFACE = 256
};

struct Tile
{
	Tile();

	void Set(TILE_INDEX tile);
	void Set(TILE_INDEX tile, TILE_FLAG_BITS flags);
	void Set(TILE_INDEX tile, TILE_FLAG_BITS flags, const Color &color);
	void Set(TILE_INDEX tile, TILE_FLAG_BITS flags, uint32_t color);
	const Matrix4x4* GetTransformationMatrix() const;

	void SetCustomColor(const Color &color);
	void SetCustomColor(uint32_t color);
	void ClearCustomColor();

	float GetBrightness() const;
	static float GetBrightness(TILE_LIGHT_VALUE light);
	static TILE_LIGHT_VALUE AdjustLightForTranslucency(TILE_LIGHT_VALUE light, float translucency);

	BOOL IsEmptySpace() const;
	BOOL IsCollideable() const;
	BOOL HasCustomColor() const;
	BOOL IsSlippery() const;
	BOOL IsSkyLit() const;

	TILE_INDEX tile;
	TILE_FLAG_BITS flags;
	TILE_LIGHT_VALUE tileLight;
	TILE_LIGHT_VALUE skyLight;
	uint32_t color;     // save some memory (4 byte int, versus 16 byte Color obj)
};

inline Tile::Tile()
{
	tile = NO_TILE;
	flags = 0;
	tileLight = 0;
	skyLight = 0;
	color = 0;
}

inline void Tile::Set(TILE_INDEX tile)
{
	this->tile = tile;
}

inline void Tile::Set(TILE_INDEX tile, TILE_FLAG_BITS flags)
{
	this->tile = tile;
	this->flags = flags;
}

inline void Tile::Set(TILE_INDEX tile, TILE_FLAG_BITS flags, const Color &color)
{
	Set(tile, flags, color.ToInt());
}

inline void Tile::Set(TILE_INDEX tile, TILE_FLAG_BITS flags, uint32_t color)
{
	SetBit(TILE_CUSTOM_COLOR, flags);

	this->tile = tile;
	this->flags = flags;
	this->color = color;
}

inline void Tile::SetCustomColor(const Color &color)
{
	SetCustomColor(color.ToInt());
}

inline void Tile::SetCustomColor(uint32_t color)
{
	SetBit(TILE_CUSTOM_COLOR, this->flags);
	this->color = color;
}

inline void Tile::ClearCustomColor()
{
	ClearBit(TILE_CUSTOM_COLOR, this->flags);
	this->color = 0;
}

inline float Tile::GetBrightness() const
{
	if (this->tileLight > this->skyLight)
		return GetBrightness(this->tileLight);
	else
		return GetBrightness(this->skyLight);
}

inline float Tile::GetBrightness(TILE_LIGHT_VALUE light)
{
	// this is a copy of the brightness formula listed here:
	// http://gamedev.stackexchange.com/a/21247

	const float BASE_BRIGHTNESS = 0.086f;
	float normalizedLightValue = (float)light / (float)(TILE_LIGHT_VALUE_MAX + 1);
	return powf(normalizedLightValue, 1.4f) + BASE_BRIGHTNESS;
}

inline TILE_LIGHT_VALUE Tile::AdjustLightForTranslucency(TILE_LIGHT_VALUE light, float translucency)
{
	return (TILE_LIGHT_VALUE)Round((float)light * translucency);
}

inline BOOL Tile::IsEmptySpace() const
{
	return this->tile == NO_TILE;
}

inline BOOL Tile::IsCollideable() const
{
	return IsBitSet(TILE_COLLIDABLE, this->flags);
}

inline BOOL Tile::HasCustomColor() const
{
	return IsBitSet(TILE_CUSTOM_COLOR, this->flags);
}

inline BOOL Tile::IsSlippery() const
{
	return IsBitSet(TILE_FRICTION_SLIPPERY, this->flags);
}

inline BOOL Tile::IsSkyLit() const
{
	return IsBitSet(TILE_LIGHT_SKY, this->flags);
}

#endif

