#ifndef __FRAMEWORK_SUPPORT_GRID_H_INCLUDED__
#define __FRAMEWORK_SUPPORT_GRID_H_INCLUDED__

#include "../common.h"

class GraphicsDevice;
class RenderState;
class VertexBuffer;

class Grid
{
public:
	Grid(GraphicsDevice *graphicsDevice, uint16_t width, uint16_t height);
	virtual ~Grid();

	void OnRender();

private:
	GraphicsDevice *m_graphicsDevice;
	VertexBuffer *m_horizontalPoints;
	VertexBuffer *m_verticalPoints;
	RenderState *m_renderState;
};

#endif
