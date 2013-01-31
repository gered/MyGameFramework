#ifdef SDL
#include "../debug.h"
#include "../log.h"
#include "../common.h"

#include "sdlfilesystem.h"

#include "file.h"
#include "filesystem.h"
#include "sdlfile.h"
#include "memoryfile.h"
#include "util.h"

SDLFileSystem::SDLFileSystem()
	: FileSystem()
{
	STACK_TRACE;
	m_assetsPath = ::GetAssetsPath();
	LOG_INFO(LOGCAT_FILEIO, "FileSystem assets path is \"%s\".\n", m_assetsPath.c_str());
}

SDLFileSystem::~SDLFileSystem()
{
	STACK_TRACE;
}

File* SDLFileSystem::Open(const stl::string &filename, int mode)
{
	STACK_TRACE;
	File *result = NULL;
	stl::string realFilename = TranslateFilePath(filename);

	if (mode & FILEMODE_MEMORY)
		result = OpenMemory(realFilename, mode);
	else
		result = OpenFile(realFilename, mode);

	return result;
}

File* SDLFileSystem::OpenFile(const stl::string &filename, int mode)
{
	STACK_TRACE;
	SDLFile *file = new SDLFile();
	ASSERT(file != NULL);

	if (file->Open(filename, mode))
		return file;
	else
	{
		SAFE_DELETE(file);
		return NULL;
	}
}

File* SDLFileSystem::OpenMemory(const stl::string &filename, int mode)
{
	STACK_TRACE;
	// open the specified file off the disk (or whatever)
	File *file = OpenFile(filename, mode);
	if (file == NULL)
		return NULL;

	// read the entire file contents into memory
	MemoryFile *memoryFile = new MemoryFile();
	ASSERT(memoryFile != NULL);
	if (!memoryFile->Open(file))
	{
		SAFE_DELETE(memoryFile);   // sets NULL, so this returns NULL on failure
	}
	
	// close the source file, we're done with it either way
	file->Close();
	SAFE_DELETE(file);

	return memoryFile;
}

stl::string SDLFileSystem::TranslateFilePath(const stl::string &filename) const
{
	STACK_TRACE;
	if (filename.substr(0, 9) == "assets://")
		return m_assetsPath + filename.substr(9);
	else
		return filename;
}
#endif

