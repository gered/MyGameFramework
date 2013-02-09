BUILD_DIR         = "build"

SDL_ROOT = os.getenv("SDL_ROOT")
if not SDL_ROOT and string.find(_ACTION, "vs") then
	printf("ERROR: Environment variable SDL_ROOT is not set.")
	os.exit()
elseif not SDL_ROOT then
	SDL_ROOT = ""
end

if _ACTION == "clean" then
	os.rmdir(BUILD_DIR)
end

solution "EngineTest"
	configurations { "Debug", "Release" }
	location (BUILD_DIR .. "/" .. _ACTION)
	
project "EngineTest"
	kind "WindowedApp"
	language "C++"
	location (BUILD_DIR .. "/" .. _ACTION)
	files {
		"./src/**.c*",
		"./src/**.h",
		"./lib/**.c*",
		"./lib/**.h",
	}
	includedirs {
		"./lib/stl/include",
		"./lib/portable-crt/include",
		"./lib/eastl/include",
		"./lib/glew",
		"./lib/gwen",
		"./lib/stb",
	}
	defines {
		"DESKTOP",
		"SDL",
		"GLEW_STATIC",
	}
	debugdir "."
	
	---- PLATFORM SPECIFICS ----------------------------------------------------
	configuration "vs*"
		flags {
			"NoPCH",
			"NoMinimalRebuild"
		}
		buildoptions { "/MP" }
		links {
			"SDLmain",
			"SDL",
			"opengl32",
			"glu32",
		}
		defines {
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_NONSTDC_NO_WARNINGS"
		}
		includedirs {
			(SDL_ROOT .. "/include"),
		}
		
	configuration { "vs*", "Debug" }
		libdirs {
			(SDL_ROOT .. "/VisualC/SDL/Debug/"),
			(SDL_ROOT .. "/VisualC/SDLmain/Debug/"),
		}
		
	configuration { "vs*", "Release" }
		libdirs {
			(SDL_ROOT .. "/VisualC/SDL/Release/"),
			(SDL_ROOT .. "/VisualC/SDLmain/Release/"),
		}
		
	configuration "gmake"
		kind "ConsoleApp"
		buildoptions { "-Wall" }
		
	configuration { "windows", "gmake" }
		kind "WindowedApp"
		defines {
			"_GNU_SOURCE=1",
			"main=SDL_main",
		}
		links {
			"mingw32",
			"SDLmain",
			"SDL",
			"opengl32",
			"glu32",
		}
		linkoptions {
			"-static-libgcc",
			"-static-libstdc++",
		}
		
	configuration "macosx"
		files {
			"./lib/sdl-osx/**.m",
			"./lib/sdl-osx/**.c*",
			"./lib/sdl-osx/**.h",
		}
		links {
			"OpenGL.framework",
			"SDL.framework",
			"Cocoa.framework",
		}
		includedirs {
			"/Library/Frameworks/SDL.framework/Headers",
			"/System/Library/Frameworks/OpenGL.framework/Headers",
		}
		
	configuration "linux"
		links {
			"SDL",
			"GL",
			"GLU",
		}
	----------------------------------------------------------------------------
			
	configuration "Debug"
		defines {
			"DEBUG",
			"DEBUG_ASSERT_BREAK",
		}
		flags { "Symbols" }
	
	configuration "Release"
		defines {
			"NDEBUG",
		}
		flags { "Optimize" }
