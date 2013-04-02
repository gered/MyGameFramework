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

			bool OnKeyDown(KEYS key);
			bool OnKeyUp(KEYS key);

			bool OnMouseButtonDown(MOUSE_BUTTONS button, uint x, uint y);
			bool OnMouseButtonUp(MOUSE_BUTTONS button, uint x, uint y);
			bool OnMouseMove(uint x, uint y, int deltaX, int deltaY);

			bool OnTouchDown(int id, uint x, uint y, bool isPrimary);
			bool OnTouchUp(int id, bool isPrimary);
			bool OnTouchMove(int id, uint x, uint y, int deltaX, int deltaY, bool isPrimary);

			bool IsEnabled() const                         { return m_enabled; }
			void Enable(bool enable);

		private:
			unsigned char ConvertToGwenKey(KEYS key) const;
			int ConvertToGwenButton(MOUSE_BUTTONS button) const;

			BaseGameApp *m_gameApp;
			Gwen::Controls::Canvas *m_canvas;
			bool m_enabled;
		};
	}
}

#endif

