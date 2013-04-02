#ifdef __S3E__
#include "../debug.h"
#include "../log.h"

#include "marmaladetouchscreen.h"
#include "touchscreenlistener.h"
#include "../math/circle.h"
#include "../math/mathhelpers.h"

MarmaladeTouchPointer::MarmaladeTouchPointer()
{
	m_id = INVALID_TOUCH_POINTER;
	m_x = 0;
	m_y = 0;
	m_deltaX = 0;
	m_deltaY = 0;
	m_isTouching = FALSE;
}

MarmaladeTouchPointer::~MarmaladeTouchPointer()
{
}

void MarmaladeTouchPointer::ResetDeltas()
{
	m_deltaX = 0;
	m_deltaY = 0;
}

BOOL MarmaladeTouchPointer::IsTouchingWithinArea(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom) const
{
	if (m_isTouching && m_x >= left && m_y >= top && m_x <= right && m_y <= bottom)
		return TRUE;
	else
		return FALSE;
}

BOOL MarmaladeTouchPointer::IsTouchingWithinArea(const Rect &area) const
{
	if (m_isTouching && m_x >= area.left && m_y >= area.top && m_x <= area.right && m_y <= area.bottom)
		return TRUE;
	else
		return FALSE;
}

BOOL MarmaladeTouchPointer::IsTouchingWithinArea(uint16_t centerX, uint16_t centerY, uint16_t radius) const
{
	if (m_isTouching)
	{
		uint32_t squaredDistance = ((centerX - m_x) * (centerX - m_x)) + ((centerY - m_y) * (centerY - m_y));
		uint32_t squaredRadius = radius * radius;
		if (squaredDistance <= squaredRadius)
			return TRUE;
	}

	return FALSE;
}

BOOL MarmaladeTouchPointer::IsTouchingWithinArea(const Circle &area) const
{
	if (m_isTouching)
	{
		uint32_t squaredDistance = ((area.x - m_x) * (area.x - m_x)) + ((area.y - m_y) * (area.y - m_y));
		uint32_t squaredRadius = area.radius * area.radius;
		if (squaredDistance <= squaredRadius)
			return TRUE;
	}

	return FALSE;
}

void MarmaladeTouchPointer::OnDown(int32_t id, uint16_t x, uint16_t y)
{
	m_isTouching = TRUE;
	m_x = x;
	m_y = y;
	m_deltaX = 0;
	m_deltaY = 0;
	m_id = id;
}

void MarmaladeTouchPointer::OnMove(int32_t id, uint16_t x, uint16_t y)
{
	// calculate the amount moved since the last time first...
	m_deltaX = x - m_x;
	m_deltaY = y - m_y;

	// ... now we can set the new position
	m_x = x;
	m_y = y;
}

void MarmaladeTouchPointer::OnUp()
{
	m_id = INVALID_TOUCH_POINTER;
	m_x = 0;
	m_y = 0;
	m_deltaX = 0;
	m_deltaY = 0;
	m_isTouching = FALSE;
}

MarmaladeTouchscreen::MarmaladeTouchscreen(MarmaladeSystem *system, BOOL isMultitouchAvailable)
{
	m_system = system;

	m_isMultitouchAvailable = isMultitouchAvailable;
	if (m_isMultitouchAvailable)
		m_maxTouchPoints = S3E_POINTER_TOUCH_MAX;
	else
		m_maxTouchPoints = 1;

	m_pointers = new MarmaladeTouchPointer[m_maxTouchPoints];
	ASSERT(m_pointers != NULL);

	Reset();
}

MarmaladeTouchscreen::~MarmaladeTouchscreen()
{
	SAFE_DELETE_ARRAY(m_pointers);
}

void MarmaladeTouchscreen::ResetDeltas()
{
	for (uint32_t i = 0; i < m_maxTouchPoints; ++i)
	{
		if (m_pointers[i].GetId() != INVALID_TOUCH_POINTER)
			m_pointers[i].ResetDeltas();
	}
}

void MarmaladeTouchscreen::ResetViewBounds(const Rect &viewBounds)
{
	m_viewBounds = viewBounds;
}

BOOL MarmaladeTouchscreen::OnSingleTouchTapEvent(const s3ePointerEvent *eventArgs)
{
	ASSERT(m_maxTouchPoints == 1);
	return FALSE;
}

BOOL MarmaladeTouchscreen::OnSingleTouchMotionEvent(const s3ePointerMotionEvent *eventArgs)
{
	ASSERT(m_maxTouchPoints == 1);
	return FALSE;
}

BOOL MarmaladeTouchscreen::OnMultiTouchTapEvent(const s3ePointerTouchEvent *eventArgs)
{
	ASSERT(m_maxTouchPoints > 1);
	BOOL isDown = (BOOL)eventArgs->m_Pressed;
	if (isDown)
	{
		// this should only happen for new touch points?
		MarmaladeTouchPointer *pointer = GetPointerByIdOrFirstAvailable(eventArgs->m_TouchID);
		if (pointer->GetId() == INVALID_TOUCH_POINTER)
		{
			uint16_t x = Clamp(eventArgs->m_x, m_viewBounds.left, m_viewBounds.right) - m_viewBounds.left;
			uint16_t y = Clamp(eventArgs->m_y, m_viewBounds.top, m_viewBounds.bottom) - m_viewBounds.top;
			pointer->OnDown(eventArgs->m_TouchID, x, y);

			// if no other touch points are currently down, then this one is to be
			// considered the primary one
			BOOL isPrimary = FALSE;
			if (m_currentTouchPoints == 0)
			{
				m_primaryPointer = pointer;
				isPrimary = TRUE;
			}

			++m_currentTouchPoints;
			
			for (stl::set<TouchscreenListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
			{
				if ((*i)->OnTouchDown(pointer->GetId(), pointer->GetX(), pointer->GetY(), isPrimary))
					break;
			}
		}
		else
		{
			LOG_WARN("TOUCHSCREEN", "Received s3ePointerTouchEvent, m_Pressed=1 for m_TouchID=%d which is already in m_pointers.\n", eventArgs->m_TouchID);
		}
	}
	else
	{
		// this should only happen for existing touch points?
		MarmaladeTouchPointer *pointer = GetPointerById_internal(eventArgs->m_TouchID);
		if (pointer != NULL)
		{
			pointer->OnUp();

			// if this pointer was the primary one, we need to switch the primary
			// to one of the other currently down pointers
			BOOL wasPrimary = FALSE;
			if (m_primaryPointer == pointer)
			{
				m_primaryPointer = GetNextDownPointer();
				wasPrimary = TRUE;
			}

			--m_currentTouchPoints;

			for (stl::set<TouchscreenListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
			{
				if ((*i)->OnTouchUp(eventArgs->m_TouchID, wasPrimary))
					break;
			}
		}
		else
		{
			// Not sure what to do here? this does seem to happen sometimes, at
			// least when using the simulator
			LOG_WARN("TOUCHSCREEN", "Received s3ePointerTouchEvent, m_Pressed=0 for m_TouchID=%d which is not currently in m_pointers.\n", eventArgs->m_TouchID);
		}
	}

	return TRUE;
}

BOOL MarmaladeTouchscreen::OnMultiTouchMotionEvent(const s3ePointerTouchMotionEvent *eventArgs)
{
	ASSERT(m_maxTouchPoints > 1);
	MarmaladeTouchPointer *pointer = GetPointerById_internal(eventArgs->m_TouchID);
	if (pointer != NULL)
	{
		uint16_t x = Clamp(eventArgs->m_x, m_viewBounds.left, m_viewBounds.right) - m_viewBounds.left;
		uint16_t y = Clamp(eventArgs->m_y, m_viewBounds.top, m_viewBounds.bottom) - m_viewBounds.top;
		pointer->OnMove(eventArgs->m_TouchID, x, y);
		
		BOOL isPrimary = FALSE;
		if (m_primaryPointer == pointer)
			isPrimary = TRUE;

		for (stl::set<TouchscreenListener*>::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i)
		{
			if ((*i)->OnTouchMove(pointer->GetId(), pointer->GetX(), pointer->GetY(), pointer->GetDeltaX(), pointer->GetDeltaY(), isPrimary))
				break;
		}
	}
	else
	{
		// Not sure what to do here? this does seem to happen sometimes, at
		// least when using the simulator
		LOG_WARN("TOUCHSCREEN", "Received s3ePointerTouchMotionEvent, m_TouchID=%d which is not currently in m_pointers.\n", eventArgs->m_TouchID);
	}

	return TRUE;
}

BOOL MarmaladeTouchscreen::WasTapped()
{
	if (m_isTouching && !m_isLocked)
	{
		m_isLocked = TRUE;
	}
	else if (!m_isTouching && m_isLocked)
	{
		m_isLocked = FALSE;
		return TRUE;
	}

	return FALSE;
}

const TouchPointer* MarmaladeTouchscreen::GetPointerById(int32_t id) const
{
	for (uint32_t i = 0; i < m_maxTouchPoints; ++i)
	{
		if (m_pointers[i].GetId() == id)
			return (TouchPointer*)&m_pointers[i];
	}

	return NULL;
}

MarmaladeTouchPointer* MarmaladeTouchscreen::GetPointerById_internal(int32_t id)
{
	for (uint32_t i = 0; i < m_maxTouchPoints; ++i)
	{
		if (m_pointers[i].GetId() == id)
			return &m_pointers[i];
	}

	return NULL;
}

MarmaladeTouchPointer* MarmaladeTouchscreen::GetFirstAvailablePointer()
{
	for (uint32_t i = 0; i < m_maxTouchPoints; ++i)
	{
		if (m_pointers[i].GetId() == INVALID_TOUCH_POINTER)
			return &m_pointers[i];
	}

	return NULL;
}

MarmaladeTouchPointer* MarmaladeTouchscreen::GetPointerByIdOrFirstAvailable(int32_t id)
{
	MarmaladeTouchPointer *result = GetPointerById_internal(id);
	if (result == NULL)
		result = GetFirstAvailablePointer();

	return result;
}

MarmaladeTouchPointer* MarmaladeTouchscreen::GetNextDownPointer()
{
	for (uint32_t i = 0; i < m_maxTouchPoints; ++i)
	{
		if (m_pointers[i].IsTouching())
			return &m_pointers[i];
	}

	// default to the first pointer. we don't want the primary pointer to
	// ever be NULL. it is up to the calling code to check the primary
	// pointer's IsTouching() state
	return &m_pointers[0];
}

void MarmaladeTouchscreen::Reset()
{
	MarmaladeTouchPointer *oldPrimaryPointer = m_primaryPointer;

	m_primaryPointer = &m_pointers[0];
	m_isTouching = FALSE;
	m_isLocked = FALSE;
	m_currentTouchPoints = 0;
	
	for (uint32_t i = 0; i < m_maxTouchPoints; ++i)
	{
		BOOL isPrimary = FALSE;
		if (oldPrimaryPointer == &m_pointers[i])
			isPrimary = TRUE;
		uint32_t id = m_pointers[i].GetId();
		
		m_pointers[i].OnUp();

		for (stl::set<TouchscreenListener*>::iterator j = m_listeners.begin(); j != m_listeners.end(); ++j)
		{
			if ((*j)->OnTouchUp(id, isPrimary))
				break;
		}
	}
}

void MarmaladeTouchscreen::RegisterListener(TouchscreenListener *listener)
{
	m_listeners.insert(listener);
}

void MarmaladeTouchscreen::UnregisterListener(TouchscreenListener *listener)
{
	m_listeners.erase(listener);
}

#endif
