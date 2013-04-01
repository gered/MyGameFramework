#include "../debug.h"

#include "graphicscontextresource.h"
#include "graphicsdevice.h"

GraphicsContextResource::GraphicsContextResource()
{
	STACK_TRACE;
	m_graphicsDevice = NULL;
}

GraphicsContextResource::GraphicsContextResource(GraphicsDevice *graphicsDevice)
{
	STACK_TRACE;
	ASSERT(graphicsDevice != NULL);

	m_graphicsDevice = graphicsDevice;
	m_graphicsDevice->RegisterManagedResource(this);
}

GraphicsContextResource::~GraphicsContextResource()
{
	STACK_TRACE;
	if (m_graphicsDevice != NULL)
		m_graphicsDevice->UnregisterManagedResource(this);
}
