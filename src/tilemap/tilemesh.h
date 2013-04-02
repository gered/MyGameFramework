#ifndef __TILEMAP_TILEMESH_H_INCLUDED__
#define __TILEMAP_TILEMESH_H_INCLUDED__

#include "../framework/common.h"
#include "tilelightdefs.h"
#include "tilemeshdefs.h"
#include "../framework/math/vector3.h"
#include "../framework/graphics/color.h"

class VertexBuffer;

const Vector3 TILEMESH_OFFSET = Vector3(0.5f, 0.5f, 0.5f);

enum TILEMESH_TYPE
{
	TILEMESH_STATIC,
	TILEMESH_CUBE
};

class TileMesh
{
public:
	TileMesh()                                             {}
	virtual ~TileMesh()                                    {}

	virtual VertexBuffer* GetBuffer() const = 0;
	virtual uint GetNumCollisionVertices() const = 0;
	virtual const Vector3* GetCollisionVertices() const = 0;

	virtual TILEMESH_TYPE GetType() const = 0;

	bool IsCompletelyOpaque() const                        { return m_opaqueSides == SIDE_ALL; }
	bool IsOpaque(MESH_SIDES sides) const                  { return IsBitSet(sides, m_opaqueSides); }
	bool IsAlpha() const                                   { return m_alpha; }
	const Color& GetColor() const                          { return m_color; }
	float GetTranslucency() const                          { return m_translucency; }
	bool IsLightSource() const                             { return m_lightValue > 0; }
	TILE_LIGHT_VALUE GetLightValue() const                 { return m_lightValue; }

protected:
	void SetOpaque(MESH_SIDES sides)                       { m_opaqueSides = sides; }
	void SetAlpha(bool alpha)                              { m_alpha = alpha; }
	void SetColor(const Color &color)                      { m_color = color; }
	void SetTranslucency(float translucency)               { m_translucency = translucency; }
	void SetLight(TILE_LIGHT_VALUE lightValue)             { m_lightValue = lightValue; }

private:
	MESH_SIDES m_opaqueSides;
	bool m_alpha;
	Color m_color;
	float m_translucency;
	TILE_LIGHT_VALUE m_lightValue;
};

#endif

