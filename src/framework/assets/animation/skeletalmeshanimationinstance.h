#ifndef __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHANIMATIONINSTANCE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_ANIMATION_SKELETALMESHANIMATIONINSTANCE_H_INCLUDED__

#include "../../common.h"
#include "skeletalmeshinstance.h"
#include <stl/string.h>

class SkeletalMesh;

class SkeletalMeshAnimationInstance : public SkeletalMeshInstance
{
public:
	SkeletalMeshAnimationInstance(SkeletalMesh *mesh);
	virtual ~SkeletalMeshAnimationInstance();
	
	void OnUpdate(float delta);
	void SetSequence(uint startFrame, uint endFrame, bool loop);
	void SetSequence(const stl::string &name, bool loop);
	void RunSequenceOnce(const stl::string &name);
	
	uint GetCurrentFrame() const                           { return m_thisFrame; }
	uint GetNextFrame() const                              { return m_nextFrame; }
	float GetInterpolation() const                         { return m_interpolation; }
	
private:
	void RecoverFromTempSequence();
	
	stl::string m_currentSequenceName;
	uint m_currentSequenceStart;
	uint m_currentSequenceEnd;
	bool m_currentSequenceLoop;
	uint m_thisFrame;
	uint m_nextFrame;
	float m_interpolation;
	bool m_isRunningTempSequence;
	stl::string m_oldSequenceName;
	bool m_oldSequenceLoop;
};

#endif
