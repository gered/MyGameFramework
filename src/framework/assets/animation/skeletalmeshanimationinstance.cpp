#include "../../debug.h"

#include "../../common.h"
#include "skeletalmeshanimationinstance.h"
#include "skeletalmeshinstance.h"
#include "skeletalmesh.h"
#include "../../support/animationsequence.h"
#include <stl/string.h>

SkeletalMeshAnimationInstance::SkeletalMeshAnimationInstance(SkeletalMesh *mesh)
	: SkeletalMeshInstance(mesh)
{
	STACK_TRACE;
	m_currentSequenceStart = 0;
	m_currentSequenceEnd = 0;
	m_currentSequenceLoop = FALSE;
	m_thisFrame = 0;
	m_nextFrame = 0;
	m_interpolation = 0.0f;
	m_isRunningTempSequence = FALSE;
	m_oldSequenceLoop = FALSE;
}

SkeletalMeshAnimationInstance::~SkeletalMeshAnimationInstance()
{
	STACK_TRACE;
}

void SkeletalMeshAnimationInstance::OnUpdate(float delta)
{
	STACK_TRACE;
	SkeletalMeshInstance::OnUpdate(delta);
	if (m_currentSequenceStart != m_currentSequenceEnd)
	{
		m_interpolation += delta * 30;
		if (m_interpolation >= 1.0f)
		{
			m_interpolation = 0.0f;
			
			++m_thisFrame;
			if (m_thisFrame > m_currentSequenceEnd)
			{
				m_thisFrame = m_currentSequenceStart;
				
				if (m_isRunningTempSequence)
				{
					RecoverFromTempSequence();
					return;
				}
				if (!m_currentSequenceLoop)
				{
					m_currentSequenceStart = m_currentSequenceEnd;
					m_thisFrame = m_currentSequenceEnd;
					m_nextFrame = m_thisFrame;
					return;
				}
			}
			
			++m_nextFrame;
			if (m_nextFrame > m_currentSequenceEnd)
			{
				if (!m_currentSequenceLoop)
					m_nextFrame = m_currentSequenceEnd;
				else
					m_nextFrame = m_currentSequenceStart;
			}
		}
	}
}

void SkeletalMeshAnimationInstance::SetSequence(uint32_t startFrame, uint32_t endFrame, BOOL loop)
{
	STACK_TRACE;
	m_currentSequenceName.clear();
	m_currentSequenceStart = startFrame;
	m_currentSequenceEnd = endFrame;
	m_currentSequenceLoop = loop;
	
	m_thisFrame = m_currentSequenceStart;
	m_nextFrame = m_thisFrame + 1;
	if (m_nextFrame > m_currentSequenceEnd)
		m_nextFrame = m_currentSequenceEnd;
	
	m_interpolation = 0.0f;
}

void SkeletalMeshAnimationInstance::SetSequence(const stl::string &name, BOOL loop)
{
	STACK_TRACE;
	if (m_currentSequenceName == name)
		return;
	
	const AnimationSequence *sequence = GetMesh()->GetAnimation(name);
	ASSERT(sequence != NULL);
	SetSequence(sequence->start, sequence->stop, loop);
	m_currentSequenceName = name;}

void SkeletalMeshAnimationInstance::RunSequenceOnce(const stl::string &name)
{
	STACK_TRACE;
	if (m_isRunningTempSequence)
		return;
	
	m_oldSequenceName = m_currentSequenceName;
	m_oldSequenceLoop = m_currentSequenceLoop;
	
	m_isRunningTempSequence = TRUE;
	
	SetSequence(name, FALSE);
}

void SkeletalMeshAnimationInstance::RecoverFromTempSequence()
{
	STACK_TRACE;
	m_isRunningTempSequence = FALSE;
	SetSequence(m_oldSequenceName, m_oldSequenceLoop);
	m_oldSequenceName.clear();
	m_oldSequenceLoop = false;
}
