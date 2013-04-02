#include "../debug.h"

#include "graphicscontextresource.h"
#include "graphicsdevice.h"

GraphicsContextResource::GraphicsContextResource()
{
	m_graphicsDevice = NULL;
}

void GraphicsContextResource::Release()
{
	if (m_graphicsDevice != NULL)
		m_graphicsDevice->UnregisterManagedResource(this);
	
	m_graphicsDevice = NULL;
}

BOOL GraphicsContextResource::Initialize()
{
	ASSERT(m_graphicsDevice == NULL);
	if (m_graphicsDevice != NULL)
		return FALSE;
	
	return TRUE;
}

BOOL GraphicsContextResource::Initialize(GraphicsDevice *graphicsDevice)
{
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
