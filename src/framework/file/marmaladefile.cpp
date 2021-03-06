#ifdef __S3E__
#include "../debug.h"
#include "../log.h"

#include "marmaladefile.h"
#include "s3eFile.h"

MarmaladeFile::MarmaladeFile()
{
	m_fp = NULL;
	m_mode = 0;
	m_canRead = false;
	m_canWrite = false;
}

MarmaladeFile::~MarmaladeFile()
{
	Close();
}

bool MarmaladeFile::Open(const stl::string &filename, int mode)
{
	ASSERT(IsOpen() == false);
	m_filename = filename;

	char fopenMode[3] = { '\0', '\0', '\0' };
	if (mode & FILEMODE_READ)
	{
		fopenMode[0] = 'r';
		m_canRead = true;
	}
	else if (mode & FILEMODE_WRITE)
	{
		fopenMode[0] = 'w';
		m_canWrite = true;
	}
	else if (mode & FILEMODE_APPEND)
	{
		fopenMode[0] = 'a';
		m_canWrite = true;
	}
	if (mode & FILEMODE_BINARY && fopenMode[0] != '\0')
		fopenMode[1] = 'b';

	if (fopenMode[0] == '\0')
	{
		ASSERT(!"Unrecognized mode.");
		return false;
	}
	else
	{
		ASSERT(m_canRead == true || m_canWrite == true);

		m_fp = s3eFileOpen(filename.c_str(), fopenMode);
		if (m_fp)
		{
			LOG_INFO(LOGCAT_FILEIO, "Opened MarmaladeFile \"%s\", mode = %s\n", filename.c_str(), fopenMode);
			m_mode = mode;
			return true;
		}
		else
		{
			LOG_WARN(LOGCAT_FILEIO, "Failed to open MarmaladeFile \"%s\", mode = %s. Error: %d, %s\n", filename.c_str(), fopenMode, s3eFileGetError(), s3eFileGetErrorString());
			return false;
		}
	}
}

void MarmaladeFile::Close()
{
	if (IsOpen())
	{
		LOG_INFO(LOGCAT_FILEIO, "Closed MarmaladeFile \"%s\"\n", m_filename.c_str());
		s3eFileClose(m_fp);
	}

	m_fp = NULL;
	m_mode = 0;
	m_canRead = false;
	m_canWrite = false;
	m_filename.clear();
}

int8_t MarmaladeFile::ReadChar()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int8_t buffer;
	s3eFileRead(&buffer, sizeof(int8_t), 1, m_fp);
	return buffer;
}

uint8_t MarmaladeFile::ReadUnsignedChar()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint8_t buffer;
	s3eFileRead(&buffer, sizeof(uint8_t), 1, m_fp);
	return buffer;
}

int16_t MarmaladeFile::ReadShort()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int16_t buffer;
	s3eFileRead(&buffer, sizeof(int16_t), 1, m_fp);
	return buffer;
}

uint16_t MarmaladeFile::ReadUnsignedShort()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint16_t buffer;
	s3eFileRead(&buffer, sizeof(uint16_t), 1, m_fp);
	return buffer;
}

int32_t MarmaladeFile::ReadInt()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int32_t buffer;
	s3eFileRead(&buffer, sizeof(int32_t), 1, m_fp);
	return buffer;
}

uint32_t MarmaladeFile::ReadUnsignedInt()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint32_t buffer;
	s3eFileRead(&buffer, sizeof(uint32_t), 1, m_fp);
	return buffer;
}

int64_t MarmaladeFile::ReadLong()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int64_t buffer;
	s3eFileRead(&buffer, sizeof(int64_t), 1, m_fp);
	return buffer;
}

uint64_t MarmaladeFile::ReadUnsignedLong()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint64_t buffer;
	s3eFileRead(&buffer, sizeof(uint64_t), 1, m_fp);
	return buffer;
}

float MarmaladeFile::ReadFloat()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	float buffer;
	s3eFileRead(&buffer, sizeof(float), 1, m_fp);
	return buffer;
}

double MarmaladeFile::ReadDouble()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	double buffer;
	s3eFileRead(&buffer, sizeof(double), 1, m_fp);
	return buffer;
}

size_t MarmaladeFile::Read(int8_t *buffer, size_t bytesToRead)
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	ASSERT(buffer != NULL);
	ASSERT(bytesToRead > 0);
	size_t numRead = s3eFileRead(buffer, bytesToRead, 1, m_fp);
	return (numRead * bytesToRead);
}

size_t MarmaladeFile::ReadString(stl::string &buffer)
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

size_t MarmaladeFile::ReadFixedString(stl::string &buffer, size_t maxLength)
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

size_t MarmaladeFile::WriteChar(int8_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = s3eFileWrite(&data, sizeof(int8_t), 1, m_fp);
	return (numWritten * sizeof(int8_t));
}

size_t MarmaladeFile::WriteUnsignedChar(uint8_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = s3eFileWrite(&data, sizeof(uint8_t), 1, m_fp);
	return (numWritten * sizeof(uint8_t));
}

size_t MarmaladeFile::WriteShort(int16_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = s3eFileWrite(&data, sizeof(int16_t), 1, m_fp);
	return (numWritten * sizeof(int16_t));
}

size_t MarmaladeFile::WriteUnsignedShort(uint16_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = s3eFileWrite(&data, sizeof(uint16_t), 1, m_fp);
	return (numWritten * sizeof(uint16_t));
}

size_t MarmaladeFile::WriteInt(int32_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = s3eFileWrite(&data, sizeof(int32_t), 1, m_fp);
	return (numWritten * sizeof(int32_t));
}

size_t MarmaladeFile::WriteUnsignedInt(uint32_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = s3eFileWrite(&data, sizeof(uint32_t), 1, m_fp);
	return (numWritten * sizeof(uint32_t));
}

size_t MarmaladeFile::WriteLong(int64_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = s3eFileWrite(&data, sizeof(int64_t), 1, m_fp);
	return (numWritten * sizeof(int64_t));
}

size_t MarmaladeFile::WriteUnsignedLong(uint64_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = s3eFileWrite(&data, sizeof(uint64_t), 1, m_fp);
	return (numWritten * sizeof(uint64_t));
}

size_t MarmaladeFile::WriteFloat(float data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = s3eFileWrite(&data, sizeof(float), 1, m_fp);
	return (numWritten * sizeof(float));
}

size_t MarmaladeFile::WriteDouble(double data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = s3eFileWrite(&data, sizeof(double), 1, m_fp);
	return (numWritten * sizeof(double));
}

size_t MarmaladeFile::Write(int8_t *buffer, size_t bytesToWrite)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	ASSERT(buffer != NULL);
	ASSERT(bytesToWrite > 0);
	size_t numWritten = s3eFileWrite(buffer, bytesToWrite, 1, m_fp);
	return (numWritten * bytesToWrite);
}

size_t MarmaladeFile::Tell()
{
	ASSERT(IsOpen());
	return (size_t)s3eFileTell(m_fp);
}

void MarmaladeFile::Seek(size_t offset, FileSeek from)
{
	ASSERT(IsOpen());

	s3eFileSeekOrigin origin = S3E_FILESEEK_CUR;
	if (from == FILESEEK_BEGINNING)
		origin = S3E_FILESEEK_SET;
	else if (from == FILESEEK_END)
		origin = S3E_FILESEEK_END;

	s3eFileSeek(m_fp, offset, origin);
}

bool MarmaladeFile::AtEOF()
{
	ASSERT(IsOpen());
	if (s3eFileEOF(m_fp) == S3E_true)
		return true;
	else
		return false;
}

size_t MarmaladeFile::GetFileSize()
{
	ASSERT(IsOpen());
	return (size_t)s3eFileGetSize(m_fp);
}

#endif
