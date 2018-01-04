
//Event
// File: event.h
// Last edit: 20/10/2017 06:53 (UTC-3)
// Author: CGR

#ifndef __UILIB_EVENT_H__
#define __UILIB_EVENT_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

namespace uilib {

	class Event
	{
		bool m_accepted;
	public:
		Event() { m_accepted = true; }
		void accept() { m_accepted = true; }
		void ignore() { m_accepted = false; }
		bool accepted() const { return m_accepted; }
	};

	class PaintEvent : public Event
	{
	public:
		PaintEvent() { }
	};

	class KeyEvent : public Event
	{
		int m_key;
	public:
		KeyEvent(int key)
			: m_key(key) { }

		int key() const { return m_key; }
	};

	class MouseEvent : public Event
	{
		Point m_mousePos;
		u32 m_button;
		u32 m_buttonsState;

	public:
		MouseEvent(Point position, u32 button, u32 buttonsState)
			: m_button(button), m_buttonsState(buttonsState), m_mousePos(position) { }

		Point position() const { return m_mousePos; }
		u32 button() const { return m_button; }
		u32 buttonsState() const { return m_buttonsState; }
	};

	class ScrollEvent : public Event
	{
		Point m_position;

	public:
		ScrollEvent(Point position) { m_position = position; }
		Point position() const { return m_position; }
	};

}

#endif

