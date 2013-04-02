#ifndef __TILEMAP_CHUNKVERTEXGENERATOR_H_INCLUDED__
#define __TILEMAP_CHUNKVERTEXGENERATOR_H_INCLUDED__

#include "../framework/common.h"

class CubeTileMesh;
class StaticTileMesh;
class TileMesh;
class VertexBuffer;
struct Color;
struct Matrix4x4;
struct Point3;
struct Vector3;

class TileChunk;

class ChunkVertexGenerator
{
public:
	ChunkVertexGenerator();
	virtual ~ChunkVertexGenerator();

	void Generate(TileChunk *chunk, uint &numVertices, uint &numAlphaVertices);

private:
	uint AddMesh(const TileMesh *mesh, TileChunk *chunk, BOOL isAlpha, const Point3 &position, const Matrix4x4 *transform, const Color &color, uint firstVertex, uint numVertices);
	virtual void CopyVertex(const TileChunk *chunk, VertexBuffer *sourceBuffer, VertexBuffer *destBuffer, const Vector3 &positionOffset, const Matrix4x4 *transform, const Color &color);
};

#endif

