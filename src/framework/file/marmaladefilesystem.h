#ifdef __S3E__
#ifndef __FRAMEWORK_FILE_MARMALADEFILESYSTEM_H_INCLUDED__
#define __FRAMEWORK_FILE_MARMALADEFILESYSTEM_H_INCLUDED__

#include <stl/string.h>

#include "filesystem.h"

class File;

class MarmaladeFileSystem : public FileSystem
{
public:
	MarmaladeFileSystem();
	virtual ~MarmaladeFileSystem();

	File* Open(const stl::string &filename, int mode);

	stl::string TranslateFilePath(const stl::string &filename) const;
	const stl::string& GetAssetsPath() const               { return m_assetsPath; }

private:
	File* OpenFile(const stl::string &filename, int mode);
	File* OpenMemory(const stl::string &filename, int mode);

	stl::string m_assetsPath;
};

#endif
#endif
