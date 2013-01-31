#ifndef __FRAMEWORK_ASSETS_MESHFILE_H_INCLUDED__
#define __FRAMEWORK_ASSETS_MESHFILE_H_INCLUDED__

#include "../common.h"

#include "chunkdescriptor.h"
#include <stl/string.h>

class File;

/**
 * Base class for mesh files that are loaded using a custom "chunked"
 * file format.
 */
class MeshFile
{
public:
	virtual ~MeshFile();

protected:
	/**
	 * Initializes basic mesh file information from the provided mesh file.
	 * @param file the mesh file this object is for
	 */
	MeshFile(File *file);

	/**
	 * Passes through the entire file collecting information for each chunk
	 * present in the file.
	 */
	void CollectChunks();

	/**
	 * Returns information about a specific chunk.
	 * @param name the name / identifier of the chunk to retrieve
	 * @return info about the chunk or NULL if the chunk wasn't found
	 */
	ChunkDescriptor* GetChunkDesc(const stl::string &name);

	/**
	 * @return the underlying file object for this mesh file
	 */
	File* GetFile()                                        { return m_file; }

private:
	File *m_file;
	ChunkMap m_chunks;

};

#endif
