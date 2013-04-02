#include "../framework/debug.h"

#include "tilemeshcollection.h"
#include "cubetilemesh.h"
#include "statictilemesh.h"
#include "tilelightdefs.h"
#include "tilemesh.h"
#include "tilemeshdefs.h"
#include "../framework/assets/static/staticmesh.h"
#include "../framework/graphics/color.h"
#include "../framework/graphics/textureatlas.h"

TileMeshCollection::TileMeshCollection(const TextureAtlas *textureAtlas)
{
	ASSERT(textureAtlas != NULL);
	m_textureAtlas = textureAtlas;

	// the first mesh (index = 0) should always be a NULL one as this has special meaning
	// in other TileMap-related objects
	AddMesh(NULL);
}

TileMeshCollection::~TileMeshCollection()
{
	for (uint i = 0; i < m_meshes.size(); ++i)
	{
		TileMesh *mesh = m_meshes[i];
		SAFE_DELETE(mesh);
	}
}

uint TileMeshCollection::Add(const StaticMesh *mesh, uint textureIndex, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, bool alpha, float translucency, const Color &color)
{
	return Add(mesh, (StaticMesh*)NULL, textureIndex, opaqueSides, lightValue, alpha, translucency, color);
}

uint TileMeshCollection::Add(const StaticMesh *mesh, const StaticMesh *collisionMesh, uint textureIndex, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, bool alpha, float translucency, const Color &color)
{
	StaticTileMesh *tileMesh = new StaticTileMesh(mesh, &m_textureAtlas->GetTile(textureIndex).texCoords, opaqueSides, lightValue, alpha, translucency, color, collisionMesh);
	ASSERT(tileMesh != NULL);

	return AddMesh(tileMesh);
}

uint TileMeshCollection::Add(const StaticMesh *mesh, uint *textureIndexes, uint numIndexes, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, bool alpha, float translucency, const Color &color)
{
	return Add(mesh, (StaticMesh*)NULL, textureIndexes, numIndexes, opaqueSides, lightValue, alpha, translucency, color);
}

uint TileMeshCollection::Add(const StaticMesh *mesh, const StaticMesh *collisionMesh, uint *textureIndexes, uint numIndexes, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, bool alpha, float translucency, const Color &color)
{
	ASSERT(numIndexes > 0);
	RectF *tileBoundaries = new RectF[numIndexes];
	ASSERT(tileBoundaries != NULL);

	for (uint i = 0; i < numIndexes; ++i)
		tileBoundaries[i] = m_textureAtlas->GetTile(textureIndexes[i]).texCoords;

	StaticTileMesh *tileMesh = new StaticTileMesh(mesh, tileBoundaries, numIndexes, opaqueSides, lightValue, alpha, translucency, color, collisionMesh);
	ASSERT(tileMesh != NULL);

	SAFE_DELETE_ARRAY(tileBoundaries);

	return AddMesh(tileMesh);
}

uint TileMeshCollection::AddCube(uint textureIndex, MESH_SIDES opaqueSides, TILE_LIGHT_VALUE lightValue, bool alpha, float translucency, const Color &color)
{
	CubeTileMesh *tileMesh = new CubeTileMesh(SIDE_ALL, &m_textureAtlas->GetTile(textureIndex).texCoords, opaqueSides, lightValue, alpha, translucency, color);
	ASSERT(tileMesh != NULL);

	return AddMesh(tileMesh);
}

uint TileMeshCollection::AddMesh(TileMesh *mesh)
{
	m_meshes.push_back(mesh);
	return m_meshes.size() - 1;
}
