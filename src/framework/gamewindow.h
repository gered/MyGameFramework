#ifndef __FRAMEWORK_GAMEWINDOW_H_INCLUDED__
#define __FRAMEWORK_GAMEWINDOW_H_INCLUDED__

#include "common.h"
#include "math/rect.h"
#include "screenorientation.h"

class BaseGameApp;
struct OSEvent;

struct GameWindowParams
{
	bool windowed;
};

/**
 * Interface for a game window management class which also manages
 * it's associated graphics context.
 */
class GameWindow
{
public:
	GameWindow(BaseGameApp *gameApp)
	{
		m_gameApp = gameApp;
	}

	virtual ~GameWindow()                                  {}

	/**
	 * Creates the game window.
	 * @param params platform specific window creation parameters
	 * @return true if successful, false if not
	 */
	virtual bool Create(GameWindowParams *params) = 0;

	/**
	 * Resizes the window to the given dimensions.
	 * @param width the new window width
	 * @param height the new window height
	 * @return true if successful, false if not
	 */
	virtual bool Resize(uint width, uint height) = 0;

	/**
	 * Toggles between fullscreen and windowed mode.
	 * @return true if successful, false if not
	 */
	virtual bool ToggleFullscreen() = 0;

	/**
	 * Signals to the underlying system that the window should be closed
	 * as soon as possible.
	 */
	virtual void Close() = 0;

	/**
	 * @return the current width of the window's client area
	 */
	virtual uint GetWidth() const = 0;

	/**
	 * @return the current height of the window's client area
	 */
	virtual uint GetHeight() const = 0;

	/** 
	 * @return the current dimensions of the window's client area
	 */
	virtual const Rect& GetRect() const = 0;

	/**
	 * @return the bits per pixel display format of the window's graphics context
	 */
	virtual uint GetBPP() const = 0;

	/**
	 * @return true if the current window is windowed, false if fullscreen
	 */
	virtual bool IsWindowed() const = 0;

	/**
	 * @return the current display rotation angle
	 */
	virtual SCREEN_ORIENTATION_ANGLE GetScreenOrientation() const = 0;

	/**
	 * @return true if this window is the currently active one
	 */
	virtual bool IsActive() const = 0;
	
	/**
	 * @return true if this window currently is active and has input focus
	 */
	virtual bool IsFocused() const = 0;

	/**
	 * @return true if the window has been signaled that it should close
	 */
	virtual bool IsClosing() const = 0;

	/**
	 * @return TRUe if the window currently has an active and usable OpenGL context associated with it
	 */
	virtual bool HasGLContext() const = 0;

	/**
	 * Performs platform specific processing for an event raised by the operating system.
	 * @param event the event arguments received by the operating system
	 */
	virtual void ProcessEvent(const OSEvent *event) = 0;

	/**
	 * Flips the display backbuffer to the frontbuffer.
	 */
	virtual void Flip() = 0;

protected:
	/**
	 * @return the parent game application object
	 */
	BaseGameApp* GetGameApp()                              { return m_gameApp; }

private:
	BaseGameApp *m_gameApp;

};

#endif
