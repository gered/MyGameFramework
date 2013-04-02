#ifndef __FRAMEWORK_FILE_MEMORYFILE_H_INCLUDED__
#define __FRAMEWORK_FILE_MEMORYFILE_H_INCLUDED__

#include "../common.h"
#include "file.h"

/**
 * File class that uses a block of memory as it's backing storage for
 * I/O operations.
 */
class MemoryFile : public File
{
public:
	MemoryFile();
	virtual ~MemoryFile();

	bool Open(File *srcFile);
	bool Open(const void *memory, size_t numBytes, bool canRead, bool canWrite, bool assumeOwnershipOfMemory = false);
	void Close();

	bool IsOpen() const                                    { return m_data != NULL; }
	bool CanRead() const                                   { return m_canRead; }
	bool CanWrite() const                                  { return m_canWrite; }
	FileType GetFileType() const                           { return FILETYPE_MEMORY; }

	int8_t ReadChar();
	uint8_t ReadUnsignedChar();
	int16_t ReadShort();
	uint16_t ReadUnsignedShort();
	int32_t ReadInt();
	uint32_t ReadUnsignedInt();
	int64_t ReadLong();
	uint64_t ReadUnsignedLong();
	float ReadFloat();
	double ReadDouble();
	size_t Read(int8_t *buffer, size_t bytesToRead);
	size_t ReadString(stl::string &buffer);
	size_t ReadFixedString(stl::string &buffer, size_t maxLength);

	size_t WriteChar(int8_t data);
	size_t WriteUnsignedChar(uint8_t data);
	size_t WriteShort(int16_t data);
	size_t WriteUnsignedShort(uint16_t data);
	size_t WriteInt(int32_t data);
	size_t WriteUnsignedInt(uint32_t data);
	size_t WriteLong(int64_t data);
	size_t WriteUnsignedLong(uint64_t data);
	size_t WriteFloat(float data);
	size_t WriteDouble(double data);
	size_t Write(int8_t *buffer, size_t bytesToWrite);

	size_t Tell();
	void Seek(size_t offset, FileSeek from);
	bool AtEOF();
	size_t GetFileSize();

	const stl::string& GetFilename() const                 { return m_filename; }

	int8_t* GetFileData() const                            { return m_data; }

private:
	int8_t *m_data;
	bool m_ownData;
	size_t m_length;
	size_t m_position;
	bool m_canRead;
	bool m_canWrite;
	stl::string m_filename;
};

#endif
