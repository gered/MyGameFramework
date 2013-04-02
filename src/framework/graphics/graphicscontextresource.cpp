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

bool GraphicsContextResource::Initialize()
{
	ASSERT(m_graphicsDevice == NULL);
	if (m_graphicsDevice != NULL)
		return false;
	
	return true;
}

bool GraphicsContextResource::Initialize(GraphicsDevice *graphicsDevice)
{
	ASSERT(m_graphicsDevice == NULL);
	if (m_graphicsDevice != NULL)
		return false;
	
	ASSERT(graphicsDevice != NULL);
	if (graphicsDevice == NULL)
		return false;
	
	m_graphicsDevice = graphicsDevice;
	m_graphicsDevice->RegisterManagedResource(this);
	
	return true;
}
