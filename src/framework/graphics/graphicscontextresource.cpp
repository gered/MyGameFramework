#include "../debug.h"

#include "graphicscontextresource.h"
#include "graphicsdevice.h"

GraphicsContextResource::GraphicsContextResource()
{
	STACK_TRACE;
	m_graphicsDevice = NULL;
}

GraphicsContextResource::~GraphicsContextResource()
{
	STACK_TRACE;
	if (m_graphicsDevice != NULL)
		m_graphicsDevice->UnregisterManagedResource(this);
}

BOOL GraphicsContextResource::Initialize()
{
	STACK_TRACE;
	return TRUE;
}

BOOL GraphicsContextResource::Initialize(GraphicsDevice *graphicsDevice)
{
	STACK_TRACE;
	ASSERT(graphicsDevice != NULL);
	
	m_graphicsDevice = graphicsDevice;
	m_graphicsDevice->RegisterManagedResource(this);
	
	return TRUE;
}
