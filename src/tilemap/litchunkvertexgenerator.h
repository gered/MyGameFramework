#ifndef __TILEMAP_LITCHUNKVERTEXGENERATOR_H_INCLUDED__
#define __TILEMAP_LITCHUNKVERTEXGENERATOR_H_INCLUDED__

#include "chunkvertexgenerator.h"

class TileChunk;
class VertexBuffer;
struct Color;
struct Matrix4x4;
struct Vector3;

class LitChunkVertexGenerator : public ChunkVertexGenerator
{
public:
	LitChunkVertexGenerator();
	virtual ~LitChunkVertexGenerator();

private:
	void CopyVertex(const TileChunk *chunk, VertexBuffer *sourceBuffer, VertexBuffer *destBuffer, const Vector3 &positionOffset, const Matrix4x4 *transform, const Color &color);
};

#endif

