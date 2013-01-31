#include "../debug.h"

#include "grid.h"

#include "../graphics/graphicsdevice.h"
#include "../graphics/renderstate.h"
#include "../graphics/vertexbuffer.h"

Grid::Grid(GraphicsDevice *graphicsDevice, uint16_t width, uint16_t height)
{
	STACK_TRACE;
	m_graphicsDevice = graphicsDevice;

	m_renderState = new RENDERSTATE_DEFAULT;
	ASSERT(m_renderState != NULL);

	m_horizontalPoints = new VertexBuffer(BUFFEROBJECT_USAGE_STATIC);
	ASSERT(m_horizontalPoints != NULL);
	m_horizontalPoints->AddAttribute(VERTEX_POS_3D);
	m_horizontalPoints->AddAttribute(VERTEX_COLOR);
	m_horizontalPoints->AddAttribute(ATTRIB_SIZE_1F);
	m_horizontalPoints->AddAttribute(ATTRIB_SIZE_1F);
	m_horizontalPoints->Create(width * 2 + 2);

	m_verticalPoints = new VertexBuffer(BUFFEROBJECT_USAGE_STATIC);
	ASSERT(m_verticalPoints != NULL);
	m_verticalPoints->AddAttribute(VERTEX_POS_3D);
	m_verticalPoints->AddAttribute(VERTEX_COLOR);
	m_verticalPoints->AddAttribute(ATTRIB_SIZE_1F);
	m_verticalPoints->AddAttribute(ATTRIB_SIZE_1F);
	m_verticalPoints->Create(height * 2 + 2);

	for (uint16_t i = 0; i < height + 1; ++i)
	{
		m_horizontalPoints->SetPosition3((i * 2), -(width / 2.0f), 0.0f, i - (height / 2.0f));
		m_horizontalPoints->SetColor((i * 2), 1.0f, 1.0f, 1.0f);
		m_horizontalPoints->SetPosition3((i * 2) + 1, width / 2.0f, 0.0f, i - (height / 2.0f));
		m_horizontalPoints->SetColor((i * 2) + 1, 1.0f, 1.0f, 1.0f);

		if (i == 8)
		{
			m_horizontalPoints->Set1f(3, (i * 2), 1.0f);
			m_horizontalPoints->Set1f(3, (i * 2) + 1, 1.0f);
		}
		else
		{
			m_horizontalPoints->Set1f(3, (i * 2), 0.0f);
			m_horizontalPoints->Set1f(3, (i * 2) + 1, 0.0f);
		}
		m_horizontalPoints->Set1f(2, (i * 2), 2.0f);
		m_horizontalPoints->Set1f(2, (i * 2) + 1, 3.0f);
	}
		
	for (uint16_t i = 0; i < width + 1; ++i)
	{
		m_verticalPoints->SetPosition3((i * 2), i - (width / 2.0f), 0.0f, -(height / 2.0f));
		m_verticalPoints->SetColor((i * 2), 1.0f, 1.0f, 1.0f);
		m_verticalPoints->SetPosition3((i * 2) + 1, i - (width / 2.0f), 0.0f, height / 2.0f);
		m_verticalPoints->SetColor((i * 2) + 1, 1.0f, 1.0f, 1.0f);
		
		if (i == 1)
		{
			m_verticalPoints->Set1f(3, (i * 2), 1.0f);
			m_verticalPoints->Set1f(3, (i * 2) + 1, 1.0f);
		}
		else
		{
			m_verticalPoints->Set1f(3, (i * 2), 0.0f);
			m_verticalPoints->Set1f(3, (i * 2) + 1, 0.0f);
		}
		m_verticalPoints->Set1f(2, (i * 2), 0.0f);
		m_verticalPoints->Set1f(2, (i * 2) + 1, 0.0f);
	}
}

Grid::~Grid()
{
	STACK_TRACE;
	SAFE_DELETE(m_horizontalPoints);
	SAFE_DELETE(m_verticalPoints);
}

void Grid::OnRender()
{
	STACK_TRACE;
	m_renderState->Apply();

	m_graphicsDevice->BindVertexBuffer(m_horizontalPoints);
	m_graphicsDevice->RenderLines(0, m_horizontalPoints->GetNumElements() / 2);
	m_graphicsDevice->UnbindVertexBuffer();

	m_graphicsDevice->BindVertexBuffer(m_verticalPoints);
	m_graphicsDevice->RenderLines(0, m_verticalPoints->GetNumElements() / 2);
	m_graphicsDevice->UnbindVertexBuffer();
}
