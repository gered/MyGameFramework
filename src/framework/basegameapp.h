#ifndef __FRAMEWORK_BASEGAMEAPP_H_INCLUDED__
#define __FRAMEWORK_BASEGAMEAPP_H_INCLUDED__

#include "common.h"

#include "operatingsystem.h"

class ContentManager;
class GameWindow;
class GraphicsDevice;
class Keyboard;
class Mouse;
class OperatingSystem;
class Touchscreen;
struct GameWindowParams;

/**
 * Base class for the game-specific main application class.
 */
class BaseGameApp
{
public:
	BaseGameApp();
	virtual ~BaseGameApp();

	/**
	 * Begins application initialization in preparation for running the
	 * main loop.
	 * @param system pre-initialized operating system class
	 * @return TRUE if successful, FALSE if not
	 */
	BOOL Start(OperatingSystem *system);

	/**
	 * Runs the main game loop.
	 */
	void Loop();

	/**
	 * @return operating system object
	 */
	OperatingSystem* GetOperatingSystem() const            { return m_system; }

	/**
	 * @return graphics device object, or NULL if initialization failed
	 */
	GraphicsDevice* GetGraphicsDevice() const              { return m_graphics; }

	/**
	 * @return keyboard input device object, or NULL if not available
	 */
	Keyboard* GetKeyboard() const                          { return m_system->GetKeyboard(); }

	/**
	 * @return mouse input device object, or NULL if not available
	 */
	Mouse* GetMouse() const                                { return m_system->GetMouse(); }

	/**
	 * @return touchscreen input device object, or NULL if not available
	 */
	Touchscreen* GetTouchscreen() const                    { return m_system->GetTouchscreen(); }

	/**
	 * @return content manager object, or NULL if initialization failed
	 */
	ContentManager* GetContentManager() const              { return m_content; }

	/**
	 * @return the number of frames that were rendered and updated during the last second
	 *         (this number includes skipped renders/updates in it's count)
	 */
	uint32_t GetFPS() const                                { return m_fps; }

	/**
	 * @return average time in seconds that each frame took during the last second
	 */
	float GetFrameTime() const                             { return m_frameTime; }

	/**
	 * @return total time in milliseconds that rendering took during the last second
	 */
	uint32_t GetRenderTime() const                         { return m_renderTime; }

	/**
	 * @return total time in milliseconds that updating game logic took during the last second
	 */
	uint32_t GetUpdateTime() const                         { return m_updateTime; }

	/**
	 * @return the number of frames rendered during the last second
	 */
	uint32_t GetRendersPerSecond() const                   { return m_rendersPerSecond; }

	/**
	 * @return the number of game logic updates during the last second
	 */
	uint32_t GetUpdatesPerSecond() const                   { return m_updatesPerSecond; }

	/**
	 * @return TRUE if the main loop is running behind 
	 *         (has to update game logic more then once per frame)
	 */
	BOOL IsRunningSlowing() const                          { return m_isRunningSlowly; }

	/**
	 * Sets the desired frames per second to run the main loop at.
	 * @param targetFrequency the number of frames to run the main loop at
	 */
	void SetUpdateFrequency(uint32_t targetFrequency);

	/**
	 * Sets the maximum number of frames to skip rendering per loop if the main loop is running slowly.
	 * @param frameSkip the maximum number of frames to skip rendering per loop
	 */
	void SetMaxFrameSkip(uint32_t frameSkip)               { m_maxFrameSkip = frameSkip; }

	/**
	 * Resets the time checking that determines if the main loop is running slowly.
	 * The next loop will begin by assuming it is running at full speed.
	 */
	void ResetNextUpdateTime()                             { m_nextUpdateAt = GetTicks(); }

	/**
	 * Sets the current frame as dirty which will ensure that the next run of the loop
	 * will render even if a game logic update has not been run since the last render
	 * was performed.
	 */
	void Invalidate()                                      { m_isDirty = TRUE; }

	unsigned int GetTicks() const                          { return m_system->GetTicks(); }
	
	/**
	 * @return TRUE if the app currently is active and has input focus
	 */
	BOOL IsAppFocused() const;

protected:
	/**
	 * Initializes the game window and the graphics and content management
	 * sub-systems.
	 * @param windowParams window creation parameters specific to the platform being run on
	 * @return TRUE if successful, FALSE if not
	 */
	BOOL Initialize(GameWindowParams *windowParams);

	/**
	 * Signals to the main loop that the application should quit as soon as possible.
	 */
	void Quit();

public:
	/**
	 * Application has gained input focus callback.
	 */
	virtual void OnAppGainFocus() = 0;

	/**
	 * Application has lost input focus callback.
	 */
	virtual void OnAppLostFocus() = 0;

	/**
	 * Application execution being paused callback.
	 */
	virtual void OnAppPause() = 0;

	/**
	 * Application execution being resumed callback.
	 */
	virtual void OnAppResume() = 0;

	/**
	 * Game-specific initialization (before content loading) callback.
	 */
	virtual BOOL OnInit() = 0;

	/**
	 * Initial game load after initialization success callback.
	 */
	virtual void OnLoadGame() = 0;

	/**
	 * Lost OpenGL graphics context callback.
	 */
	virtual void OnLostContext();

	/**
	 * New OpenGL graphics context creation callback.
	 */
	virtual void OnNewContext();

	/**
	 * Render callback.
	 */
	virtual void OnRender();

	/**
	 * Viewport resize callback.
	 */
	virtual void OnResize();

	/**
	 * Update (tick) callback.
	 * @param delta time since last update
	 */
	virtual void OnUpdate(float delta);

private:
	void InternalUpdate();

	OperatingSystem *m_system;
	GameWindow *m_window;
	BOOL m_stop;
	BOOL m_isPaused;

	uint32_t m_fps;
	float m_frameTime;
	uint32_t m_numRenders;
	uint32_t m_rendersPerSecond;
	uint32_t m_numUpdates;
	uint32_t m_updatesPerSecond;
	uint32_t m_renderTime;
	uint32_t m_updateTime;

	uint32_t m_targetUpdatesPerSecond;
	uint32_t m_ticksPerUpdate;
	uint32_t m_maxFrameSkip;
	float m_fixedUpdateInterval;
	uint32_t m_nextUpdateAt;
	BOOL m_isRunningSlowly;

	BOOL m_isDirty;

	GraphicsDevice *m_graphics;
	ContentManager *m_content;
};

#endif
