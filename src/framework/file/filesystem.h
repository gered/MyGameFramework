#ifndef __FRAMEWORK_FILE_FILESYSTEM_H_INCLUDED__
#define __FRAMEWORK_FILE_FILESYSTEM_H_INCLUDED__

#include <stl/string.h>

class File;

/**
 * Represents the current platform's filesystem, provided methods
 * for opening files and for translating paths using the "assets://" 
 * and "storage://" prefixes to their equivalent locations understood 
 * by the underlying operating system.
 */
class FileSystem
{
public:
	FileSystem()                                           {}
	virtual ~FileSystem()                                  {}

	/**
	 * Opens a file.
	 * @param filename the path and filename of the file to open
	 * @param mode the mode to open the file in
	 * @return a file object for the opened file, or NULL if not successful
	 */
	virtual File* Open(const stl::string &filename, int mode) = 0;

	/**
	 * Translates a path and filename which may include a "assets://" or
	 * "storage://" prefix to the full path understood by the underlying
	 * platform / operating system.
	 * @param filename path and filename to translate
	 * @return the translated path
	 */
	virtual stl::string TranslateFilePath(const stl::string &filename) const = 0;

	/**
	 * @return the path to the root of the assets directory
	 */
	virtual const stl::string& GetAssetsPath() const = 0;
};

#endif
