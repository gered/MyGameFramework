#include "../debug.h"

#include "graphicscontextresource.h"
#include "graphicsdevice.h"

GraphicsContextResource::GraphicsContextResource()
{
	STACK_TRACE;
	m_graphicsDevice = NULL;
}

void GraphicsContextResource::Release()
{
	STACK_TRACE;
	if (m_graphicsDevice != NULL)
		m_graphicsDevice->UnregisterManagedResource(this);
	
	m_graphicsDevice = NULL;
}

BOOL GraphicsContextResource::Initialize()
{
	STACK_TRACE;
	ASSERT(m_graphicsDevice == NULL);
	if (m_graphicsDevice != NULL)
		return FALSE;
	
	return TRUE;
}

BOOL GraphicsContextResource::Initialize(GraphicsDevice *graphicsDevice)
{
	STACK_TRACE;
	ASSERT(m_graphicsDevice == NULL);
	if (m_graphicsDevice != NULL)
		return FALSE;
	
	ASSERT(graphicsDevice != NULL);
	if (graphicsDevice == NULL)
		return FALSE;
	
	m_graphicsDevice = graphicsDevice;
	m_graphicsDevice->RegisterManagedResource(this);
	
	return TRUE;
}
