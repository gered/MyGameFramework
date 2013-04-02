#ifdef __S3E__
#ifndef __FRAMEWORK_MARMALADESYSTEM_H_INCLUDED__
#define __FRAMEWORK_MARMALADESYSTEM_H_INCLUDED__

#include "common.h"

#include "operatingsystem.h"
#include "s3eTimer.h"

class MarmaladeFileSystem;
class MarmaladeGameWindow;
class MarmaladeKeyboard;
class MarmaladeMouse;
class MarmaladeTouchscreen;
class BaseGameApp;
class FileSystem;
class GameWindow;
class Mouse;
class Keyboard;
class Touchscreen;
struct GameWindowParams;
struct MarmaladeSystemEvent;

class MarmaladeSystem : public OperatingSystem
{
public:
	MarmaladeSystem();
	virtual ~MarmaladeSystem();

	bool Initialize();
	bool CreateGameWindow(BaseGameApp *gameApp, GameWindowParams *params);
	void ProcessEvents();
	int OnEvent(const MarmaladeSystemEvent *eventArgs);
	void Quit();

	bool IsQuitting() const                                { return m_isQuitting; }

	bool HasShaderSupport() const                          { return m_hasShaderSupport; }
	float GetSupportedShaderVersion() const                { return m_supportedShaderVersion; }

	GameWindow* GetWindow() const                          { return (GameWindow*)m_window; }
	FileSystem* GetFileSystem() const                      { return (FileSystem*)m_filesystem; }
	Mouse* GetMouse() const                                { return (Mouse*)m_mouse; }
	Keyboard* GetKeyboard() const                          { return (Keyboard*)m_keyboard; }
	Touchscreen* GetTouchscreen() const                    { return (Touchscreen*)m_touchscreen; }

	uint GetTicks() const                                  { return (uint)s3eTimerGetMs(); }
	void Delay(uint milliseconds) const;

private:
	bool m_isQuitting;

	MarmaladeGameWindow *m_window;
	MarmaladeFileSystem *m_filesystem;
	MarmaladeKeyboard *m_keyboard;
	MarmaladeMouse *m_mouse;
	MarmaladeTouchscreen *m_touchscreen;

	bool m_hasShaderSupport;
	float m_supportedShaderVersion;
};

#endif
#endif
