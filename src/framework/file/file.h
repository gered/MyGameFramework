#ifndef __FRAMEWORK_FILE_FILE_H_INCLUDED__
#define __FRAMEWORK_FILE_FILE_H_INCLUDED__

#include <stl/string.h>
#include "../common.h"

enum FileMode
{
	FILEMODE_READ = 1,
	FILEMODE_WRITE = 2,
	FILEMODE_BINARY = 4,
	FILEMODE_APPEND = 8,
	FILEMODE_MEMORY = 16
};

enum FileSeek
{
	FILESEEK_BEGINNING,
	FILESEEK_CURRENT,
	FILESEEK_END
};

enum FileType
{
	FILETYPE_IO,
	FILETYPE_MEMORY
};

/**
 * Represents a file that can be read from and/or written to.
 */
class File
{
public:
	File()                                                    {}
	virtual ~File()                                           {}

	/**
	 * Closes the file.
	 */
	virtual void Close() = 0;

	/**
	 * @return TRUE if the file is currently open
	 */
	virtual BOOL IsOpen() const = 0;

	/**
	 * @return TRUE if this file supports reading
	 */
	virtual BOOL CanRead() const = 0;

	/**
	 * @return TRUE if this file supports writing
	 */
	virtual BOOL CanWrite() const = 0;

	/**
	 * @return a type value representing the backing storage where this file's
	 *         data is located
	 */
	virtual FileType GetFileType() const = 0;

	/**
	 * Reads and returns the next signed 8-bit value from the file and then
	 * advances the current file pointer.
	 * @return the signed 8-bit value read from the file
	 */
	virtual int8_t ReadChar() = 0;

	/**
	 * Reads and returns the next unsigned 8-bit value from the file and then
	 * advances the current file pointer.
	 * @return the unsigned 8-bit value read from the file
	 */
	virtual uint8_t ReadUnsignedChar() = 0;

	/**
	 * Reads and returns the next signed 16-bit value from the file and then
	 * advances the current file pointer.
	 * @return the signed 16-bit value read from the file
	 */
	virtual int16_t ReadShort() = 0;

	/**
	 * Reads and returns the next unsigned 16-bit value from the file and then
	 * advances the current file pointer.
	 * @return the unsigned 16-bit value read from the file
	 */
	virtual uint16_t ReadUnsignedShort() = 0;

	/**
	 * Reads and returns the next signed 32-bit value from the file and then
	 * advances the current file pointer.
	 * @return the signed 32-bit value read from the file
	 */
	virtual int32_t ReadInt() = 0;

	/**
	 * Reads and returns the next unsigned 32-bit value from the file and then
	 * advances the current file pointer.
	 * @return the unsigned 32-bit value read from the file
	 */
	virtual uint32_t ReadUnsignedInt() = 0;

	/**
	 * Reads and returns the next signed 64-bit value from the file and then
	 * advances the current file pointer.
	 * @return the signed 64-bit value read from the file
	 */
	virtual int64_t ReadLong() = 0;

	/**
	 * Reads and returns the next unsigned 64-bit value from the file and then
	 * advances the current file pointer.
	 * @return the unsigned 64-bit value read from the file
	 */
	virtual uint64_t ReadUnsignedLong() = 0;

	/**
	 * Reads and returns the next 32-bit floating point value from the file 
	 * and then advances the current file pointer.
	 * @return the 32-bit floating point value read from the file
	 */
	virtual float ReadFloat() = 0;

	/**
	 * Reads and returns the next 64-bit floating point value from the file
	 * and then advances the current file pointer.
	 * @return the 64-bit floating point value read from the file
	 */
	virtual double ReadDouble() = 0;

	/**
	 * Reads the specified number of bytes and stores it in the provided buffer.
	 * @param buffer the pre-allocated buffer to store the read data in
	 * @param bytesToRead the number of bytes to read
	 * @return the number of bytes read
	 */
	virtual size_t Read(int8_t *buffer, size_t bytesToRead) = 0;

	/**
	 * Reads a null-terminated string from the file. Reading stops at the first
	 * NULL character encountered.
	 * @param buffer the string to store the read characters in
	 * @return the number of characters read, not including the NULL terminator
	 */
	virtual size_t ReadString(stl::string &buffer) = 0;

	/**
	 * Reads a null-terminated string from the file. Reading stops when either
	 * the maximum length specified has been reached or the first NULL character
	 * is encountered.
	 * @param buffer the string to store the read characters in
	 * @param maxLength the maximum number of characters to read
	 * @return the number of characters read, not including the NULL terminator
	 */
	virtual size_t ReadFixedString(stl::string &buffer, size_t maxLength) = 0;

	/**
	 * Writes a signed 8-bit value to the file.
	 * @param data the signed 8-bit value to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t WriteChar(int8_t data) = 0;

	/**
	 * Writes an unsigned 8-bit value to the file.
	 * @param data the unsigned 8-bit value to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t WriteUnsignedChar(uint8_t data) = 0;

	/**
	 * Writes a signed 16-bit value to the file.
	 * @param data the signed 16-bit value to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t WriteShort(int16_t data) = 0;

	/**
	 * Writes an unsigned 16-bit value to the file.
	 * @param data the unsigned 16-bit value to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t WriteUnsignedShort(uint16_t data) = 0;

	/**
	 * Writes a signed 32-bit value to the file.
	 * @param data the signed 32-bit value to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t WriteInt(int32_t data) = 0;

	/**
	 * Writes an unsigned 32-bit value to the file.
	 * @param data the unsigned 32-bit value to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t WriteUnsignedInt(uint32_t data) = 0;

	/**
	 * Writes a signed 64-bit value to the file.
	 * @param data the signed 64-bit value to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t WriteLong(int64_t data) = 0;

	/**
	 * Writes an unsigned 64-bit value to the file.
	 * @param data the unsigned 64-bit value to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t WriteUnsignedLong(uint64_t data) = 0;

	/**
	 * Writes a 32-bit floating point value to the file.
	 * @param data the 32-bit floating point value to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t WriteFloat(float data) = 0;

	/**
	 * Writes a 64-bit floating point value to the file.
	 * @param data the 64-bit floating point value to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t WriteDouble(double data) = 0;

	/**
	 * Writes a buffer of data to the file.
	 * @param buffer the buffer of data to write
	 * @param bytesToWrite the number of bytes from the buffer to write
	 * @return the number of bytes written to the file
	 */
	virtual size_t Write(int8_t *buffer, size_t bytesToWrite) = 0;

	/**
	 * @return the current file pointer position as an offset number of bytes
	 *         from the beginning of the file
	 */
	virtual size_t Tell() = 0;

	/**
	 * Moves the current file pointer by a given amount from the position
	 * specified.
	 * @param offset the number of bytes to move
	 * @param from the position to move relative to
	 */
	virtual void Seek(size_t offset, FileSeek from) = 0;

	/**
	 * @return TRUE if the current file pointer is at the end of the file
	 */
	virtual BOOL AtEOF() = 0;

	/**
	 * @return the size of this file in bytes
	 */
	virtual size_t GetFileSize() = 0;

	/**
	 * @return the path and filename of this file
	 */
	virtual const stl::string& GetFilename() const = 0;
};

#endif
