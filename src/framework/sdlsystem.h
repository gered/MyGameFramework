#ifdef SDL
#ifndef __FRAMEWORK_SDLSYSTEM_H_INCLUDED__
#define __FRAMEWORK_SDLSYSTEM_H_INCLUDED__

#include "common.h"
#include "sdlincludes.h"

#include "operatingsystem.h"
#include "sdlgamewindow.h"
#include "file/sdlfilesystem.h"

class BaseGameApp;
class FileSystem;
class GameWindow;
class Mouse;
class Keyboard;
class Touchscreen;
class SDLFileSystem;
class SDLGameWindow;
class SDLMouse;
class SDLKeyboard;
struct GameWindowParams;

class SDLSystem : public OperatingSystem
{
public:
	SDLSystem();
	virtual ~SDLSystem();

	bool Initialize();
	bool CreateGameWindow(BaseGameApp *gameApp, GameWindowParams *params);
	void ProcessEvents();
	void Quit();

	bool IsQuitting() const                                { return m_isQuitting; }

	bool HasShaderSupport() const                          { return m_hasShaderSupport; }
	float GetSupportedShaderVersion() const                { return m_supportedShaderVersion; }

	GameWindow* GetWindow() const                          { return m_window; }
	FileSystem* GetFileSystem() const                      { return m_filesystem; }
	Mouse* GetMouse() const                                { return (Mouse*)m_mouse; }
	Keyboard* GetKeyboard() const                          { return (Keyboard*)m_keyboard; }
	Touchscreen* GetTouchscreen() const                    { return NULL; }

	uint GetTicks() const                                  { return SDL_GetTicks(); }
	void Delay(uint milliseconds) const                    { return SDL_Delay(milliseconds); }

private:
	bool m_isQuitting;

	SDLGameWindow *m_window;
	SDLFileSystem *m_filesystem;
	SDLMouse *m_mouse;
	SDLKeyboard *m_keyboard;

	bool m_hasShaderSupport;
	float m_supportedShaderVersion;
};

#endif
#endif

