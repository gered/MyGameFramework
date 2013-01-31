#include "../debug.h"

#include "util.h"
#ifndef MOBILE
#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#define WIN32_EXTRA_LEAN
#include <windows.h>
#elif __linux__
#include <stdio.h>
#elif __APPLE__
#include <stdlib.h>
#include <mach-o/dyld.h>
#include <sys/param.h>
#include <sys/stat.h>
#else
#error Unsupported platform
#endif
#endif

stl::string g_appPath;
stl::string g_assetsPath;

const stl::string& GetAppPath()
{
	STACK_TRACE;
	if (g_appPath.length() > 0)
		return g_appPath;

#ifndef MOBILE
#ifdef _WIN32
	char pathBuffer[MAX_PATH + 1];
	DWORD result = GetCurrentDirectoryA(MAX_PATH, pathBuffer);
	ASSERT(result != 0);
	if (!result)
		return g_appPath;

	stl::string path = pathBuffer;
	g_appPath = path + "\\";
#elif __linux__
	// Code taken from: http://www.gamedev.net/community/forums/topic.asp?topic_id=459511
	stl::string path = "";
	pid_t pid = getpid();
	char buffer[20] = { 0 };
	sprintf(buffer, "%d", pid);
	stl::string link = "/proc/";
	link.append(buffer);
	link.append("/exe");
	char proc[512];
	int ch = readlink(link.c_str(), proc, 512);
	if (ch != -1)
	{
		proc[ch] = 0;
		path = proc;
		stl::string::size_type t = path.find_last_of("/");
		path = path.substr(0, t);
	}
	g_appPath = path + "/";
#elif __APPLE__
	uint32_t size = MAXPATHLEN;
	char *pathBuffer = new char[size];
	int result = _NSGetExecutablePath(pathBuffer, &size);
	if (result == -1)
	{
		delete[] pathBuffer;
		pathBuffer = new char[size];
		result = _NSGetExecutablePath(pathBuffer, &size);
		ASSERT(result == 0);
		if (result == -1)
			return g_appPath; // not sure why this would fail again (probably won't ever?)
	}
	// resolve symlinks (who cares .. ?)
	char *realPathBuffer = realpath(pathBuffer, NULL);
	ASSERT(realPathBuffer != NULL);
	if (realPathBuffer == NULL)
		return g_appPath;

	stl::string appPath = realPathBuffer;
	delete[] pathBuffer;
	free(realPathBuffer);

	// _NSGetExecutablePath actually returns the path to and including the
	// currently running executable, but we just want the path containing the
	// executable.
	stl::string::size_type pos = appPath.find_last_of('/');
	ASSERT(pos != stl::string::npos);
	g_appPath = appPath.substr(0, pos + 1);
#endif
#else
	g_appPath = "./";
#endif
	return g_appPath;
}

const stl::string& GetAssetsPath()
{
	STACK_TRACE;
	if (g_assetsPath.length() > 0)
		return g_assetsPath;
	
#ifndef MOBILE
#if defined(_WIN32) || defined(__linux__)
	g_assetsPath = GetAppPath() + "assets/";
#elif __APPLE__
	stl::string appPath = GetAppPath();
	
	// check if this binary is running from inside an app bundle directory
	if (appPath.length() > 20 && appPath.substr(appPath.length() - 20) == ".app/Contents/MacOS/")
	{
		// binary is running inside an app bundle. first get the path to
		// the "Contents" part of the app bundle
		stl::string appBundleContentsPath = appPath.substr(0, appPath.length() - 6);
		stl::string appBundleResourcesPath = appBundleContentsPath + "Resources";
		
		// we need to first check if the app bundle's "Resources" directory
		// exists, and if so, use that as the assets path.
		// if it does not exists, use a path to an "assets" directory
		// in the same directory containing the app bundle (that is, we
		// treat the app bundle as if it were the actual binary itself
		// and then put together the assets path like we do in Win/Linux)
		struct stat sb;
		if (stat(appBundleResourcesPath.c_str(), &sb) == 0 && S_ISDIR(sb.st_mode))
		{
			// the "Resources" directory exists
			g_assetsPath = appBundleResourcesPath + "/";
		}
		else
		{
			// fall back to use a path to "assets" in the same directory as
			// the main app bundle (".app") directory
			
			// get the directory that contains the app bundle
			size_t p = appPath.rfind('/', appPath.length() - 20);
			if (p == stl::string::npos)
				// weird! this would mean that the app bundle is at the root
				g_assetsPath = "/assets/";
			else
				g_assetsPath = appPath.substr(0, p) + "/assets/";
		}
	}
	else
		// just a standalone binary. no app bundle directory
		g_assetsPath = appPath + "assets/";
#endif
#else
#ifdef __S3E__
	g_assetsPath = GetAppPath();
#else
	g_assetsPath = GetAppPath() + "assets/";
#endif
#endif
	return g_assetsPath;
}
