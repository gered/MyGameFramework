#ifdef SDL
#ifndef __FRAMEWORK_FILE_SDLFILE_H_INCLUDED__
#define __FRAMEWORK_FILE_SDLFILE_H_INCLUDED__

#include "../common.h"
#include "file.h"

#include <stl/string.h>
#include "../sdlincludes.h"

class SDLFile : public File
{
public:
	SDLFile();
	virtual ~SDLFile();

	BOOL Open(const stl::string &filename, int mode);
	void Close();

	BOOL IsOpen() const                                    { return m_fp != NULL; }
	BOOL CanRead() const                                   { return m_canRead; }
	BOOL CanWrite() const                                  { return m_canWrite; }
	FileType GetFileType() const                           { return FILETYPE_IO; }

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
	BOOL AtEOF();
	size_t GetFileSize();

	const stl::string& GetFilename() const                 { return m_filename; }

private:
	SDL_RWops *m_fp;
	int m_mode;
	BOOL m_canRead;
	BOOL m_canWrite;
	stl::string m_filename;
};

#endif
#endif

