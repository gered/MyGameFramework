#ifdef SDL
#ifndef __FRAMEWORK_FILE_SDLFILESYSTEM_H_INCLUDED__
#define __FRAMEWORK_FILE_SDLFILESYSTEM_H_INCLUDED__

#include <stl/string.h>

#include "filesystem.h"

class File;

class SDLFileSystem : public FileSystem
{
public:
	SDLFileSystem();
	virtual ~SDLFileSystem();

	File* Open(const stl::string &filename, int mode);

	stl::string TranslateFilePath(const stl::string &filename) const;
	const stl::string& GetAssetsPath() const                  { return m_assetsPath; }

private:
	File* OpenFile(const stl::string &filename, int mode);
	File* OpenMemory(const stl::string &filename, int mode);

	stl::string m_assetsPath;
};

#endif
#endif

