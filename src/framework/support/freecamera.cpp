#include "freecamera.h"

#include "../basegameapp.h"
#include "../input/keyboard.h"
#include "../input/mouse.h"
#include "../input/touchscreen.h"
#include "../math/mathhelpers.h"
#include "../math/vector3.h"

FreeCamera::FreeCamera(ViewContext *viewContext, BaseGameApp *gameApp)
	: Camera(viewContext)
{
	m_gameApp = gameApp;
	m_movement = ZERO_VECTOR;
	m_forward = ZERO_VECTOR;
}

FreeCamera::~FreeCamera()
{
}

void FreeCamera::Move(float x, float y, float z)
{
	GetPosition().x += x;
	GetPosition().y += y;
	GetPosition().z += z;
}

void FreeCamera::Orient(float x, float y, float z)
{
	GetOrientation().x += x;
	GetOrientation().y += y;
	GetOrientation().z += z;
}

void FreeCamera::OnUpdate(float delta)
{
#ifdef MOBILE
	if (m_gameApp->GetTouchscreen()->IsTouching())
	{
		const TouchPointer *p = m_gameApp->GetTouchscreen()->GetPrimaryPointer();
		GetOrientation().y += p->GetDeltaX() * 0.01f;
		GetOrientation().x += p->GetDeltaY() * 0.01f;

		GetOrientation().y = RolloverClamp(GetOrientation().y, RADIANS_0, RADIANS_360);
		GetOrientation().x = RolloverClamp(GetOrientation().x, RADIANS_0, RADIANS_360);
	}

	m_movement = ZERO_VECTOR;
#else
	if (m_gameApp->GetMouse()->IsDown(MOUSE_LEFT))
	{
		GetOrientation().y += m_gameApp->GetMouse()->GetDeltaX() * 0.01f;
		GetOrientation().x += m_gameApp->GetMouse()->GetDeltaY() * 0.01f;

		GetOrientation().y = RolloverClamp(GetOrientation().y, RADIANS_0, RADIANS_360);
		GetOrientation().x = RolloverClamp(GetOrientation().x, RADIANS_0, RADIANS_360);
	}

	m_movement = ZERO_VECTOR;

	if (m_gameApp->GetKeyboard()->IsDown(KSYM_W))
		m_movement.z -= delta * 6.0f;
	if (m_gameApp->GetKeyboard()->IsDown(KSYM_S))
		m_movement.z += delta * 6.0f;
	if (m_gameApp->GetKeyboard()->IsDown(KSYM_A))
		m_movement.x -= delta * 6.0f;
	if (m_gameApp->GetKeyboard()->IsDown(KSYM_D))
		m_movement.x += delta * 6.0f;
#endif

	UpdateLookAtMatrix(m_movement);
}
