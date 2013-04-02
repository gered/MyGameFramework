#ifndef __TILEMAP_TILEMESHDEFS_H_INCLUDED__
#define __TILEMAP_TILEMESHDEFS_H_INCLUDED__

#include "../framework/common.h"

// random place for global-ish mesh definitions and constants

enum SIDES
{
	SIDE_TOP = 1,
	SIDE_BOTTOM = 2,
	SIDE_FRONT = 4,
	SIDE_BACK = 8,
	SIDE_LEFT = 16,
	SIDE_RIGHT = 32,
	SIDE_ALL = 255
};

typedef uint8_t CUBE_FACES;
typedef uint8_t MESH_SIDES;

const uint CUBE_VERTICES_PER_FACE = 6;

#endif

