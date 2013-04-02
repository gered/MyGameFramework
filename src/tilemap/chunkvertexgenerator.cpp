#include "../framework/debug.h"
#include "../framework/common.h"

#include "chunkvertexgenerator.h"

#include "../framework/graphics/color.h"
#include "../framework/graphics/vertexbuffer.h"
#include "../framework/math/matrix4x4.h"
#include "../framework/math/point3.h"
#include "../framework/math/vector3.h"
#include "cubetilemesh.h"
#include "tilechunk.h"
#include "tilemap.h"
#include "tilemesh.h"
#include "tilemeshcollection.h"
#include "tilemeshdefs.h"

ChunkVertexGenerator::ChunkVertexGenerator()
{
}

ChunkVertexGenerator::~ChunkVertexGenerator()
{
}

void ChunkVertexGenerator::Generate(TileChunk *chunk, uint &numVertices, uint &numAlphaVertices)
{
	numVertices = 0;
	numAlphaVertices = 0;

	chunk->GetVertices()->MoveToStart();
	if (chunk->IsAlphaEnabled())
		chunk->GetAlphaVertices()->MoveToStart();

	const TileMap *tileMap = chunk->GetTileMap();

	for (uint y = 0; y < chunk->GetHeight(); ++y)
	{
		for (uint z = 0; z < chunk->GetDepth(); ++z)
		{
			for (uint x = 0; x < chunk->GetWidth(); ++x)
			{
				Tile *tile = chunk->Get(x, y, z);
				if (tile->tile == NO_TILE)
					continue;

				const TileMesh *mesh = chunk->GetTileMap()->GetMeshes()->Get(tile);

				// enable alpha for this chunk if this tile has an alpha-enabled
				// mesh and we haven't done that already
				// TODO: optimize so that we only do this if the tile is visible
				if (mesh->IsAlpha())
				{
					if (!chunk->IsAlphaEnabled())
						chunk->EnableAlphaVertices(true);
				}

				// "tilemap space" position that this tile is at
				Point3 position;
				position.x = x + (int)chunk->GetPosition().x;
				position.y = y + (int)chunk->GetPosition().y;
				position.z = z + (int)chunk->GetPosition().z;

				const Matrix4x4 *transform = tile->GetTransformationMatrix();

				// tile color
				Color color;
				if (tile->HasCustomColor())
					color = Color::FromInt(tile->color);
				else
					color = mesh->GetColor();

				if (mesh->GetType() == TILEMESH_CUBE)
				{
					CubeTileMesh *cubeMesh = (CubeTileMesh*)mesh;

					// determine what's next to each cube face
					Tile *left = chunk->GetWithinSelfOrNeighbourSafe(x - 1, y, z);
					Tile *right = chunk->GetWithinSelfOrNeighbourSafe(x + 1, y, z);
					Tile *forward = chunk->GetWithinSelfOrNeighbourSafe(x, y, z - 1);
					Tile *backward = chunk->GetWithinSelfOrNeighbourSafe(x, y, z + 1);
					Tile *down = chunk->GetWithinSelfOrNeighbourSafe(x, y - 1, z);
					Tile *up = chunk->GetWithinSelfOrNeighbourSafe(x, y + 1, z);

					// evaluate each face's visibility and add it's vertices if needed one at a time
					if ((left == NULL || left->tile == NO_TILE || !tileMap->GetMeshes()->Get(left)->IsOpaque(SIDE_RIGHT)) && cubeMesh->HasFace(SIDE_LEFT))
					{
						// left face is visible
						if (cubeMesh->IsAlpha())
							numAlphaVertices += AddMesh(cubeMesh, chunk, true, position, transform, color, cubeMesh->GetLeftFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
						else
							numVertices += AddMesh(cubeMesh, chunk, false, position, transform, color, cubeMesh->GetLeftFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
					}
					if ((right == NULL || right->tile == NO_TILE || !tileMap->GetMeshes()->Get(right)->IsOpaque(SIDE_LEFT)) && cubeMesh->HasFace(SIDE_RIGHT))
					{
						// right face is visible
						if (cubeMesh->IsAlpha())
							numAlphaVertices += AddMesh(cubeMesh, chunk, true, position, transform, color, cubeMesh->GetRightFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
						else
							numVertices += AddMesh(cubeMesh, chunk, false, position, transform, color, cubeMesh->GetRightFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
					}
					if ((forward == NULL || forward->tile == NO_TILE || !tileMap->GetMeshes()->Get(forward)->IsOpaque(SIDE_BACK)) && cubeMesh->HasFace(SIDE_FRONT))
					{
						// front face is visible
						if (cubeMesh->IsAlpha())
							numAlphaVertices += AddMesh(cubeMesh, chunk, true, position, transform, color, cubeMesh->GetFrontFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
						else
							numVertices += AddMesh(cubeMesh, chunk, false, position, transform, color, cubeMesh->GetFrontFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
					}
					if ((backward == NULL || backward->tile == NO_TILE || !tileMap->GetMeshes()->Get(backward)->IsOpaque(SIDE_FRONT)) && cubeMesh->HasFace(SIDE_BACK))
					{
						// back face is visible
						if (cubeMesh->IsAlpha())
							numAlphaVertices += AddMesh(cubeMesh, chunk, true, position, transform, color, cubeMesh->GetBackFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
						else
							numVertices += AddMesh(cubeMesh, chunk, false, position, transform, color, cubeMesh->GetBackFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
					}
					if ((down == NULL || down->tile == NO_TILE || !tileMap->GetMeshes()->Get(down)->IsOpaque(SIDE_TOP)) && cubeMesh->HasFace(SIDE_BOTTOM))
					{
						// bottom face is visible
						if (cubeMesh->IsAlpha())
							numAlphaVertices += AddMesh(cubeMesh, chunk, true, position, transform, color, cubeMesh->GetBottomFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
						else
							numVertices += AddMesh(cubeMesh, chunk, false, position, transform, color, cubeMesh->GetBottomFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
					}
					if ((up == NULL || up->tile == NO_TILE || !tileMap->GetMeshes()->Get(up)->IsOpaque(SIDE_BOTTOM)) && cubeMesh->HasFace(SIDE_TOP))
					{
						// top face is visible
						if (cubeMesh->IsAlpha())
							numAlphaVertices += AddMesh(cubeMesh, chunk, true, position, transform, color, cubeMesh->GetTopFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
						else
							numVertices += AddMesh(cubeMesh, chunk, false, position, transform, color, cubeMesh->GetTopFaceVertexOffset(), CUBE_VERTICES_PER_FACE);
					}
				}
				else
				{
					bool visible = false;

					// visibility determination. we check for at least one 
					// adjacent empty space / non-opaque tile
					Tile *left = chunk->GetWithinSelfOrNeighbourSafe(x - 1, y, z);
					Tile *right = chunk->GetWithinSelfOrNeighbourSafe(x + 1, y, z);
					Tile *forward = chunk->GetWithinSelfOrNeighbourSafe(x, y, z - 1);
					Tile *backward = chunk->GetWithinSelfOrNeighbourSafe(x, y, z + 1);
					Tile *down = chunk->GetWithinSelfOrNeighbourSafe(x, y - 1, z);
					Tile *up = chunk->GetWithinSelfOrNeighbourSafe(x, y + 1, z);

					// NULL == empty space (off the edge of the entire map)
					if (
						(left == NULL || left->tile == NO_TILE || !tileMap->GetMeshes()->Get(left)->IsOpaque(SIDE_RIGHT)) ||
						(right == NULL || right->tile == NO_TILE || !tileMap->GetMeshes()->Get(right)->IsOpaque(SIDE_LEFT)) ||
						(forward == NULL || forward->tile == NO_TILE || !tileMap->GetMeshes()->Get(forward)->IsOpaque(SIDE_BACK)) ||
						(backward == NULL || backward->tile == NO_TILE || !tileMap->GetMeshes()->Get(backward)->IsOpaque(SIDE_FRONT)) ||
						(up == NULL || up->tile == NO_TILE || !tileMap->GetMeshes()->Get(up)->IsOpaque(SIDE_BOTTOM)) ||
						(down == NULL || down->tile == NO_TILE || !tileMap->GetMeshes()->Get(down)->IsOpaque(SIDE_TOP))
						)
						visible = true;

					if (visible)
					{
						if (mesh->IsAlpha())
							numAlphaVertices += AddMesh(mesh, chunk, true, position, transform, color, 0, mesh->GetBuffer()->GetNumElements());
						else
							numVertices += AddMesh(mesh, chunk, false, position, transform, color, 0, mesh->GetBuffer()->GetNumElements());
					}
				}
			}
		}
	}

	if (numAlphaVertices == 0)
		chunk->EnableAlphaVertices(false);
}

uint ChunkVertexGenerator::AddMesh(const TileMesh *mesh, TileChunk *chunk, bool isAlpha, const Point3 &position, const Matrix4x4 *transform, const Color &color, uint firstVertex, uint numVertices)
{
	VertexBuffer *sourceBuffer = mesh->GetBuffer();
	sourceBuffer->MoveToStart();

	VertexBuffer *destBuffer;
	if (isAlpha)
		destBuffer = chunk->GetAlphaVertices();
	else
		destBuffer = chunk->GetVertices();

	ASSERT(firstVertex < sourceBuffer->GetNumElements());
	ASSERT((firstVertex + numVertices - 1) < sourceBuffer->GetNumElements());

	// ensure there is enough space in the destination buffer
	uint verticesToAdd = numVertices;
	if (destBuffer->GetRemainingSpace() < verticesToAdd)
	{
		// not enough space, need to resize the destination buffer
		// resize by the exact amount needed making sure there's no wasted space at the end
		destBuffer->Extend(verticesToAdd - destBuffer->GetRemainingSpace());
		ASSERT(destBuffer->GetRemainingSpace() >= verticesToAdd);
	}

	// adjust position by the tilemesh offset. TileMesh's are modeled using the
	// origin (0,0,0) as the center and are 1 unit wide/deep/tall. So, their
	// max extents are from -0.5,-0.5,-0.5 to 0.5,0.5,0.5. For rendering
	// purposes in a chunk, we want the extents to be 0,0,0 to 1,1,1. This
	// adjustment fixes that
	Vector3 positionOffset = TILEMESH_OFFSET;
	positionOffset.x += (float)position.x;
	positionOffset.y += (float)position.y;
	positionOffset.z += (float)position.z;

	// copy vertices
	sourceBuffer->MoveTo(firstVertex);
	for (uint i = 0; i < numVertices; ++i)
	{
		CopyVertex(chunk, sourceBuffer, destBuffer, positionOffset, transform, color);

		sourceBuffer->MoveNext();
		destBuffer->MoveNext();
	}

	return verticesToAdd;
}

void ChunkVertexGenerator::CopyVertex(const TileChunk *chunk, VertexBuffer *sourceBuffer, VertexBuffer *destBuffer, const Vector3 &positionOffset, const Matrix4x4 *transform, const Color &color)
{
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

	// copy to destination
	destBuffer->SetCurrentPosition3(v);
	destBuffer->SetCurrentNormal(n);

	// just directly copy the tex coord as-is
	destBuffer->SetCurrentTexCoord(sourceBuffer->GetCurrentTexCoord());

	// color is the same for the entire mesh
	destBuffer->SetCurrentColor(color);
}

