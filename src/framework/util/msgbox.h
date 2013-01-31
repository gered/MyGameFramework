#ifndef __FRAMEWORK_UTIL_MSGBOX_H_INCLUDED__
#define __FRAMEWORK_UTIL_MSGBOX_H_INCLUDED__

#include <stl/string.h>

enum MSGBOX_TYPE
{
	MSGBOX_TYPE_PLAIN,
	MSGBOX_TYPE_PROMPT,
	MSGBOX_TYPE_WARNING,
	MSGBOX_TYPE_ERROR
};

enum MSGBOX_RESULT
{
	MSGBOX_RESULT_UNKNOWN,
	MSGBOX_RESULT_OK,
	MSGBOX_RESULT_CANCEL,
	MSGBOX_RESULT_YES,
	MSGBOX_RESULT_NO,
	MSGBOX_RESULT_ABORT,
	MSGBOX_RESULT_RETRY,
	MSGBOX_RESULT_IGNORE
};

MSGBOX_RESULT MsgBox_Ok(MSGBOX_TYPE type, const stl::string& message, const stl::string& caption);
MSGBOX_RESULT MsgBox_OkCancel(MSGBOX_TYPE type, const stl::string& message, const stl::string& caption);
MSGBOX_RESULT MsgBox_YesNo(MSGBOX_TYPE type, const stl::string& message, const stl::string& caption);
MSGBOX_RESULT MsgBox_AbortRetryIgnore(MSGBOX_TYPE type, const stl::string& message, const stl::string& caption);

#endif
