#include "../debug.h"
#include "../log.h"

#include "memoryfile.h"

#include <string.h>

MemoryFile::MemoryFile()
{
	m_data = NULL;
	m_ownData = false;
	m_length = 0;
	m_position = 0;
	m_canRead = false;
	m_canWrite = false;
}

MemoryFile::~MemoryFile()
{
	Close();
}

bool MemoryFile::Open(File *srcFile)
{
	ASSERT(IsOpen() == false);
	ASSERT(srcFile->IsOpen());
	size_t filesize = srcFile->GetFileSize();
	m_data = new int8_t[filesize];
	ASSERT(m_data != NULL);
	m_ownData = true;
	m_length = filesize;
	m_filename = srcFile->GetFilename();

	m_canRead = srcFile->CanRead();
	m_canWrite = srcFile->CanWrite();

	srcFile->Read(m_data, filesize);

	LOG_INFO(LOGCAT_FILEIO, "Create MemoryFile from source file \"%s\"\n", srcFile->GetFilename().c_str());

	return true;
}

bool MemoryFile::Open(const void *memory, size_t numBytes, bool canRead, bool canWrite, bool assumeOwnershipOfMemory)
{
	ASSERT(IsOpen() == false);
	ASSERT(memory != NULL);
	ASSERT(numBytes > 0);
	ASSERT(canRead == true || canWrite == true);

	m_data = (int8_t*)memory;
	m_ownData = assumeOwnershipOfMemory;
	m_length = numBytes;
	m_filename.clear();

	m_canRead = canRead;
	m_canWrite = canWrite;

	return true;
}

void MemoryFile::Close()
{
	if (IsOpen())
	{
		if (m_ownData)
			SAFE_DELETE_ARRAY(m_data);

		if (m_filename.size() > 0)
			LOG_INFO(LOGCAT_FILEIO, "Free MemoryFile \"%s\"\n", m_filename.c_str());
	}

	m_ownData = false;
	m_length = 0;
	m_position = 0;
	m_canRead = false;
	m_canWrite = false;
	m_filename.clear();
}

int8_t MemoryFile::ReadChar()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	void *p = &m_data[m_position];
	m_position += sizeof(int8_t);
	return *(int8_t*)p;
}

uint8_t MemoryFile::ReadUnsignedChar()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	void *p = &m_data[m_position];
	m_position += sizeof(uint8_t);
	return *(uint8_t*)p;
}

int16_t MemoryFile::ReadShort()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	void *p = &m_data[m_position];
	m_position += sizeof(int16_t);
	return *(int16_t*)p;
}

uint16_t MemoryFile::ReadUnsignedShort()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	void *p = &m_data[m_position];
	m_position += sizeof(uint16_t);
	return *(uint16_t*)p;
}

int32_t MemoryFile::ReadInt()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	void *p = &m_data[m_position];
	m_position += sizeof(int32_t);
	return *(int32_t*)p;
}

uint32_t MemoryFile::ReadUnsignedInt()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	void *p = &m_data[m_position];
	m_position += sizeof(uint32_t);
	return *(uint32_t*)p;
}

int64_t MemoryFile::ReadLong()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	void *p = &m_data[m_position];
	m_position += sizeof(int64_t);
	return *(int64_t*)p;
}

uint64_t MemoryFile::ReadUnsignedLong()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	void *p = &m_data[m_position];
	m_position += sizeof(uint64_t);
	return *(uint64_t*)p;
}

float MemoryFile::ReadFloat()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	void *p = &m_data[m_position];
	m_position += sizeof(float);
	return *(float*)p;
}

double MemoryFile::ReadDouble()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	void *p = &m_data[m_position];
	m_position += sizeof(double);
	return *(double*)p;
}

size_t MemoryFile::Read(int8_t *buffer, size_t bytesToRead)
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	ASSERT(buffer != NULL);
	ASSERT(bytesToRead > 0);
	size_t actualBytesToRead = Min(bytesToRead, m_length - m_position);
	void *p = &m_data[m_position];
	memcpy(buffer, p, actualBytesToRead);
	m_position += actualBytesToRead;
	return actualBytesToRead;
}

size_t MemoryFile::ReadString(stl::string &buffer)
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	size_t charactersRead = 0;
	int8_t c;
	do
	{
		c = ReadChar();
		if (c)
		{
			buffer += c;
			++charactersRead;
		}
	} while (c != '\0');

	return charactersRead;
}


size_t MemoryFile::ReadFixedString(stl::string &buffer, size_t maxLength)
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	size_t charactersRead = 0;
	int8_t c;
	for (size_t i = 0; i < maxLength; ++i)
	{
		c = ReadChar();
		if (c)
		{
			buffer += c;
			++charactersRead;
		}
		if (c == '\0')
			break;
	}

	return charactersRead;
}

size_t MemoryFile::WriteChar(int8_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	int8_t *p = (int8_t*)&m_data[m_position];
	*p = data;
	m_position += sizeof(int8_t);
	return sizeof(int8_t);
}

size_t MemoryFile::WriteUnsignedChar(uint8_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	uint8_t *p = (uint8_t*)&m_data[m_position];
	*p = data;
	m_position += sizeof(uint8_t);
	return sizeof(uint8_t);
}

size_t MemoryFile::WriteShort(int16_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	int16_t *p = (int16_t*)&m_data[m_position];
	*p = data;
	m_position += sizeof(int16_t);
	return sizeof(int16_t);
}

size_t MemoryFile::WriteUnsignedShort(uint16_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	uint16_t *p = (uint16_t*)&m_data[m_position];
	*p = data;
	m_position += sizeof(uint16_t);
	return sizeof(uint16_t);
}

size_t MemoryFile::WriteInt(int32_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	int32_t *p = (int32_t*)&m_data[m_position];
	*p = data;
	m_position += sizeof(int32_t);
	return sizeof(int32_t);
}

size_t MemoryFile::WriteUnsignedInt(uint32_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	uint32_t *p = (uint32_t*)&m_data[m_position];
	*p = data;
	m_position += sizeof(uint32_t);
	return sizeof(uint32_t);
}

size_t MemoryFile::WriteLong(int64_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	int64_t *p = (int64_t*)&m_data[m_position];
	*p = data;
	m_position += sizeof(int64_t);
	return sizeof(int64_t);
}

size_t MemoryFile::WriteUnsignedLong(uint64_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	uint64_t *p = (uint64_t*)&m_data[m_position];
	*p = data;
	m_position += sizeof(uint64_t);
	return sizeof(uint64_t);
}

size_t MemoryFile::WriteFloat(float data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	float *p = (float*)&m_data[m_position];
	*p = data;
	m_position += sizeof(float);
	return sizeof(float);
}

size_t MemoryFile::WriteDouble(double data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	double *p = (double*)&m_data[m_position];
	*p = data;
	m_position += sizeof(double);
	return sizeof(double);
}

size_t MemoryFile::Write(int8_t *buffer, size_t bytesToWrite)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	ASSERT(buffer != NULL);
	ASSERT(bytesToWrite > 0);
	size_t actualBytesToWrite = Min(bytesToWrite, m_length - m_position);
	void *p = &m_data[m_position];
	memcpy(p, buffer, actualBytesToWrite);
	m_position += actualBytesToWrite;
	return actualBytesToWrite;
}

size_t MemoryFile::Tell()
{
	ASSERT(IsOpen());
	return m_position;
}

void MemoryFile::Seek(size_t offset, FileSeek from)
{
	ASSERT(IsOpen());
	if (from == FILESEEK_BEGINNING)
		m_position = offset;
	else if (from == FILESEEK_END)
		m_position = m_length - offset;
	else
		m_position += offset;
}

bool MemoryFile::AtEOF()
{
	ASSERT(IsOpen());
	return m_position >= m_length;
}

size_t MemoryFile::GetFileSize()
{
	ASSERT(IsOpen());
	return m_length;
}
