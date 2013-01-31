#ifdef SDL
#include "../debug.h"
#include "../log.h"

#include "sdlfile.h"

SDLFile::SDLFile()
	: File()
{
	STACK_TRACE;
	m_fp = NULL;
	m_mode = 0;
	m_canRead = FALSE;
	m_canWrite = FALSE;
}

SDLFile::~SDLFile()
{
	STACK_TRACE;
	Close();
}

BOOL SDLFile::Open(const stl::string &filename, int mode)
{
	STACK_TRACE;
	ASSERT(IsOpen() == FALSE);
	m_filename = filename;

	char fopenMode[3] = { '\0', '\0', '\0' };
	if (mode & FILEMODE_READ)
	{
		fopenMode[0] = 'r';
		m_canRead = TRUE;
	}
	else if (mode & FILEMODE_WRITE)
	{
		fopenMode[0] = 'w';
		m_canWrite = TRUE;
	}
	else if (mode & FILEMODE_APPEND)
	{
		fopenMode[0] = 'a';
		m_canWrite = TRUE;
	}
	if (mode & FILEMODE_BINARY && fopenMode[0] != '\0')
		fopenMode[1] = 'b';

	if (fopenMode[0] == '\0')
	{
		ASSERT(!"Unrecognized mode.");
		return FALSE;
	}
	else
	{
		ASSERT(m_canRead == TRUE || m_canWrite == TRUE);

		m_fp = SDL_RWFromFile(filename.c_str(), fopenMode);
		if (m_fp)
		{
			LOG_INFO(LOGCAT_FILEIO, "Opened SDLFile \"%s\", mode = %s\n", filename.c_str(), fopenMode);
			m_mode = mode;
			return TRUE;
		}
		else
		{
			LOG_WARN(LOGCAT_FILEIO, "Failed to open SDLFile \"%s\", mode = %s\n", filename.c_str(), fopenMode);
			return FALSE;
		}
	}
}

void SDLFile::Close()
{
	STACK_TRACE;
	if (IsOpen())
	{
		LOG_INFO(LOGCAT_FILEIO, "Closed SDLFIle \"%s\"\n", m_filename.c_str());
		SDL_FreeRW(m_fp);
	}

	m_fp = NULL;
	m_mode = 0;
	m_canRead = FALSE;
	m_canWrite = FALSE;
	m_filename.clear();
}

int8_t SDLFile::ReadChar()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int8_t buffer;
	SDL_RWread(m_fp, &buffer, sizeof(int8_t), 1);
	return buffer;
}

uint8_t SDLFile::ReadUnsignedChar()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint8_t buffer;
	SDL_RWread(m_fp, &buffer, sizeof(uint8_t), 1);
	return buffer;
}

int16_t SDLFile::ReadShort()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int16_t buffer;
	SDL_RWread(m_fp, &buffer, sizeof(int16_t), 1);
	return buffer;
}

uint16_t SDLFile::ReadUnsignedShort()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint16_t buffer;
	SDL_RWread(m_fp, &buffer, sizeof(uint16_t), 1);
	return buffer;
}

int32_t SDLFile::ReadInt()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int32_t buffer;
	SDL_RWread(m_fp, &buffer, sizeof(int32_t), 1);
	return buffer;
}

uint32_t SDLFile::ReadUnsignedInt()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint32_t buffer;
	SDL_RWread(m_fp, &buffer, sizeof(uint32_t), 1);
	return buffer;
}

int64_t SDLFile::ReadLong()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	int64_t buffer;
	SDL_RWread(m_fp, &buffer, sizeof(int64_t), 1);
	return buffer;
}

uint64_t SDLFile::ReadUnsignedLong()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	uint64_t buffer;
	SDL_RWread(m_fp, &buffer, sizeof(uint64_t), 1);
	return buffer;
}

float SDLFile::ReadFloat()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	float buffer;
	SDL_RWread(m_fp, &buffer, sizeof(float), 1);
	return buffer;
}

double SDLFile::ReadDouble()
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	double buffer;
	SDL_RWread(m_fp, &buffer, sizeof(double), 1);
	return buffer;
}

size_t SDLFile::Read(int8_t *buffer, size_t bytesToRead)
{
	ASSERT(IsOpen());
	ASSERT(CanRead());
	ASSERT(buffer != NULL);
	ASSERT(bytesToRead > 0);
	size_t numRead = SDL_RWread(m_fp, buffer, bytesToRead, 1);
	return (numRead * bytesToRead);
}

size_t SDLFile::ReadString(stl::string &buffer)
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

size_t SDLFile::ReadFixedString(stl::string &buffer, size_t maxLength)
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

size_t SDLFile::WriteChar(int8_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = SDL_RWwrite(m_fp, &data, sizeof(int8_t), 1);
	return (numWritten * sizeof(int8_t));
}

size_t SDLFile::WriteUnsignedChar(uint8_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = SDL_RWwrite(m_fp, &data, sizeof(uint8_t), 1);
	return (numWritten * sizeof(uint8_t));
}

size_t SDLFile::WriteShort(int16_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = SDL_RWwrite(m_fp, &data, sizeof(int16_t), 1);
	return (numWritten * sizeof(int16_t));
}

size_t SDLFile::WriteUnsignedShort(uint16_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = SDL_RWwrite(m_fp, &data, sizeof(uint16_t), 1);
	return (numWritten * sizeof(uint16_t));
}

size_t SDLFile::WriteInt(int32_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = SDL_RWwrite(m_fp, &data, sizeof(int32_t), 1);
	return (numWritten * sizeof(int32_t));
}

size_t SDLFile::WriteUnsignedInt(uint32_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = SDL_RWwrite(m_fp, &data, sizeof(uint32_t), 1);
	return (numWritten * sizeof(uint32_t));
}

size_t SDLFile::WriteLong(int64_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = SDL_RWwrite(m_fp, &data, sizeof(int64_t), 1);
	return (numWritten * sizeof(int64_t));
}

size_t SDLFile::WriteUnsignedLong(uint64_t data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = SDL_RWwrite(m_fp, &data, sizeof(uint64_t), 1);
	return (numWritten * sizeof(uint64_t));
}

size_t SDLFile::WriteFloat(float data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = SDL_RWwrite(m_fp, &data, sizeof(float), 1);
	return (numWritten * sizeof(float));
}

size_t SDLFile::WriteDouble(double data)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	size_t numWritten = SDL_RWwrite(m_fp, &data, sizeof(double), 1);
	return (numWritten * sizeof(double));
}

size_t SDLFile::Write(int8_t *buffer, size_t bytesToWrite)
{
	ASSERT(IsOpen());
	ASSERT(CanWrite());
	ASSERT(buffer != NULL);
	ASSERT(bytesToWrite > 0);
	size_t numWritten = SDL_RWwrite(m_fp, buffer, bytesToWrite, 1);
	return (numWritten * bytesToWrite);
}

size_t SDLFile::Tell()
{
	ASSERT(IsOpen());
	return (size_t)SDL_RWtell(m_fp);
}

void SDLFile::Seek(size_t offset, FileSeek from)
{
	ASSERT(IsOpen());

	int origin = SEEK_CUR;
	if (from == FILESEEK_BEGINNING)
		origin = SEEK_SET;
	else if (from == FILESEEK_END)
		origin = SEEK_END;

	SDL_RWseek(m_fp, offset, origin);
}

BOOL SDLFile::AtEOF()
{
	ASSERT(IsOpen());
	size_t filesize = GetFileSize();
	size_t currentPos = Tell();
	if (filesize == currentPos)
		return TRUE;
	else
		return FALSE;
}

size_t SDLFile::GetFileSize()
{
	ASSERT(IsOpen());
	size_t currentPos = Tell();
	Seek(0, FILESEEK_END);
	size_t filesize = Tell();
	Seek(currentPos, FILESEEK_BEGINNING);
	return filesize;
}
#endif
