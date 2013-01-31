#include "../common.h"

#include "msgbox.h"
#include <stl/string.h>

#ifdef DESKTOP
	#ifdef _WIN32
		#define MSGBOX_WINDOWS
	#elif defined __APPLE__
		#define MSGBOX_OSX
	#elif defined __linux__
		#define MSGBOX_LINUX
	#endif
#endif

enum MSGBOX_BUTTONS
{
	MSGBOX_BUTTON_OK = 1,
	MSGBOX_BUTTON_CANCEL = 2,
	MSGBOX_BUTTON_YES = 4,
	MSGBOX_BUTTON_NO = 8,
	MSGBOX_BUTTON_ABORT = 16,
	MSGBOX_BUTTON_RETRY = 32,
	MSGBOX_BUTTON_IGNORE = 64
};

uint32_t ConvertMsgBoxTypeToSysMsgBoxIcon(MSGBOX_TYPE type);

#ifdef MSGBOX_WINDOWS

	#define WIN32_LEAN_AND_MEAN
	#define WIN32_EXTRA_LEAN
	#include <windows.h>

	MSGBOX_RESULT SystemMessageBox(const stl::string& message, const stl::string& caption, uint32_t flags);

#elif defined MSGBOX_OSX

	#include <CoreFoundation/CoreFoundation.h>

	int SystemMessageBox(const stl::string& message, const stl::string& caption, CFOptionFlags flags, const char* firstButtonText, const char* secondButtonText = NULL, const char* thirdButtonText = NULL);

#elif defined MSGBOX_LINUX
	// TODO
#endif

MSGBOX_RESULT MsgBox_Ok(MSGBOX_TYPE type, const stl::string& message, const stl::string& caption)
{
	MSGBOX_RESULT result = MSGBOX_RESULT_UNKNOWN;
	uint32_t icon = ConvertMsgBoxTypeToSysMsgBoxIcon(type);

#ifdef MSGBOX_WINDOWS
	result = SystemMessageBox(message, caption, MB_OK | icon);
#elif defined MSGBOX_OSX
	int clicked = SystemMessageBox(message, caption, icon, "OK");
	if (clicked == 0)
		result = MSGBOX_RESULT_OK;
	else
		result = MSGBOX_RESULT_UNKNOWN;
#elif defined MSGBOX_LINUX
	// TODO
#endif
	return result;
}

MSGBOX_RESULT MsgBox_OkCancel(MSGBOX_TYPE type, const stl::string& message, const stl::string& caption)
{
	MSGBOX_RESULT result = MSGBOX_RESULT_UNKNOWN;
	uint32_t icon = ConvertMsgBoxTypeToSysMsgBoxIcon(type);

#ifdef MSGBOX_WINDOWS
	result = SystemMessageBox(message, caption, MB_OKCANCEL | icon);
#elif defined MSGBOX_OSX
	int clicked = SystemMessageBox(message, caption, icon, "OK", "Cancel");
	switch (clicked)
	{
		case 0:
			result = MSGBOX_RESULT_OK;
			break;
		case 1:
			result = MSGBOX_RESULT_CANCEL;
			break;
		default:
			result = MSGBOX_RESULT_UNKNOWN;
	}
#elif defined MSGBOX_LINUX
	// TODO
#endif
	return result;
}

MSGBOX_RESULT MsgBox_YesNo(MSGBOX_TYPE type, const stl::string& message, const stl::string& caption)
{
	MSGBOX_RESULT result = MSGBOX_RESULT_UNKNOWN;
	uint32_t icon = ConvertMsgBoxTypeToSysMsgBoxIcon(type);

#ifdef MSGBOX_WINDOWS
	result = SystemMessageBox(message, caption, MB_YESNO | icon);
#elif defined MSGBOX_OSX
	int clicked = SystemMessageBox(message, caption, icon, "Yes", "No");
	switch (clicked)
	{
		case 0:
			result = MSGBOX_RESULT_YES;
			break;
		case 1:
			result = MSGBOX_RESULT_NO;
			break;
		default:
			result = MSGBOX_RESULT_UNKNOWN;
	}
#elif defined MSGBOX_LINUX
	// TODO
#endif
	return result;
}

MSGBOX_RESULT MsgBox_AbortRetryIgnore(MSGBOX_TYPE type, const stl::string& message, const stl::string& caption)
{
	MSGBOX_RESULT result = MSGBOX_RESULT_UNKNOWN;
	uint32_t icon = ConvertMsgBoxTypeToSysMsgBoxIcon(type);

#ifdef MSGBOX_WINDOWS
	result = SystemMessageBox(message, caption, MB_ABORTRETRYIGNORE | icon);
#elif defined MSGBOX_OSX
	int clicked = SystemMessageBox(message, caption, icon, "Abort", "Retry", "Ignore");
	switch (clicked)
	{
		case 0:
			result = MSGBOX_RESULT_ABORT;
			break;
		case 1:
			result = MSGBOX_RESULT_RETRY;
			break;
		case 2:
			result = MSGBOX_RESULT_IGNORE;
			break;
		default:
			result = MSGBOX_RESULT_UNKNOWN;
	}
#elif defined MSGBOX_LINUX
	// TODO
#endif
	return result;
}

uint32_t ConvertMsgBoxTypeToSysMsgBoxIcon(MSGBOX_TYPE type)
{
	switch (type)
	{
		case MSGBOX_TYPE_PLAIN:
#ifdef MSGBOX_WINDOWS
			return 0;
#elif defined MSGBOX_OSX
			return kCFUserNotificationPlainAlertLevel;
#elif defined MSGBOX_LINUX
			// TODO
#else
			return 0;
#endif
			break;
		case MSGBOX_TYPE_PROMPT:
#ifdef MSGBOX_WINDOWS
			return MB_ICONQUESTION;
#elif defined MSGBOX_OSX
			return kCFUserNotificationNoteAlertLevel;
#elif defined MSGBOX_LINUX
			// TODO
#else
			return 0;
#endif
			break;
		case MSGBOX_TYPE_WARNING:
#ifdef MSGBOX_WINDOWS
			return MB_ICONEXCLAMATION;
#elif defined MSGBOX_OSX
			return kCFUserNotificationCautionAlertLevel;
#elif defined MSGBOX_LINUX
			// TODO
#else
			return 0;
#endif
			break;
		case MSGBOX_TYPE_ERROR:
#ifdef MSGBOX_WINDOWS
			return MB_ICONERROR;
#elif defined MSGBOX_OSX
			return kCFUserNotificationStopAlertLevel;
#elif defined MSGBOX_LINUX
			// TODO
#else
			return 0;
#endif
			break;
		default:
			return 0;
	}
}

#ifdef MSGBOX_WINDOWS
MSGBOX_RESULT SystemMessageBox(const stl::string& message, const stl::string& caption, uint32_t flags)
{
	int result = MessageBoxA(NULL, message.c_str(), caption.c_str(), flags);
	
	switch (result)
	{
		case IDOK:     return MSGBOX_RESULT_OK;
		case IDCANCEL: return MSGBOX_RESULT_CANCEL;
		case IDYES:    return MSGBOX_RESULT_YES;
		case IDNO:     return MSGBOX_RESULT_NO;
		case IDABORT:  return MSGBOX_RESULT_ABORT;
		case IDRETRY:  return MSGBOX_RESULT_RETRY;
		case IDIGNORE: return MSGBOX_RESULT_IGNORE;
	}
	
	return MSGBOX_RESULT_UNKNOWN;
}
#elif defined MSGBOX_OSX
int SystemMessageBox(const stl::string& message, const stl::string& caption, CFOptionFlags flags, const char* firstButtonText, const char* secondButtonText, const char* thirdButtonText)
{
	CFStringRef captionRef = CFStringCreateWithCString(NULL, caption.c_str(), kCFStringEncodingUTF8);
	CFStringRef messageRef = CFStringCreateWithCString(NULL, message.c_str(), kCFStringEncodingUTF8);
	
	CFStringRef firstButton = NULL;
	if (firstButtonText != NULL)
		firstButton = CFStringCreateWithCString(NULL, firstButtonText, strlen(firstButtonText));
	
	CFStringRef secondButton = NULL;
	if (secondButtonText != NULL)
		secondButton = CFStringCreateWithCString(NULL, secondButtonText, strlen(secondButtonText));

	CFStringRef thirdButton = NULL;
	if (thirdButtonText != NULL)
		thirdButton = CFStringCreateWithCString(NULL, thirdButtonText, strlen(thirdButtonText));

	CFOptionFlags result;
		
	CFUserNotificationDisplayAlert(
	                               0,
	                               flags,
	                               NULL,
	                               NULL,
	                               NULL,
	                               captionRef,
	                               messageRef,
	                               firstButton,
	                               secondButton,
	                               thirdButton,
	                               &result
	                               );
		
	CFRelease(captionRef);
	CFRelease(messageRef);
	
	if (firstButton != NULL)
		CFRelease(firstButton);
	if (secondButton != NULL)
		CFRelease(secondButton);
	if (thirdButton != NULL)
		CFRelease(thirdButton);

	switch (result)
	{
		case kCFUserNotificationDefaultResponse: return 0;
		case kCFUserNotificationAlternateResponse: return 1;
		case kCFUserNotificationOtherResponse: return 2;
	}
	
	return -1;
}
#elif defined MSGBOX_LINUX
	// TODO
#endif
