#ifndef __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHSUBSET_INCLUDED_H__
#define __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHSUBSET_INCLUDED_H__

#include "../../common.h"
#include <stl/string.h>

class IndexBuffer;

class SkeletalMeshSubset
{
public:
	SkeletalMeshSubset();
	virtual ~SkeletalMeshSubset();
	
	void Create(const stl::string &name, uint numTriangles, BOOL alpha);

	const stl::string& GetName() const                     { return m_name; }
	IndexBuffer* GetIndices() const                        { return m_indices; }
	BOOL IsAlphaBlended() const                            { return m_alpha; }

private:
	stl::string m_name;
	IndexBuffer *m_indices;
	BOOL m_alpha;
};

#endif
