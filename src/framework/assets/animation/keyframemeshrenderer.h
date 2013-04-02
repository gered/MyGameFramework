#ifndef __FRAMEWORK_ASSETS_ANIMATION_KEYFRAMEMESHRENDERER_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_KEYFRAMEMESHRENDERER_H_INCLUDED__

#include "../../common.h"

class GraphicsDevice;
class KeyframeMesh;
class KeyframeMeshInstance;
class Texture;
class VertexLerpShader;

/**
 * Helper object that renders instances of keyframe mesh objects and
 * standalone keyframe mesh objects.
 */
class KeyframeMeshRenderer
{
public:
	/**
	 * Creates a keyframe mesh renderer.
	 */
	KeyframeMeshRenderer();

	virtual ~KeyframeMeshRenderer();

	/**
	 * Renders an instance of a keyframe mesh.
	 * @param graphicsDevice the graphics device to render with
	 * @param instance the keyframe mesh instance to render
	 */
	void Render(GraphicsDevice *graphicsDevice, KeyframeMeshInstance *instance, VertexLerpShader *shader);

	/**
	 * Renders an instance of a keyframe mesh.
	 * @param graphicsDevice the graphics device to render with
	 * @param instance the keyframe mesh instance to render
	 * @param frame render this frame instead of the instance's current frame
	 */
	void Render(GraphicsDevice *graphicsDevice, KeyframeMeshInstance *instance, uint frame, VertexLerpShader *shader);

	/**
	 * Renders an instance of a keyframe mesh.
	 * @param graphicsDevice the graphics device to render with
	 * @param instance the keyframe mesh instance to render
	 * @param startFrame the start frame to use for rendering instead of
	 *                   the instance's current "current frame"
	 * @param endFrame the end frame to use for rendering instead of the
	 *                   instance's current "next frame"
	 * @param interpolation the amount to interpolate between the start frame
	 *                      and end frame
	 */
	void Render(GraphicsDevice *graphicsDevice, KeyframeMeshInstance *instance, uint startFrame, uint endFrame, float interpolation, VertexLerpShader *shader);

	/**
	 * Renders a keyframe mesh using the first frame.
	 * @param graphicsDevice the graphics device to render with
	 * @param mesh the keyframe mesh to render
	 * @param texture the texture to render the mesh with, or NULL for no texture
	 */
	void Render(GraphicsDevice *graphicsDevice, KeyframeMesh *mesh, const Texture *texture, VertexLerpShader *shader);

	/**
	 * Renders a keyframe mesh.
	 * @param graphicsDevice the graphics device to render with
	 * @param mesh the keyframe mesh to render
	 * @param texture the texture to render the mesh with, or NULL for no texture
	 * @param frame the keyframe to render
	 */
	void Render(GraphicsDevice *graphicsDevice, KeyframeMesh *mesh, const Texture *texture, uint frame, VertexLerpShader *shader);

	/**
	 * Renders a keyframe mesh.
	 * @param graphicsDevice the graphics device to render with
	 * @param mesh the keyframe mesh to render
	 * @param texture the texture to render the mesh with, or NULL for no texture
	 * @param startFrame the start frame to use for rendering
	 * @param endFrame the end frame to use for rendering
	 * @param interpolation the amount to interpolate by between the start 
	 *                      and end frame
	 */
	void Render(GraphicsDevice *graphicsDevice, KeyframeMesh *mesh, const Texture *texture, uint startFrame, uint endFrame, float interpolation, VertexLerpShader *shader);

private:
	void SetFrameVertices(KeyframeMesh *mesh, uint frame);
	void SetFrameVertices(KeyframeMesh *mesh, uint startFrame, uint endFrame);
};

#endif
