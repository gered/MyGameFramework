#include "../debug.h"

#include "grid.h"

#include "../graphics/graphicsdevice.h"
#include "../graphics/renderstate.h"
#include "../graphics/vertexbuffer.h"

Grid::Grid(GraphicsDevice *graphicsDevice, uint width, uint height)
{
	m_graphicsDevice = graphicsDevice;

	m_renderState = new RENDERSTATE_DEFAULT;
	ASSERT(m_renderState != NULL);
	
	VERTEX_ATTRIBS attribs[] = {
		VERTEX_POS_3D,
		VERTEX_COLOR,
	};

	m_horizontalPoints = new VertexBuffer();
	ASSERT(m_horizontalPoints != NULL);
	m_horizontalPoints->Initialize(attribs, 2, width * 2 + 2, BUFFEROBJECT_USAGE_STATIC);

	m_verticalPoints = new VertexBuffer();
	ASSERT(m_verticalPoints != NULL);
	m_verticalPoints->Initialize(attribs, 2, height * 2 + 2, BUFFEROBJECT_USAGE_STATIC);

	for (uint i = 0; i < height + 1; ++i)
	{
		m_horizontalPoints->SetPosition3((i * 2), -(width / 2.0f), 0.0f, i - (height / 2.0f));
		m_horizontalPoints->SetColor((i * 2), 1.0f, 1.0f, 1.0f);
		m_horizontalPoints->SetPosition3((i * 2) + 1, width / 2.0f, 0.0f, i - (height / 2.0f));
		m_horizontalPoints->SetColor((i * 2) + 1, 1.0f, 1.0f, 1.0f);
	}
		
	for (uint i = 0; i < width + 1; ++i)
	{
		m_verticalPoints->SetPosition3((i * 2), i - (width / 2.0f), 0.0f, -(height / 2.0f));
		m_verticalPoints->SetColor((i * 2), 1.0f, 1.0f, 1.0f);
		m_verticalPoints->SetPosition3((i * 2) + 1, i - (width / 2.0f), 0.0f, height / 2.0f);
		m_verticalPoints->SetColor((i * 2) + 1, 1.0f, 1.0f, 1.0f);
	}
}

Grid::~Grid()
{
	SAFE_DELETE(m_horizontalPoints);
	SAFE_DELETE(m_verticalPoints);
}

void Grid::OnRender()
{
	m_renderState->Apply();

	m_graphicsDevice->BindVertexBuffer(m_horizontalPoints);
	m_graphicsDevice->RenderLines(0, m_horizontalPoints->GetNumElements() / 2);
	m_graphicsDevice->UnbindVertexBuffer();

	m_graphicsDevice->BindVertexBuffer(m_verticalPoints);
	m_graphicsDevice->RenderLines(0, m_verticalPoints->GetNumElements() / 2);
	m_graphicsDevice->UnbindVertexBuffer();
}
