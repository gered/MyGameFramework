#ifndef __CONTEXTS_RENDERCONTEXT_H_INCLUDED__
#define __CONTEXTS_RENDERCONTEXT_H_INCLUDED__

#include "../framework/common.h"

#include "../framework/graphics/color.h"
#include "../framework/math/vector3.h"

class BillboardSpriteBatch;
class ContentManager;
class GraphicsDevice;
class KeyframeMeshRenderer;
class SkeletalMeshRenderer;
class SpriteBatch;
class StaticMeshRenderer;

class RenderContext
{
public:
	RenderContext(GraphicsDevice *graphicsDevice, ContentManager *contentManager);
	virtual ~RenderContext();

	void OnLoadGame();
	void OnPreRender();
	void OnPostRender();
	void OnResize();

	GraphicsDevice* GetGraphicsDevice() const              { return m_graphicsDevice; }
	SpriteBatch* GetSpriteBatch() const                    { return m_spriteBatch; }
	BillboardSpriteBatch* GetBillboardSpriteBatch() const  { return m_billboardSpriteBatch; }
	KeyframeMeshRenderer* GetKeyframeMeshRenderer() const  { return m_keyframeMeshRenderer; }
	SkeletalMeshRenderer* GetSkeletalMeshRenderer() const  { return m_skeletalMeshRenderer; }
	StaticMeshRenderer* GetStaticMeshRenderer() const      { return m_staticMeshRenderer; }

	uint GetScreenScale() const                            { return m_screenScale; }

private:
	void CalculateScreenScale();

	ContentManager *m_contentManager;
	GraphicsDevice *m_graphicsDevice;
	SpriteBatch *m_spriteBatch;
	BillboardSpriteBatch *m_billboardSpriteBatch;
	KeyframeMeshRenderer *m_keyframeMeshRenderer;
	SkeletalMeshRenderer *m_skeletalMeshRenderer;
	StaticMeshRenderer *m_staticMeshRenderer;

	uint m_screenScale;
};

#endif

