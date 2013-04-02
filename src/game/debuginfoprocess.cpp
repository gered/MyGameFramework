#include "debuginfoprocess.h"

#include "../gameapp.h"
#include "../contexts/contentcache.h"
#include "../contexts/rendercontext.h"
#include "../entities/entity.h"
#include "../entities/entitymanager.h"
#include "../framework/content/contentmanager.h"
#include "../framework/content/spritefontparam.h"
#include "../framework/graphics/graphicsdevice.h"
#include "../framework/graphics/spritebatch.h"
#include "../framework/graphics/spritefont.h"
#include "../framework/graphics/viewcontext.h"
#include "../framework/math/camera.h"
#include "../states/gamestate.h"
#include <stl/string.h>

const Color TEXT_COLOR = COLOR_WHITE;

DebugInfoProcess::DebugInfoProcess(GameState *gameState, ProcessManager *processManager)
	: GameProcess(gameState, processManager)
{
}

DebugInfoProcess::~DebugInfoProcess()
{
}

void DebugInfoProcess::OnRender(RenderContext *renderContext)
{
	SpriteFont *font = GetGameApp()->GetContentCache()->GetUIFont();

	int y = 5;
	renderContext->GetSpriteBatch()->Printf(font, 5, y, TEXT_COLOR, "Loops: %d\nFT: %.6fs\nRPS: %d (%d)\nUPS: %d (%d)", 
		GetGameApp()->GetFPS(), 
		GetGameApp()->GetFrameTime(),
		GetGameApp()->GetRendersPerSecond(), GetGameApp()->GetRenderTime(),
		GetGameApp()->GetUpdatesPerSecond(), GetGameApp()->GetUpdateTime()
		);
	y += font->GetLetterHeight() * 3;

	renderContext->GetSpriteBatch()->Printf(font, 5, (y += font->GetLetterHeight()), TEXT_COLOR, "UI Scale: %d", GetGameApp()->GetScreenScale());
	renderContext->GetSpriteBatch()->Printf(font, 5, (y += font->GetLetterHeight()), TEXT_COLOR, "CP: %f, %f, %f", 
			renderContext->GetGraphicsDevice()->GetViewContext()->GetCamera()->GetPosition().x, 
			renderContext->GetGraphicsDevice()->GetViewContext()->GetCamera()->GetPosition().y, 
			renderContext->GetGraphicsDevice()->GetViewContext()->GetCamera()->GetPosition().z
			);
}
