#ifndef __GAME_DEBUGINFOPROCESS_H_INCLUDED__
#define __GAME_DEBUGINFOPROCESS_H_INCLUDED__

#include "../processes/gameprocess.h"
#include "../framework/util/typesystem.h"

class GameState;
class ProcessManager;
class RenderContext;

class DebugInfoProcess : public GameProcess
{
public:
	TYPE_DEFINE(GAMEPROCESS_TYPE, "DebugInfoProcess");

	DebugInfoProcess(GameState *gameState, ProcessManager *processManager);
	virtual ~DebugInfoProcess();

	void OnRender(RenderContext *renderContext);
};

#endif

