
//Window
// File: window.h
// Last edit: 04/01/2018 01:57 (UTC-3)
// Author: CGR

#ifndef __WINDOW_H__
#define __WINDOW_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

namespace uilib {

	class Window;
	class MenuBar;
	class StatusBar;

	typedef void(*WindowFunc)(Window& window);

	class Window : public Control
	{
	protected:
		string m_title;

		MenuBar *m_menuBar;
		StatusBar *m_statusBar;

		u32 m_hScrollPos;
		u32 m_vScrollPos;
		bool m_hScrollbarVisible;
		bool m_vScrollbarVisible;

		u32 m_currentKey;

		WindowFunc m_onMove;
		WindowFunc m_onSize;
		WindowFunc m_onClose;
		WindowFunc m_onHScroll;
		WindowFunc m_onVScroll;
		WindowFunc m_onKeyDown;
		WindowFunc m_onKeyUp;


	public:
		friend class OSDWindow;
		Window();
		~Window();

		Size getAutoSize();

		void setTitle(const string &text);

		void setFocus();

		MenuBar *useMenuBar();
		StatusBar *useStatusBar();
		void setMenuBarVisible(bool visible);
		void setStatusBarVisible(bool visible);

		void setScrollLength(const Size &contentSize, const Size &workingSize);
		void setAutoScrollLength(const Size &contentSize);

		void setParam(void *param);

		void setOnMove(WindowFunc onMove);
		void setOnSize(WindowFunc onSize);
		void setOnHScroll(WindowFunc onHScroll);
		void setOnVScroll(WindowFunc onVScroll);
		void setOnKeyDown(WindowFunc onKeyDown);
		void setOnKeyUp(WindowFunc onKeyUp);

		const string& title() const { return m_title; }
		Point position();
		bool visible() const { return m_visible; }
		Font &font() { return m_font; }
		MenuBar *menuBar() const { return m_menuBar; }
		StatusBar *statusBar() const { return m_statusBar; }
		bool menuBarVisible() const;
		bool statusBarVisible() const;

		u32 hScrollPos() const { return m_hScrollPos; }
		u32 vScrollPos() const { return m_vScrollPos; }
		u32 currentKey() const { return m_currentKey; }

		u32 scrollBarWidth(u32 which);
		u32 scrollBarPos(u32 which);
		bool scrollBarVisible(u32 which);

		OSDWindow& osdRef() const { return reinterpret_cast<OSDWindow&>(Control::osdRef()); }
	};

}

#endif

