#include "../framework/debug.h"

#include "tile.h"
#include "../framework/common.h"
#include "../framework/math/common.h"
#include "../framework/math/matrix4x4.h"

const Matrix4x4* Tile::GetTransformationMatrix() const
{
	// static so that we only need to set them up once, since this method will
	// be used inside inner loops and such
	// **IMPORTANT**: these rotations assume every TileMesh is modeled facing north!
	static Matrix4x4 faceNorth = Matrix4x4::CreateRotationY(RADIANS_0);
	static Matrix4x4 faceEast = Matrix4x4::CreateRotationY(RADIANS_90);
	static Matrix4x4 faceSouth = Matrix4x4::CreateRotationY(RADIANS_180);
	static Matrix4x4 faceWest = Matrix4x4::CreateRotationY(RADIANS_270);

	if (IsBitSet(TILE_FACE_NORTH, flags))
		return &faceNorth;
	else if (IsBitSet(TILE_FACE_EAST, flags))
		return &faceEast;
	else if (IsBitSet(TILE_FACE_SOUTH, flags))
		return &faceSouth;
	else if (IsBitSet(TILE_FACE_WEST, flags))
		return &faceWest;
	else
		return NULL;
}

