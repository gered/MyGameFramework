#include "../../debug.h"

#include "staticmeshinstance.h"

#include "staticmesh.h"
#include "staticmeshsubset.h"
#include "../../graphics/renderstate.h"
#include "../../graphics/texture.h"

StaticMeshInstance::StaticMeshInstance(StaticMesh *mesh)
{
	m_mesh = mesh;

	m_renderState = new RENDERSTATE_DEFAULT;
	ASSERT(m_renderState != NULL);

	// prepare list of local mesh instance texture overrides
	// initialize it as an exact copy of the source mesh's textures
	m_textures = NULL;
	if (m_mesh->GetNumSubsets() > 0)
	{
		m_textures = new Texture*[m_mesh->GetNumSubsets()];
		ASSERT(m_textures != NULL);

		for (uint i = 0; i < m_mesh->GetNumSubsets(); ++i)
			ResetTexture(i);
	}
}

StaticMeshInstance::~StaticMeshInstance()
{
	SAFE_DELETE(m_renderState);
}

void StaticMeshInstance::SetTexture(uint index, Texture *texture)
{

	m_textures[index] = texture;
}

void StaticMeshInstance::ResetTexture(uint index)
{
	m_textures[index] = m_mesh->GetSubset(index)->GetTexture();
}

