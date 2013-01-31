#ifndef __FRAMEWORK_GRAPHICS_BLENDSTATE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_BLENDSTATE_H_INCLUDED__

#include "../common.h"

enum BLEND_FACTOR
{
	ZERO,
	ONE,
	SRC_COLOR,
	INVERSE_SRC_COLOR,
	DST_COLOR,
	INVERSE_DST_COLOR,
	SRC_ALPHA,
	INVERSE_SRC_ALPHA,
	DST_ALPHA,
	INVERSE_DST_ALPHA,
	SRC_ALPHA_SATURATION
};

/**
 * Wraps blending settings for OpenGL. Instances of this class can be initialized
 * and then used later on during execution to quickly apply a set of blend states.
 */
class BlendState
{
public:
	/**
	 * Creates a new blend state object with default settings matching OpenGL's
	 * initial settings.
	 */
	BlendState();

	/**
	 * Creates a new blend state object with default settings matching OpenGL's
	 * initial settings except for those specified.
	 * @param sourceFactor source blend factor
	 * @param destinationFactor destination blend factor
	 */
	BlendState(BLEND_FACTOR sourceFactor, BLEND_FACTOR destinationFactor);

	virtual ~BlendState();

	/**
	 * Applies blend states to the current OpenGL context.
	 */
	void Apply() const;

	/**
	 * @return TRUE if blending is enabled
	 */
	BOOL GetBlending() const                               { return m_blending; }

	/**
	 * @return the source blending factor
	 */
	BLEND_FACTOR GetSourceBlendFactor() const              { return m_sourceBlendFactor; }

	/**
	 * @return the destination blending factor
	 */
	BLEND_FACTOR GetDestBlendFactor() const                { return m_destBlendFactor; }

	/**
	 * Toggles blending on/off.
	 */
	void SetBlending(BOOL enable)                          { m_blending = enable; }

	/**
	 * Sets the source blending factor.
	 */
	void SetSourceBlendFactor(BLEND_FACTOR factor)         { m_sourceBlendFactor = factor; }

	/**
	 * Sets the destination blending factor.
	 */
	void SetDestBlendFactor(BLEND_FACTOR factor)           { m_destBlendFactor = factor; }

private:
	void Initialize();
	int FindBlendFactorValue(BLEND_FACTOR factor) const;

	BOOL m_blending;
	BLEND_FACTOR m_sourceBlendFactor;
	BLEND_FACTOR m_destBlendFactor;
};

#define BLENDSTATE_DEFAULT BlendState()
#define BLENDSTATE_OPAQUE BlendState()
#define BLENDSTATE_ALPHABLEND BlendState(SRC_ALPHA, INVERSE_SRC_ALPHA)

#endif
