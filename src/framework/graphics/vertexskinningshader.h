#ifndef __FRAMEWORK_GRAPHICS_VERTEXSKINNINGSHADER_H_INCLUDED__
#define __FRAMEWORK_GRAPHICS_VERTEXSKINNINGSHADER_H_INCLUDED__

#include "../common.h"
#include "standardshader.h"
#include <stl/string.h>

struct Quaternion;
struct Vector3;

class VertexSkinningShader : public StandardShader
{
public:
	virtual ~VertexSkinningShader();
	
	void SetJointPositions(const Vector3 *positions, uint count);
	void SetJointRotations(const Quaternion *rotations, uint count);
	
protected:
	VertexSkinningShader();
	
	const stl::string& GetJointPositionsUniform() const    { return m_positionsUniform; }
	const stl::string& GetJointRotationsUniform() const    { return m_rotationsUniform; }
	
	void SetJointPositionsUniform(const stl::string &name) { m_positionsUniform = name; }
	void SetJointRotationsUniform(const stl::string &name) { m_rotationsUniform = name; }
	
private:
	stl::string m_positionsUniform;
	stl::string m_rotationsUniform;
};

#endif
