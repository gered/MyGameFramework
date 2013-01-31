#ifndef __FRAMEWORK_GRAPHICS_RENDERSTATE_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_RENDERSTATE_H_INCLUDED__

#include "../common.h"

enum CULL_MODE
{
	BACK,
	FRONT,
	FRONT_AND_BACK
};

enum DEPTH_FUNCTION
{
	DEPTH_NEVER,
	DEPTH_LESS,
	DEPTH_EQUAL,
	DEPTH_LESS_OR_EQUAL,
	DEPTH_GREATER,
	DEPTH_NOT_EQUAL,
	DEPTH_GREATER_OR_EQUAL,
	DEPTH_ALWAYS
};

/**
 * Wraps render settings for OpenGL. Instances of this class can be initialized
 * and then used later on during execution to quickly apply a set of render states.
 */
class RenderState
{
public:
	/**
	 * Creates a new render state object with default settings matching OpenGL's
	 * initial settings.
	 */
	RenderState();

	virtual ~RenderState();

	/**
	 * Applies render states to the current OpenGL context.
	 */
	void Apply() const;

	/**
	 * @return TRUE if depth testing is enabled
	 */
	BOOL GetDepthTesting() const                           { return m_depthTesting; }

	/**
	 * @return the depth testing function
	 */
	DEPTH_FUNCTION GetDepthFunction() const                { return m_depthFunction; }

	/**
	 * @return TRUE if polygons are being culled based on their vertices winding
	 */
	BOOL GetFaceCulling() const                            { return m_faceCulling; }

	/**
	 * @return the polygon cull mode
	 */
	CULL_MODE GetFaceCullingMode() const                   { return m_faceCullingMode; }

	/**
	 * @return the width of rasterized lines
	 */
	float GetLineWidth() const                             { return m_lineWidth; }

	/**
	 * Toggles depth testing on/off.
	 */
	void SetDepthTesting(BOOL enable)                      { m_depthTesting = enable; }

	/**
	 * Sets the depth testing function.
	 */
	void SetDepthFunction(DEPTH_FUNCTION function)         { m_depthFunction = function; }

	/**
	 * Toggles polygon culling based on their vertices winding on/off.
	 */
	void SetFaceCulling(BOOL enable)                       { m_faceCulling = enable; }

	/**
	 * Sets the polygon culling mode.
	 */
	void SetFaceCullingMode(CULL_MODE mode)                { m_faceCullingMode = mode; }

	/**
	 * Sets the width of rasterized lines.
	 */
	void SetLineWidth(float width)                         { m_lineWidth = width; }

private:
	void Initialize();
	void SetFaceCulling() const;
	int FindDepthFunctionValue(DEPTH_FUNCTION function) const;

	BOOL m_depthTesting;
	DEPTH_FUNCTION m_depthFunction;
	BOOL m_faceCulling;
	CULL_MODE m_faceCullingMode;
	float m_lineWidth;
};

#define RENDERSTATE_DEFAULT RenderState()

#endif
