#include "gameapp.h"
#include "framework/common.h"
#include "framework/debug.h"
#include "framework/log.h"
#include "framework/operatingsystem.h"
#include "framework/file/util.h"
#include <stl/string.h>
#include <math.h>

#if defined(DESKTOP) && defined(SDL)
#include "framework/sdlsystem.h"
#elif MOBILE
#include "framework/marmaladesystem.h"
#else
#error Unknown platform
#endif

int main(int argc, char **argv)
{
	LogStart();
	DebugInit();
	LOG_INFO(LOGCAT_SYSTEM, "Build compiled on %s %s\n", __DATE__, __TIME__);
#ifdef DESKTOP
	LOG_INFO(LOGCAT_SYSTEM, "App path is: %s\n", GetAppPath().c_str());
#endif

	OperatingSystem *system = NULL;

#if defined(DESKTOP) && defined(SDL)
	system = new SDLSystem();
#elif MOBILE
	system = new MarmaladeSystem();
#else
#error Unknown platform
#endif
	
	ASSERT(system != NULL);

	if (!system->Initialize())
	{
		SAFE_DELETE(system);

		LOG_ERROR(LOGCAT_SYSTEM, "Initialization failed. Exiting.\n");
		DebugClose();
		LogEnd();

		return 1;
	}

	GameApp *app = new GameApp();

	if (!app->Start(system))
	{
		SAFE_DELETE(app);
		SAFE_DELETE(system);

		LOG_ERROR(LOGCAT_SYSTEM, "Error while starting GameApp. Exiting.\n");
		DebugClose();
		LogEnd();

		return 1;
	}

	app->Loop();

	SAFE_DELETE(app);
	SAFE_DELETE(system);

	LOG_INFO(LOGCAT_SYSTEM, "Finished successfully.\n");
	DebugClose();
	LogEnd();

	return 0;
}
