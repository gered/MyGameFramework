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

	int GetId() const                                      { return m_id; }
	uint GetX() const                                      { return m_x; }
	uint GetY() const                                      { return m_y; }
	int GetDeltaX() const                                  { return m_deltaX; }
	int GetDeltaY() const                                  { return m_deltaY; }
	BOOL IsTouching() const                                { return m_isTouching; }

	BOOL IsTouchingWithinArea(uint left, uint top, uint right, uint bottom) const;
	BOOL IsTouchingWithinArea(const Rect &area) const;
	BOOL IsTouchingWithinArea(uint centerX, uint centerY, uint radius) const;
	BOOL IsTouchingWithinArea(const Circle &area) const;

	void OnDown(int id, uint x, uint y);
	void OnMove(int id, uint x, uint y);
	void OnUp();

private:
	int m_id;
	uint m_x;
	uint m_y;
	int m_deltaX;
	int m_deltaY;
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
	uint GetPointerCount() const                           { return m_maxTouchPoints; }
	uint GetCurrentPointerCount() const                    { return m_currentTouchPoints; }
	BOOL IsTouching() const                                { return m_isTouching; }
	BOOL WasTapped();
	const TouchPointer* GetPrimaryPointer() const          { return (TouchPointer*)m_primaryPointer; }
	const TouchPointer* GetPointer(uint index) const   { return (TouchPointer*)&m_pointers[index]; }
	const TouchPointer* GetPointerById(int id) const;

	void Reset();

	void RegisterListener(TouchscreenListener *listener);
	void UnregisterListener(TouchscreenListener *listener);

private:
	MarmaladeTouchPointer* GetPointerById_internal(int id);

	MarmaladeTouchPointer* GetFirstAvailablePointer();
	MarmaladeTouchPointer* GetPointerByIdOrFirstAvailable(int id);
	MarmaladeTouchPointer* GetNextDownPointer();

	Rect m_viewBounds;
	stl::set<TouchscreenListener*> m_listeners;
	MarmaladeTouchPointer *m_pointers;
	MarmaladeTouchPointer *m_primaryPointer;
	BOOL m_isTouching;
	BOOL m_isLocked;

	MarmaladeSystem *m_system;
	BOOL m_isMultitouchAvailable;
	uint m_maxTouchPoints;
	uint m_currentTouchPoints;
};

#endif
#endif
