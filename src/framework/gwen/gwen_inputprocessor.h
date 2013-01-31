#ifndef __FRAMEWORK_GWEN_GWEN_INPUTPROCESSOR_H_INCLUDED__
#define __FRAMEWORK_GWEN_GWEN_INPUTPROCESSOR_H_INCLUDED__

#include "../common.h"
#include "../input/keyboardlistener.h"
#include "../input/mouselistener.h"
#include "../input/touchscreenlistener.h"
#include <controls/gwen_canvas.h>

class BaseGameApp;

namespace Gwen
{
	namespace Input
	{
		class InputProcessor : public KeyboardListener, public MouseListener, public TouchscreenListener
		{
		public:
			InputProcessor(BaseGameApp *gameApp, Gwen::Controls::Canvas *canvas);
			virtual ~InputProcessor();

			BOOL OnKeyDown(KEYS key);
			BOOL OnKeyUp(KEYS key);

			BOOL OnMouseButtonDown(MOUSE_BUTTONS button, uint16_t x, uint16_t y);
			BOOL OnMouseButtonUp(MOUSE_BUTTONS button, uint16_t x, uint16_t y);
			BOOL OnMouseMove(uint16_t x, uint16_t y, int16_t deltaX, int16_t deltaY);

			BOOL OnTouchDown(int32_t id, uint16_t x, uint16_t y, BOOL isPrimary);
			BOOL OnTouchUp(int32_t id, BOOL isPrimary);
			BOOL OnTouchMove(int32_t id, uint16_t x, uint16_t y, int16_t deltaX, int16_t deltaY, BOOL isPrimary);

			BOOL IsEnabled() const                         { return m_enabled; }
			void Enable(BOOL enable);

		private:
			unsigned char ConvertToGwenKey(KEYS key) const;
			int ConvertToGwenButton(MOUSE_BUTTONS button) const;

			BaseGameApp *m_gameApp;
			Gwen::Controls::Canvas *m_canvas;
			BOOL m_enabled;
		};
	}
}

#endif

