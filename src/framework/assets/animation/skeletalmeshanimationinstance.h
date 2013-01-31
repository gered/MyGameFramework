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
	void SetSequence(uint32_t startFrame, uint32_t endFrame, BOOL loop);
	void SetSequence(const stl::string &name, BOOL loop);
	void RunSequenceOnce(const stl::string &name);
	
	uint32_t GetCurrentFrame() const                       { return m_thisFrame; }
	uint32_t GetNextFrame() const                          { return m_nextFrame; }
	float GetInterpolation() const                         { return m_interpolation; }
	
private:
	void RecoverFromTempSequence();
	
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
