#ifdef __S3E__
#ifndef __FRAMEWORK_INPUT_MARMALADETOUCHSCREEN_H_INCLUDED__
#define __FRAMEWORK_INPUT_MARMALADETOUCHSCREEN_H_INCLUDED__

#include "../common.h"
#include "touchscreen.h"
#include "../math/rect.h"
#include "s3ePointer.h"
#include <stl/set.h>

class MarmaladeSystem;
class TouchscreenListener;
struct Circle;

class MarmaladeTouchPointer : public TouchPointer
{
public:
	MarmaladeTouchPointer();
	virtual ~MarmaladeTouchPointer();

	void ResetDeltas();

	int32_t GetId() const                                  { return m_id; }
	uint16_t GetX() const                                  { return m_x; }
	uint16_t GetY() const                                  { return m_y; }
	int16_t GetDeltaX() const                              { return m_deltaX; }
	int16_t GetDeltaY() const                              { return m_deltaY; }
	BOOL IsTouching() const                                { return m_isTouching; }

	BOOL IsTouchingWithinArea(uint16_t left, uint16_t top, uint16_t right, uint16_t bottom) const;
	BOOL IsTouchingWithinArea(const Rect &area) const;
	BOOL IsTouchingWithinArea(uint16_t centerX, uint16_t centerY, uint16_t radius) const;
	BOOL IsTouchingWithinArea(const Circle &area) const;

	void OnDown(int32_t id, uint16_t x, uint16_t y);
	void OnMove(int32_t id, uint16_t x, uint16_t y);
	void OnUp();

private:
	int32_t m_id;
	uint16_t m_x;
	uint16_t m_y;
	int16_t m_deltaX;
	int16_t m_deltaY;
	BOOL m_isTouching;
};

class MarmaladeTouchscreen : public Touchscreen
{
public:
	MarmaladeTouchscreen(MarmaladeSystem *system, BOOL isMultitouchAvailable);
	virtual ~MarmaladeTouchscreen();

	void ResetDeltas();
	void ResetViewBounds(const Rect &viewBounds);

	BOOL OnSingleTouchTapEvent(const s3ePointerEvent *eventArgs);
	BOOL OnSingleTouchMotionEvent(const s3ePointerMotionEvent *eventArgs);
	BOOL OnMultiTouchTapEvent(const s3ePointerTouchEvent *eventArgs);
	BOOL OnMultiTouchMotionEvent(const s3ePointerTouchMotionEvent *eventArgs);

	BOOL IsMultitouchAvailable() const                     { return m_isMultitouchAvailable; }
	uint32_t GetPointerCount() const                       { return m_maxTouchPoints; }
	uint32_t GetCurrentPointerCount() const                { return m_currentTouchPoints; }
	BOOL IsTouching() const                                { return m_isTouching; }
	BOOL WasTapped();
	const TouchPointer* GetPrimaryPointer() const          { return (TouchPointer*)m_primaryPointer; }
	const TouchPointer* GetPointer(uint32_t index) const   { return (TouchPointer*)&m_pointers[index]; }
	const TouchPointer* GetPointerById(int32_t id) const;

	void Reset();

	void RegisterListener(TouchscreenListener *listener);
	void UnregisterListener(TouchscreenListener *listener);

private:
	MarmaladeTouchPointer* GetPointerById_internal(int32_t id);

	MarmaladeTouchPointer* GetFirstAvailablePointer();
	MarmaladeTouchPointer* GetPointerByIdOrFirstAvailable(int32_t id);
	MarmaladeTouchPointer* GetNextDownPointer();

	Rect m_viewBounds;
	stl::set<TouchscreenListener*> m_listeners;
	MarmaladeTouchPointer *m_pointers;
	MarmaladeTouchPointer *m_primaryPointer;
	BOOL m_isTouching;
	BOOL m_isLocked;

	MarmaladeSystem *m_system;
	BOOL m_isMultitouchAvailable;
	uint32_t m_maxTouchPoints;
	uint32_t m_currentTouchPoints;
};

#endif
#endif
