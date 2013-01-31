#ifndef __FRAMEWORK_OPERATINGSYSTEM_H_INCLUDED__
#define __FRAMEWORK_OPERATINGSYSTEM_H_INCLUDED__

#include "common.h"

class BaseGameApp;
class FileSystem;
class GameWindow;
class Mouse;
class Keyboard;
class Touchscreen;
struct GameWindowParams;

/**
 * Interface for an operating system management class which handles
 * low level, non-graphics/window related, details including accepting
 * and passing off incoming operating system raised events.
 */
class OperatingSystem
{
public:
	OperatingSystem()                                      {}
	virtual ~OperatingSystem()                             {}

	/**
	 * Initializes internal platform specific objects that wrap access
	 * to various operating system features so the game application class
	 * can make use of them.
	 * @return TRUE if successful, FALSE if not
	 */
	virtual BOOL Initialize() = 0;

	/**
	 * Creates a game window object for the given game application object.
	 * @param gameApp the game application object to create the window for
	 * @param params platform-specific window creation parameters
	 * @return TRUE if successful, FALSE if not
	 */
	virtual BOOL CreateGameWindow(BaseGameApp *gameApp, GameWindowParams *params) = 0;

	/**
	 * Checks for and handles any events raised by the operating system.
	 */
	virtual void ProcessEvents() = 0;

	/**
	 * Signals that the system should close as soon as possible, passing
	 * this signal up the chain to the window object.
	 */
	virtual void Quit() = 0;

	/**
	 * @return TRUE if the system has been signaled to quit
	 */
	virtual BOOL IsQuitting() const = 0;

	/**
	 * @return TRUE if hardware support for OpenGL shaders was detected
	 */
	virtual BOOL HasShaderSupport() const = 0;

	/**
	 * @return the version of shader support that the OpenGL hardware has
	 */
	virtual float GetSupportedShaderVersion() const = 0;

	/**
	 * @return the current game window object, or NULL if one does not exist yet
	 */
	virtual GameWindow* GetWindow() const = 0;

	/**
	 * @return file system object, or NULL if not available
	 */
	virtual FileSystem* GetFileSystem() const = 0;

	/**
	 * @return mouse input device object, or NULL if not available
	 */
	virtual Mouse* GetMouse() const = 0;

	/**
	 * @return keyboard input device object, or NULL if not available
	 */
	virtual Keyboard* GetKeyboard() const = 0;

	/**
	 * @return touchscreen input device object, or NULL if not available
	 */
	virtual Touchscreen* GetTouchscreen() const = 0;

	/**
	 * @return the number of milliseconds since program execution began
	 */
	virtual uint32_t GetTicks() const = 0;

	/**
	 * Blocks execution until the specified number of milliseconds has passed.
	 */
	virtual void Delay(uint32_t milliseconds) const = 0;
};

#endif
