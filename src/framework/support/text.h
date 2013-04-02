#ifndef __FRAMEWORK_SUPPORT_TEXT_H_INCLUDED__
#define __FRAMEWORK_SUPPORT_TEXT_H_INCLUDED__

#include "../common.h"
#include "../content/content.h"

class File;

/**
 * Represents one or more lines of text.
 */
class Text : public Content
{
public:
	static CONTENT_TYPE GetType()
	{
		static CONTENT_TYPE typeName = "Text";
		return typeName;
	}
	CONTENT_TYPE GetTypeOf() const                         { return GetType(); }

	/**
	 * Creates a text object from a text file.
	 * @param file the file to load as text
	 * @return a text object or NULL on failure
	 */
	static Text* CreateFrom(File *file);

	/**
	 * Creates a text object from an arbitrary string of text.
	 * @param text the source text to copy into this text object
	 * @param length the number of characters in the source text
	 * @return a text object or NULL on failure
	 */
	static Text* CreateFrom(const char *text, size_t length);

	virtual ~Text();

	/**
	 * @return the text contained in this object
	 */
	const char* GetText() const                            { return m_text; }

	/**
	 * Gets a string for the specified line of text from this object.
	 * @param line the line to get a string for (line 1 is the first line)
	 * @param lineLength will contain the number of characters in the
	 *                   retrieved line, not including new-line characters
	 * @return pointer to the text starting at the specified line
	 */
	const char* GetLine(uint line, size_t &lineLength) const;

	/**
	 * @return the total number of characters in this text object
	 */
	size_t GetLength() const                               { return m_length; }

	/**
	 * @return the number of lines of text in this text object
	 */
	uint GetNumLines() const                               { return m_numLines; }

private:
	Text(char *text, size_t length);

	uint CountLines();

	char *m_text;
	size_t m_length;
	uint m_numLines;
};

#endif
