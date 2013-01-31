#ifndef __FRAMEWORK_FILE_UTIL_H_INCLUDED__
#define __FRAMEWORK_FILE_UTIL_H_INCLUDED__

#include <stl/string.h>

/**
 * @return the path to the currently running executable
 */
const stl::string& GetAppPath();

/**
 * @return the path to the assets directory
 */
const stl::string& GetAssetsPath();

#endif
