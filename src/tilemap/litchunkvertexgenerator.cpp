#include "litchunkvertexgenerator.h"
#include "tile.h"
#include "tilechunk.h"
#include "tilelightdefs.h"
#include "tilemap.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/vertexbuffer.h"
#include "../framework/math/matrix4x4.h"
#include "../framework/math/vector3.h"

LitChunkVertexGenerator::LitChunkVertexGenerator()
{
}

LitChunkVertexGenerator::~LitChunkVertexGenerator()
{
}

void LitChunkVertexGenerator::CopyVertex(const TileChunk *chunk, VertexBuffer *sourceBuffer, VertexBuffer *destBuffer, const Vector3 &positionOffset, const Matrix4x4 *transform, const Color &color)
{
	// figure out what the default lighting value is for this chunk
	TILE_LIGHT_VALUE defaultLightValue = chunk->GetTileMap()->GetSkyLightValue();
	if (chunk->GetTileMap()->GetAmbientLightValue() > defaultLightValue)
		defaultLightValue = chunk->GetTileMap()->GetAmbientLightValue();

	Vector3 v = sourceBuffer->GetCurrentPosition3();
	Vector3 n = sourceBuffer->GetCurrentNormal();

	if (transform != NULL)
	{
		// need to transform the vertex + normal first before copying it
		v = v * (*transform);
		n = n * (*transform);
	}

	// translate vertex into "tilemap space"
	v += positionOffset;

	destBuffer->SetCurrentPosition3(v);
	destBuffer->SetCurrentNormal(n);

	// just directly copy the tex coord as-is
	destBuffer->SetCurrentTexCoord(sourceBuffer->GetCurrentTexCoord());

	// the color we set to the destination determines the brightness (lighting)

	// use the tile that's adjacent to this one in the direction that
	// this vertex's normal is pointing as the light source
	Vector3 lightSource = positionOffset + n;

	// if the light source position is off the bounds of the entire world
	// then use the default light value.
	// the below call to TileChunk::GetWithinSelfOrNeighbour() actually does
	// do bounds checking, but we would need to cast from float to int 
	// first. this causes some issues when the one or more of the 
	// lightSource x/y/z values are between 0 and -1 (rounds up to 0 when 
	// using a cast). rather then do some weird custom rounding, we just 
	// check for negatives to ensure we catch them and handle it properly
	// NOTE: this is only a problem currently because world coords are
	//       always >= 0. this will need to be adjusted if that changes
	float brightness;
	if (lightSource.x < 0.0f || lightSource.y < 0.0f || lightSource.z < 0.0f)
		brightness = Tile::GetBrightness(defaultLightValue);
	else
	{
		// light source is within the boundaries of the world, get the
		// actual tile (may or may not be in a neighbouring chunk)
		int lightX = (int)lightSource.x - chunk->GetX();
		int lightY = (int)lightSource.y - chunk->GetY();
		int lightZ = (int)lightSource.z - chunk->GetZ();

		const Tile *lightTile = chunk->GetWithinSelfOrNeighbourSafe(lightX, lightY, lightZ);
		if (lightTile == NULL)
			brightness = Tile::GetBrightness(defaultLightValue);
		else
			brightness = lightTile->GetBrightness();
	}

	Color resultingColor;
	resultingColor.r = color.r * brightness;
	resultingColor.g = color.g * brightness;
	resultingColor.b = color.b * brightness;
	resultingColor.a = color.a;

	destBuffer->SetCurrentColor(resultingColor);
}

