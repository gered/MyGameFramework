#ifndef __FRAMEWORK_MATH_MATRIXSTACK_H_INCLUDED__
#define __FRAMEWORK_MATH_MATRIXSTACK_H_INCLUDED__

#include "matrix4x4.h"
#include <stl/list.h>

/**
 * Manages a LIFO stack of matrices.
 */
class MatrixStack
{
public:
	/**
	 * Creates an empty matrix stack.
	 */
	MatrixStack();

	/**
	 * Pushes the stack down by one, duplicating the current matrix.
	 */
	void Push();

	/**
	 * Pops the stack, replacing the top matrix with the one under it.
	 */
	void Pop();

	/**
	 * Removes all items on the stack and resets the top matrix.
	 */
	void Clear();
	
	Matrix4x4 top;

private:
	stl::list<Matrix4x4> m_stack;
};

inline MatrixStack::MatrixStack()
{
	top = Matrix4x4();
	m_stack.resize(16, Matrix4x4());
}

inline void MatrixStack::Push()
{
	m_stack.push_back(top);
}

inline void MatrixStack::Pop()
{
	top = m_stack.back();
	m_stack.pop_back();
}

inline void MatrixStack::Clear()
{
	top = Matrix4x4();
	m_stack.clear();
}

#endif
