#ifndef __FRAMEWORK_ASSETS_ANIMATION_KEYFRAMEMESHINSTANCE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_KEYFRAMEMESHINSTANCE_H_INCLUDED__

#include "../../common.h"
#include <stl/string.h>

class KeyframeMesh;
class RenderState;
class Texture;

/**
 * An instance of a keyframe mesh that can be manipulated and rendered from
 * a single shared mesh object.
 */
class KeyframeMeshInstance
{
public:
	/**
	 * Creates a new keyframe mesh instance.
	 * @param mesh the mesh this instance is of
	 */
	KeyframeMeshInstance(KeyframeMesh *mesh);

	virtual ~KeyframeMeshInstance();

	/**
	 * Update (tick) callback.
	 * @param delta time since last update
	 */
	void OnUpdate(float delta);

	/**
	 * Sets the current animation sequence.
	 * @param startFrame the first frame of the sequence
	 * @param endFrame the last frame of the sequence
	 * @param loop TRUE to loop the sequence, FALSE to stop when done
	 *             and leave the current frame as the end frame
	 */
	void SetSequence(uint32_t startFrame, uint32_t endFrame, BOOL loop);

	/**
	 * Sets the current animation sequence.
	 * @param name the name of the sequence from the mesh's list of sequences
	 * @param loop TRUE to loop the sequence, FALSE to stop when done
	 *             and leave the current frame as the end frame
	 */
	void SetSequence(const stl::string &name, BOOL loop);

	/**
	 * Temporarily override the current animation sequence to run the specified
	 * sequence just once. When it's finished, the original sequence will be
	 * restored and continued as before.
	 * @param name the name of the sequence from the mesh's list of sequences
	 */
	void RunSequenceOnce(const stl::string &name);

	/**
	 * @return the current frame
	 */
	uint32_t GetCurrentFrame() const                       { return m_thisFrame; }

	/**
	 * @return the next frame
	 */
	uint32_t GetNextFrame() const                          { return m_nextFrame; }

	/**
	 * @return the current interpolation amount between the current frame
	 *         and the next frame
	 */
	float GetInterpolation() const                         { return m_interpolation; }

	/**
	 * @return the current texture
	 */
	Texture* GetTexture() const                            { return m_texture; }

	/**
	 * @param texture the new texture, or NULL to set no texture
	 */
	void SetTexture(Texture *texture)                      { m_texture = texture; }

	/**
	 * @return the render state used to render this instance
	 */
	RenderState* GetRenderState() const                    { return m_renderState; }

	/**
	 * @return the mesh this instance is of
	 */
	KeyframeMesh* GetMesh() const                          { return m_mesh; }

private:
	void RecoverFromTempSequence();

	KeyframeMesh *m_mesh;
	Texture *m_texture;
	RenderState *m_renderState;
	stl::string m_currentSequenceName;
	uint32_t m_currentSequenceStart;
	uint32_t m_currentSequenceEnd;
	BOOL m_currentSequenceLoop;
	uint32_t m_thisFrame;
	uint32_t m_nextFrame;
	float m_interpolation;
	BOOL m_isRunningTempSequence;
	stl::string m_oldSequenceName;
	BOOL m_oldSequenceLoop;
};

#endif

