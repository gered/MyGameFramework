#include "../../debug.h"

#include "keyframemeshinstance.h"

#include "keyframemesh.h"
#include "../../graphics/renderstate.h"
#include "../../graphics/texture.h"
#include "../../support/animationsequence.h"
#include <stl/string.h>

KeyframeMeshInstance::KeyframeMeshInstance(KeyframeMesh *mesh)
{
	m_mesh = mesh;
	m_texture = NULL;

	m_renderState = new RENDERSTATE_DEFAULT;
	ASSERT(m_renderState != NULL);

	m_currentSequenceStart = 0;
	m_currentSequenceEnd = 0;
	m_currentSequenceLoop = FALSE;
	m_thisFrame = 0;
	m_nextFrame = 0;
	m_interpolation = 0.0f;
	m_isRunningTempSequence = FALSE;
	m_oldSequenceLoop = FALSE;
}

KeyframeMeshInstance::~KeyframeMeshInstance()
{
	SAFE_DELETE(m_renderState);
}

void KeyframeMeshInstance::OnUpdate(float delta)
{
	if (m_currentSequenceStart != m_currentSequenceEnd)
	{
		m_interpolation += delta * 10;
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

void KeyframeMeshInstance::SetSequence(uint startFrame, uint endFrame, BOOL loop)
{
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

void KeyframeMeshInstance::SetSequence(const stl::string &name, BOOL loop)
{
	if (m_currentSequenceName == name)
		return;

	const AnimationSequence *sequence = GetMesh()->GetAnimation(name);
	ASSERT(sequence != NULL);
	SetSequence(sequence->start, sequence->stop, loop);
	m_currentSequenceName = name;
}

void KeyframeMeshInstance::RunSequenceOnce(const stl::string &name)
{
	if (m_isRunningTempSequence)
		return;

	m_oldSequenceName = m_currentSequenceName;
	m_oldSequenceLoop = m_currentSequenceLoop;

	m_isRunningTempSequence = TRUE;

	SetSequence(name, FALSE);
}

void KeyframeMeshInstance::RecoverFromTempSequence()
{
	m_isRunningTempSequence = FALSE;
	SetSequence(m_oldSequenceName, m_oldSequenceLoop);
	m_oldSequenceName.clear();
	m_oldSequenceLoop = false;
}

