
//Window
// File: window.cpp
// Last edit: 30/06/2017 02:54 (UTC-3)
// Author: CGR

#include "uibase.h"

namespace uilib {

	Window::Window() : Control(*new OSDWindow(*this))
	{
		m_menuBar = NULL;
		m_statusBar = NULL;
		m_visible = false;
		m_hScrollPos = 0;
		m_vScrollPos = 0;
		m_hScrollbarVisible = false;
		m_vScrollbarVisible = false;
		m_param = NULL;
		m_onMove = NULL;
		m_onSize = NULL;
		m_onClose = NULL;
		m_onHScroll = NULL;
		m_onVScroll = NULL;
		m_onKeyDown = NULL;
		m_onKeyUp = NULL;

		m_style = CS_Window | CS_Caption | CS_SysMenu | CS_MinimizeButton | CS_MaximizeButton;

		//
		osdRef().create();
		setDefaultFont();
		setBackgroundColor(Color(255, 255, 255));
	}

	Window::~Window()
	{
		if (m_menuBar) {
			delete m_menuBar;
			m_menuBar = NULL;
		}
		if (m_statusBar) {
			delete m_statusBar;
			m_statusBar = NULL;
		}
	}

	void Window::setTitle(const string &text)
	{
		m_title = text;
		osdRef().setTitle(text);
	}

	Size Window::getAutoSize()
	{
		Size ret = Control::getAutoSize();

		//add the caption bar if any
		ret += size();
		ret -= getFrameSize();
		return ret;
	}

	void Window::setFocus()
	{
		osdRef().setFocus();
	}


	MenuBar* Window::useMenuBar()
	{
		if (m_menuBar)
			return m_menuBar;

		m_menuBar = new MenuBar;
		m_menuBar->setParent(*this);
		return m_menuBar;
	}

	StatusBar* Window::useStatusBar()
	{
		if (m_statusBar)
			return m_statusBar;

		m_statusBar = new StatusBar;
		append(*m_statusBar);
		return m_statusBar;
	}


	void Window::setMenuBarVisible(bool visible)
	{
		if (m_menuBar) m_menuBar->setVisible(visible);
	}

	void Window::setStatusBarVisible(bool visible)
	{
		if (m_statusBar) m_statusBar->setVisible(visible);
	}

	void Window::setScrollLength(const Size &contentSize, const Size &workingSize)
	{
		osdRef().setScrollLength(contentSize, workingSize);
	}

	void Window::setAutoScrollLength(const Size &contentSize)
	{
		Size viewSize = getFrameSize();

		//adjust the view size in case the scrollbar will be shown
		if (contentSize.width() > viewSize.width()) {
			//adjust size only if scrollbar was not visible previously
			if (!scrollBarVisible(0))
				viewSize.setHeight(viewSize.height() - scrollBarWidth(0));

			//as workingSize changed, recheck if contentSize is still smaller than workingSize
			if (contentSize.height() > viewSize.height()) {
				//adjust size only if scrollbar was not visible previously
				if (!scrollBarVisible(1))
					viewSize.setWidth(viewSize.width() - scrollBarWidth(1));
			}
		}
		else if (contentSize.height() > viewSize.height()) {
			//adjust size only if scrollbar was not visible previously
			if (!scrollBarVisible(1))
				viewSize.setWidth(viewSize.width() - scrollBarWidth(1));

			//as viewSize changed, recheck if contentSize is still smaller than viewSize
			if (contentSize.width() > viewSize.width()) {
				//adjust size only if scrollbar was not visible previously
				if (!scrollBarVisible(0))
					viewSize.setHeight(viewSize.height() - scrollBarWidth(0));
			}
		}

		setScrollLength(contentSize, viewSize);
	}


	void Window::setParam(void *param)
	{
		m_param = param;
	}



	void Window::setOnMove(WindowFunc onMove) { m_onMove = onMove; }
	void Window::setOnSize(WindowFunc onSize) { m_onSize = onSize; }
	void Window::setOnHScroll(WindowFunc onHScroll) { m_onHScroll = onHScroll; }
	void Window::setOnVScroll(WindowFunc onVScroll) { m_onVScroll = onVScroll; }
	void Window::setOnKeyDown(WindowFunc onKeyDown) { m_onKeyDown = onKeyDown; }
	void Window::setOnKeyUp(WindowFunc onKeyUp) { m_onKeyUp = onKeyUp; }




	Point Window::position()
	{
		return m_position;
	}

	bool Window::menuBarVisible() const
	{
		return m_menuBar ? m_menuBar->visible() : false;
	}

	bool Window::statusBarVisible() const
	{
		return m_statusBar ? m_statusBar->visible() : false;
	}

	u32 Window::scrollBarWidth(u32 which)
	{
		return osdRef().getScrollBarWidth(which);
	}

	u32 Window::scrollBarPos(u32 which)
	{
		if (which == 0)
			return m_hScrollPos;

		return m_vScrollPos;
	}

	bool Window::scrollBarVisible(u32 which)
	{
		if (which == 0)
			return m_hScrollbarVisible;

		return m_vScrollbarVisible;
	}

}