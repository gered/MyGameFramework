#include "../framework/debug.h"
#include "../framework/log.h"

#include "statemanager.h"
#include "gamestate.h"
#include "stateinfo.h"
#include "../gameapp.h"
#include "../effects/effectmanager.h"
#include "../processes/processmanager.h"

#define LOGCAT_STATEMANAGER "STATEMANAGER"

StateManager::StateManager(GameApp *gameApp)
{
	ASSERT(gameApp != NULL);
	m_gameApp = gameApp;
	m_stateReturnValue = 0;
	m_hasStateReturnValue = false;
	m_pushQueueHasOverlay = false;
	m_swapQueueHasOverlay = false;
	m_lastCleanedStatesWereAllOverlays = false;
}

StateManager::~StateManager()
{
	LOG_INFO(LOGCAT_STATEMANAGER, "StateManager shutting down.\n");

	while (!m_states.empty())
	{
		StateInfo *stateInfo = m_states.back();
		LOG_INFO(LOGCAT_STATEMANAGER, "Popping state %s as part of StateManager shutdown.\n", stateInfo->GetDescriptor().c_str());
		stateInfo->gameState->OnPop();
		SAFE_DELETE(stateInfo->gameState);
		SAFE_DELETE(stateInfo);
		m_states.pop_back();
	}

	// these queues likely will not have anything in them, but just in case...
	while (!m_pushQueue.empty())
	{
		StateInfo *stateInfo = m_pushQueue.front();
		LOG_INFO(LOGCAT_STATEMANAGER, "Deleting push-queued state %s as part of StateManager shutdown.\n", stateInfo->GetDescriptor().c_str());
		SAFE_DELETE(stateInfo->gameState);
		SAFE_DELETE(stateInfo);
		m_pushQueue.pop_front();
	}
	while (!m_swapQueue.empty())
	{
		StateInfo *stateInfo = m_swapQueue.front();
		LOG_INFO(LOGCAT_STATEMANAGER, "Deleting swap-queued state %s as part of StateManager shutdown.\n", stateInfo->GetDescriptor().c_str());
		SAFE_DELETE(stateInfo->gameState);
		SAFE_DELETE(stateInfo);
		m_swapQueue.pop_front();
	}
}

void StateManager::Pop()
{
	ASSERT(IsTransitioning() == false);
	LOG_INFO(LOGCAT_STATEMANAGER, "Pop initiated for top-most state only.\n");
	StartOnlyTopStateTransitioningOut(false);
}

void StateManager::PopTopNonOverlay()
{
	ASSERT(IsTransitioning() == false);
	LOG_INFO(LOGCAT_STATEMANAGER, "Pop initiated for all top active states.\n");
	StartTopStatesTransitioningOut(false);
}

bool StateManager::HasState(const stl::string &name) const
{
	for (StateInfoList::const_iterator i = m_states.begin(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (stateInfo->name.length() > 0 && name == stateInfo->name)
			return true;
	}
	
	return false;
}

void StateManager::OnAppGainFocus()
{
	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (!stateInfo->isInactive)
			stateInfo->gameState->OnAppGainFocus();
	}
}

void StateManager::OnAppLostFocus()
{
	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (!stateInfo->isInactive)
			stateInfo->gameState->OnAppLostFocus();
	}
}

void StateManager::OnAppPause()
{
	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (!stateInfo->isInactive)
			stateInfo->gameState->OnAppPause();
	}
}

void StateManager::OnAppResume()
{
	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (!stateInfo->isInactive)
			stateInfo->gameState->OnAppResume();
	}
}

void StateManager::OnLostContext()
{
	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (!stateInfo->isInactive)
			stateInfo->gameState->OnLostContext();
	}
}

void StateManager::OnNewContext()
{
	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (!stateInfo->isInactive)
			stateInfo->gameState->OnNewContext();
	}
}

void StateManager::OnRender(RenderContext *renderContext)
{
	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (!stateInfo->isInactive)
		{
			stateInfo->gameState->OnRender(renderContext);
			stateInfo->gameState->GetEffectManager()->OnRenderGlobal(renderContext);
		}
	}
}

void StateManager::OnResize()
{
	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (!stateInfo->isInactive)
			stateInfo->gameState->OnResize();
	}
}

void StateManager::OnUpdate(float delta)
{
	// clear return values (ensuring they're only accessible for 1 tick)
	m_stateReturnValue = 0;
	m_hasStateReturnValue = false;
	m_lastCleanedStatesWereAllOverlays = false;

	CleanupInactiveStates();
	CheckForFinishedStates();
	ProcessQueues();
	ResumeStatesIfNeeded();
	UpdateTransitions(delta);

	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (!stateInfo->isInactive)
			stateInfo->gameState->OnUpdate(delta);
	}
}

void StateManager::ProcessQueues()
{
	// don't do anything if stuff is currently transitioning
	if (IsTransitioning())
		return;

	ASSERT(m_pushQueue.empty() == true || m_swapQueue.empty() == true);

	// for each state in the queue, add it to the main list and start it 
	// transitioning in
	while (!m_pushQueue.empty())
	{
		StateInfo *stateInfo = m_pushQueue.front();

		if (!m_states.empty())
		{
			// if this new state is an overlay, and the current top state is both
			// currently active and is not currently marked as being overlay-ed
			// then we should pause it due to overlay
			StateInfo *currentTopStateInfo = GetTop();
			if (stateInfo->isOverlay && !currentTopStateInfo->isInactive && !currentTopStateInfo->isOverlayed)
			{
				LOG_INFO(LOGCAT_STATEMANAGER, "Pausing %sstate %s due to overlay.\n", (currentTopStateInfo->isOverlay ? "overlay " : ""), currentTopStateInfo->GetDescriptor().c_str());
				currentTopStateInfo->gameState->OnPause(true);
			
				// also mark the current top state as being overlay-ed
				currentTopStateInfo->isOverlayed = true;
			}
		}

		LOG_INFO(LOGCAT_STATEMANAGER, "Pushing %sstate %s from push-queue.\n", (stateInfo->isOverlay ? "overlay " : ""), stateInfo->GetDescriptor().c_str());
		stateInfo->gameState->OnPush();

		TransitionIn(stateInfo, false);

		m_states.push_back(stateInfo);

		m_pushQueue.pop_front();
	}
	while (!m_swapQueue.empty())
	{
		StateInfo *stateInfo = m_swapQueue.front();

		// if this new state is an overlay, and the current top state is both
		// currently active and is not currently marked as being overlay-ed
		// then we should pause it due to overlay
		StateInfo *currentTopStateInfo = GetTop();
		if (stateInfo->isOverlay && !currentTopStateInfo->isInactive && !currentTopStateInfo->isOverlayed)
		{
			LOG_INFO(LOGCAT_STATEMANAGER, "Pausing %sstate %s due to overlay.\n", (currentTopStateInfo->isOverlay ? "overlay " : ""), currentTopStateInfo->GetDescriptor().c_str());
			currentTopStateInfo->gameState->OnPause(true);
			
			// also mark the current top state as being overlay-ed
			currentTopStateInfo->isOverlayed = true;
		}

		LOG_INFO(LOGCAT_STATEMANAGER, "Pushing %sstate %s from swap-queue.\n", (stateInfo->isOverlay ? "overlay " : ""), stateInfo->GetDescriptor().c_str());
		stateInfo->gameState->OnPush();

		TransitionIn(stateInfo, false);

		m_states.push_back(stateInfo);

		m_swapQueue.pop_front();
	}

	m_pushQueueHasOverlay = false;
	m_swapQueueHasOverlay = false;
}

void StateManager::ResumeStatesIfNeeded()
{
	if (m_states.empty())
		return;

	// don't do anything if stuff is currently transitioning
	if (IsTransitioning())
		return;

	// did we just clean up one or more overlay states?
	if (m_lastCleanedStatesWereAllOverlays)
	{
		// then we need to resume the current top state (flagged as "from an overlay")
		StateInfo *stateInfo = GetTop();
		ASSERT(stateInfo->isInactive == false);
		ASSERT(stateInfo->isOverlayed == true);

		LOG_INFO(LOGCAT_STATEMANAGER, "Resuming %sstate %s due to overlay removal.\n", (stateInfo->isOverlay ? "overlay " : ""), stateInfo->GetDescriptor().c_str());
		stateInfo->gameState->OnResume(true);

		stateInfo->isOverlayed = false;

		return;
	}

	// if the top state is not inactive, then we don't need to resume anything
	if (!GetTop()->isInactive)
		return;

	LOG_INFO(LOGCAT_STATEMANAGER, "Top-most state is inactive. Resuming top states.\n");

	// top state is inactive, time to resume one or more states...
	// find the topmost non-overlay state and take it and all overlay
	// states that are above it and transition them in
	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;

		LOG_INFO(LOGCAT_STATEMANAGER, "Resuming %sstate %s.\n", (stateInfo->isOverlay ? "overlay " : ""), stateInfo->GetDescriptor().c_str());
		stateInfo->gameState->OnResume(false);

		TransitionIn(stateInfo, true);
	}
}

void StateManager::UpdateTransitions(float delta)
{
	if (m_states.empty())
		return;

	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (stateInfo->isTransitioning)
		{
			bool isDone = stateInfo->gameState->OnTransition(delta, stateInfo->isTransitioningOut, stateInfo->isTransitionStarting);
			if (isDone)
			{
				LOG_INFO(LOGCAT_STATEMANAGER, "Transition %s %sstate %s finished.\n", (stateInfo->isTransitioningOut ? "out of" : "into"), (stateInfo->isOverlay ? "overlay " : ""), stateInfo->GetDescriptor().c_str());

				// if the state was being transitioned out, then we should mark
				// it as inactive, and trigger it's OnPop or OnPause event now
				if (stateInfo->isTransitioningOut)
				{
					if (stateInfo->isBeingPopped)
					{
						LOG_INFO(LOGCAT_STATEMANAGER, "Popping %sstate %s.\n", (stateInfo->isOverlay ? "overlay " : ""), stateInfo->GetDescriptor().c_str());
						stateInfo->gameState->OnPop();

						if (stateInfo->gameState->HasReturnValue())
						{
							m_stateReturnValue = stateInfo->gameState->GetReturnValue();
							m_hasStateReturnValue = true;
							LOG_INFO(LOGCAT_STATEMANAGER, "Return value of %d retrieved from %s.\n", m_stateReturnValue, stateInfo->GetDescriptor().c_str());
						}
					}
					else
					{
						LOG_INFO(LOGCAT_STATEMANAGER, "Pausing %sstate %s.\n", (stateInfo->isOverlay ? "overlay " : ""), stateInfo->GetDescriptor().c_str());
						stateInfo->gameState->OnPause(false);
					}
					stateInfo->isInactive = true;
				}

				// done transitioning
				stateInfo->isTransitioning = false;
				stateInfo->isTransitioningOut = false;
			}
			stateInfo->isTransitionStarting = false;
		}
	}
}

void StateManager::TransitionOut(StateInfo *stateInfo, bool forPopping)
{
	stateInfo->isTransitioning = true;
	stateInfo->isTransitioningOut = true;
	stateInfo->isTransitionStarting = true;
	stateInfo->isBeingPopped = forPopping;
	LOG_INFO(LOGCAT_STATEMANAGER, "Transition out of %sstate %s started.\n", (stateInfo->isOverlay ? "overlay " : ""), stateInfo->GetDescriptor().c_str());

	if (forPopping)
		stateInfo->gameState->GetProcessManager()->RemoveAll();
	else
		stateInfo->gameState->GetProcessManager()->OnPause(false);
}

void StateManager::TransitionIn(StateInfo *stateInfo, bool forResuming)
{
	stateInfo->isInactive = false;
	stateInfo->isTransitioning = true;
	stateInfo->isTransitioningOut = false;
	stateInfo->isTransitionStarting = true;
	LOG_INFO(LOGCAT_STATEMANAGER, "Transition into %sstate %s started.\n", (stateInfo->isOverlay ? "overlay " : ""), stateInfo->GetDescriptor().c_str());

	if (forResuming)
		stateInfo->gameState->GetProcessManager()->OnResume(false);
}

void StateManager::QueueForPush(StateInfo *newStateInfo)
{
	//ASSERT(IsTransitioning() == false);
	//ASSERT(m_swapQueue.empty() == true);
	ASSERT(newStateInfo != NULL);
	ASSERT(newStateInfo->gameState != NULL);
	ASSERT(m_pushQueueHasOverlay == false || (m_pushQueueHasOverlay == true && newStateInfo->isOverlay == true));

	LOG_INFO(LOGCAT_STATEMANAGER, "Queueing state %s for pushing.\n", newStateInfo->GetDescriptor().c_str());

	if (!newStateInfo->isOverlay)
		StartTopStatesTransitioningOut(true);

	m_pushQueue.push_back(newStateInfo);

	if (newStateInfo->isOverlay)
		m_pushQueueHasOverlay = true;
}

void StateManager::QueueForSwap(StateInfo *newStateInfo, bool swapTopNonOverlay)
{
	//ASSERT(IsTransitioning() == false);
	//ASSERT(m_pushQueue.empty() == true);
	ASSERT(newStateInfo != NULL);
	ASSERT(newStateInfo->gameState != NULL);
	ASSERT(m_swapQueueHasOverlay == false || (m_swapQueueHasOverlay == true && newStateInfo->isOverlay == true));

	LOG_INFO(LOGCAT_STATEMANAGER, "Queueing state %s for swapping with %s.\n", newStateInfo->GetDescriptor().c_str(), (swapTopNonOverlay ? "all top active states" : "only top-most active state."));

	if (swapTopNonOverlay)
		StartTopStatesTransitioningOut(false);
	else
		StartOnlyTopStateTransitioningOut(false);

	m_swapQueue.push_back(newStateInfo);

	if (newStateInfo->isOverlay)
		m_swapQueueHasOverlay = true;
}

StateInfo* StateManager::GetTopNonOverlay() const
{
	StateInfoList::const_reverse_iterator itor = m_states.rbegin();
	while (itor != m_states.rend() && (*itor)->isOverlay)
		++itor;

	return (*itor);
}

StateInfoList::iterator StateManager::GetTopNonOverlayItor()
{
	// TODO: probably a more efficient way to do this using reverse iterators
	StateInfoList::iterator result = m_states.end();
	for (StateInfoList::iterator i = m_states.begin(); i != m_states.end(); ++i)
	{
		if ((*i)->isOverlay == false)
			result = i;
	}

	return result;
}

StateInfo* StateManager::GetStateInfoFor(const GameState *state) const
{
	ASSERT(state != NULL);
	for (StateInfoList::const_iterator i = m_states.begin(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (stateInfo->gameState == state)
			return stateInfo;
	}

	return NULL;
}

bool StateManager::IsTransitioning() const
{
	for (StateInfoList::const_iterator i = m_states.begin(); i != m_states.end(); ++i)
	{
		const StateInfo *stateInfo = *i;
		if (stateInfo->isTransitioning)
			return true;
	}

	return false;
}

void StateManager::StartTopStatesTransitioningOut(bool pausing)
{
	for (StateInfoList::iterator i = GetTopNonOverlayItor(); i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		// only look at active states, since inactive ones have already been 
		// transitioned out and will be removed on the next OnUpdate()
		if (!stateInfo->isInactive)
			TransitionOut(stateInfo, !pausing);
	}
}

void StateManager::StartOnlyTopStateTransitioningOut(bool pausing)
{
	StateInfo *stateInfo = GetTop();
	// if it's not active, then it's just been transitioned out and will be
	// removed on the next OnUpdate()
	if (!stateInfo->isInactive)
		TransitionOut(stateInfo, !pausing);
}

void StateManager::CleanupInactiveStates()
{
	if (m_states.empty())
		return;

	// we don't want to remove any states until everything is done transitioning.
	// this is to avoid the scenario where the top non-overlay state finishes 
	// transitioning before one of the overlays. if we removed it, the overlays
	// would then be overlayed over an inactive non-overlay (which wouldn't get
	// resumed until the current active overlays were done being transitioned)
	if (IsTransitioning())
		return;

	bool cleanedUpSomething = false;
	bool cleanedUpNonOverlay = false;

	StateInfoList::iterator i = m_states.begin();
	while (i != m_states.end())
	{
		StateInfo *stateInfo = *i;
		if (stateInfo->isInactive && stateInfo->isBeingPopped)
		{
			cleanedUpSomething = true;
			if (!stateInfo->isOverlay)
				cleanedUpNonOverlay = true;

			m_states.erase(i++);

			LOG_INFO(LOGCAT_STATEMANAGER, "Deleting inactive popped state %s.\n", stateInfo->GetDescriptor().c_str());
			SAFE_DELETE(stateInfo->gameState);
			SAFE_DELETE(stateInfo);
		}
		else
			++i;
	}

	if (cleanedUpSomething && !cleanedUpNonOverlay)
		m_lastCleanedStatesWereAllOverlays = true;
}

void StateManager::CheckForFinishedStates()
{
	if (m_states.empty())
		return;

	// don't do anything if something is currently transitioning
	if (IsTransitioning())
		return;

	bool needToAlsoTransitionOutOverlays = false;
	
	// check the top non-overlay state first to see if it's finished and
	// should be transitioned out
	StateInfo *topNonOverlayStateInfo = GetTopNonOverlay();
	if (!topNonOverlayStateInfo->isInactive && topNonOverlayStateInfo->gameState->IsFinished())
	{
		LOG_INFO(LOGCAT_STATEMANAGER, "State %s marked as finished.\n", topNonOverlayStateInfo->GetDescriptor().c_str());
		TransitionOut(topNonOverlayStateInfo, true);
		
		needToAlsoTransitionOutOverlays = true;
	}
	
	// now also check the overlay states (if there were any). we force them to
	// transition out if the non-overlay state started to transition out so that
	// we don't end up with overlay states without a parent non-overlay state
	
	// start the loop off 1 beyond the top non-overlay (which is where the
	// overlays are, if any)
	StateInfoList::iterator i = ++GetTopNonOverlayItor();
	for (; i != m_states.end(); ++i)
	{
		StateInfo *stateInfo = *i;
		if (!stateInfo->isInactive && (stateInfo->gameState->IsFinished() || needToAlsoTransitionOutOverlays))
		{
			LOG_INFO(LOGCAT_STATEMANAGER, "State %s marked as finished.\n", stateInfo->GetDescriptor().c_str());
			TransitionOut(stateInfo, true);
		}
	}
}
