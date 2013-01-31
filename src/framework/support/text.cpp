#include "../debug.h"

#include "text.h"
#include "../file/file.h"
#include <string.h>

Text* Text::CreateFrom(File *file)
{
	STACK_TRACE;
	ASSERT(file != NULL);
	ASSERT(file->IsOpen());

	size_t length = file->GetFileSize();
	
	// we add 1 to the length so that we can ensure a NULL terminator is at the end
	size_t allocSize = length + 1;

	int8_t *buffer = new int8_t[allocSize];

	size_t bytesRead = file->Read(buffer, length);
	ASSERT(bytesRead == length);

	// put a NULL terminator at the end first
	buffer[allocSize - 1] = '\0';

	Text *result = new Text((char*)buffer, allocSize);
	ASSERT(result != NULL);

	return result;
}

Text* Text::CreateFrom(const char *text, size_t length)
{
	STACK_TRACE;
	ASSERT(text != NULL);
	ASSERT(length > 0);

	size_t allocSize = length;
	// if the source string does not have a NULL terminator at the end, we should add one
	if (text[allocSize - 1] != '\0')
		++allocSize;

	char *ownText = new char[allocSize];
	memcpy(ownText, text, length);

	if (allocSize > length)
		ownText[allocSize - 1] = '\0';

	Text *result = new Text(ownText, allocSize);
	ASSERT(result != NULL);

	return result;
}

Text::Text(char *text, size_t length)
{
	STACK_TRACE;
	ASSERT(text != NULL);
	ASSERT(length > 0);
	m_text = text;
	m_length = length;
	m_numLines = CountLines();
}

Text::~Text()
{
	STACK_TRACE;
	SAFE_DELETE(m_text);
}

uint32_t Text::CountLines()
{
	STACK_TRACE;
	uint32_t numLines = 1;
	for (uint32_t i = 0; i < m_length; ++i)
	{
		char c = m_text[i];
		if (c == '\n')
			++numLines;
	}

	return numLines;
}

const char* Text::GetLine(uint32_t line, size_t &lineLength) const
{
	STACK_TRACE;
	ASSERT(line < m_numLines);

	// find the start of the requested line
	uint32_t currentLine = 1;
	const char* startOfLine = &m_text[0];

	uint32_t i = 0;
	while (line != currentLine && i < m_length)
	{
		char c = m_text[i];
		++i;

		if (c == '\n')
		{
			// only track the start of new lines...
			++currentLine;
			startOfLine = &m_text[i];
		}
	}

	// now count up the characters to the end of this line
	lineLength = 0;
	while (i < m_length)
	{
		char c = m_text[i];
		++i;

		if (c == '\n')
			break;   // end of line, don't count newline character in length

		++lineLength;
	}

	// on windows, newlines are "\r\n". we don't include '\n', so we shouldn't
	// include '\r' either. right now, if on windows, '\r' will be at the end
	// of the line if we just broke out of the previous loop upon finding a '\n'
	if (*(startOfLine + (lineLength - 1)) == '\r')
		--lineLength;

	return startOfLine;
}