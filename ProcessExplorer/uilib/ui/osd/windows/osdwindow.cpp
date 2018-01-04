
//OSD Window
// File: osdwindow.cpp
// Last edit: 20/10/2017 06:53 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	TCHAR g_szWindowClassName[] = TEXT("uiwindow");

	static DWORD ControlStyleToWindowsWindowStyle(u32 cs)
	{
		DWORD style = 0;
		if (cs & CS_Caption)
			style |= WS_CAPTION;
		if (cs & CS_Resizable)
			style |= WS_THICKFRAME;
		if (cs & CS_SysMenu)
			style |= WS_SYSMENU;
		if (cs & CS_MinimizeButton)
			style |= WS_MINIMIZEBOX;
		if (cs & CS_MaximizeButton)
			style |= WS_MAXIMIZEBOX;

		return style;
	}



	void OSDWindow::createBackBuffer()
	{
		HWND hwnd = handle();

		if (m_backBufferBitmap)
			::DeleteObject(m_backBufferBitmap);

		BITMAP bmp;
		HDC hdc = ::GetDC(hwnd);
		m_backBufferDC = ::CreateCompatibleDC(hdc);

		RECT screenRect;
		::GetClientRect(hwnd, &screenRect);

		m_backBufferBitmap = ::CreateCompatibleBitmap(hdc, screenRect.right, screenRect.bottom);
		::SelectObject(m_backBufferDC, m_backBufferBitmap);

		::GetObject(m_backBufferBitmap, sizeof(BITMAP), &bmp);
		m_bmHeader = bmp;
		::ReleaseDC(hwnd, hdc);
	}

	void OSDWindow::flip(HDC hdcDest)
	{
		if (!m_backBufferDC)
			return;
		RECT rc = { 0,0, m_bmHeader.bmWidth, m_bmHeader.bmHeight };
		BOOL test = ::BitBlt(hdcDest, 0, 0, m_bmHeader.bmWidth, m_bmHeader.bmHeight, m_backBufferDC, 0, 0, SRCCOPY);
	}



	void OSDWindow::create()
	{
		DWORD winStyle = ControlStyleToWindowsWindowStyle(ref().style());
		createWindow(*this, g_szWindowClassName, 0, winStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
		//if caption style is set, window has some size already, get it
		RECT rc;
		::GetWindowRect(m_hwnd, &rc);
		Size size(rc.right - rc.left, rc.bottom - rc.top);
		ref().m_size = size;
		ref().m_curSize = size;
		createBackBuffer();
	}

	void OSDWindow::destroy()
	{
		if (m_backBufferBitmap) {
			::DeleteObject(m_backBufferBitmap);
			m_backBufferBitmap = NULL;
		}

		if (m_backBrush) {
			::DeleteObject(m_backBrush);
			m_backBrush = NULL;
		}

		if (m_hwnd) {
			::DestroyWindow(m_hwnd);
			m_hwnd = NULL;
		}

		if (ref().m_paintDevice) {
			delete ref().m_paintDevice;
			ref().m_paintDevice = NULL;
		}
	}

	void OSDWindow::update()
	{
		OSDControl::update();
		Window& ref = this->ref();
		setTitle(ref.title().data());
		setBackgroundColor(ref.backgroundColor());
	}

	OSDWindow::~OSDWindow()
	{
		destroy();
	}

	void OSDWindow::setParent(Control *parent)
	{
		//	destroy();
		//	create();
		//	update();

		u32 style;

		if (parent) {
			style = ::GetWindowLong(m_hwnd, GWL_STYLE);
			m_savedStyle = style;
			style &= ~WS_POPUP;
			style &= ~WS_CAPTION;
			style &= ~WS_SYSMENU;
			style &= ~WS_THICKFRAME;
			style |= WS_CHILD;
		}
		else {
			style &= ~WS_CHILD;
			style = m_savedStyle;
		}

		HWND hwndparent = parent ? parent->osdRef().handle() : NULL;
		::SetWindowLongPtr(m_hwnd, GWL_STYLE, (UINT_PTR)style);
		::SetParent(m_hwnd, hwndparent);

		//OSDControl::setParent(parent);
	}

	void OSDWindow::setSize(const Size &size)
	{
		::SetWindowPos(m_hwnd, HWND_TOPMOST, 0, 0, size.width(), size.height(), SWP_NOZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);

		//update status bar if any
		//status bar must be updated after any frame changes
		//automatic update happens if you change its font, but not after a window resize
	//	if(ref().statusBar())
	//		::SetWindowPos(ref().statusBar()->osdRef().handle(),NULL,0,0,0,0,SWP_NOZORDER | SWP_FRAMECHANGED);

		createBackBuffer();
	}


	Size OSDWindow::getFrameSize()
	{
		RECT ncArea = { 0,0,0,0 }; //Non-Client Area
		UINT winstyle, winexstyle;
		winstyle = (LONG_PTR)::GetWindowLongPtr(m_hwnd, GWL_STYLE);
		winexstyle = (LONG_PTR)::GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);
		::AdjustWindowRectEx(&ncArea, winstyle, ref().menuBarVisible(), winexstyle);

		RECT rc;
		RECT statusrect = { 0,0,0,0 };
		::GetWindowRect(m_hwnd, &rc);
		if (ref().statusBarVisible()) {
			::GetWindowRect(ref().statusBar()->osdRef().handle(), &statusrect);
		}

		Size windowSize = Size(rc.right - rc.left, rc.bottom - rc.top);
		Size ncSize = Size(ncArea.right - ncArea.left, ncArea.bottom - ncArea.top);
		Size statusSize = Size(statusrect.right - statusrect.left, statusrect.bottom - statusrect.top);

		Size size;
		size.setWidth(windowSize.width() - ncSize.width());
		size.setHeight(windowSize.height());
		size.setHeight(size.height() - ncSize.height()); //remove top shit (title and menu)
		size.setHeight(size.height() - statusSize.height()); //remove status bar

		if (ref().scrollBarVisible(0))
			size.setHeight(size.height() - ref().scrollBarWidth(0));
		if (ref().scrollBarVisible(1))
			size.setWidth(size.width() - ref().scrollBarWidth(1));

		//and we finally have the client metrics
		return size;
	}

	void OSDWindow::setTitle(const string& text)
	{
		::SetWindowText(m_hwnd, text.data());
	}

	void OSDWindow::setBackgroundColor(const Color &color)
	{
		Color _backcolor = ref().backgroundColor();
		COLORREF backcolor = RGB(_backcolor.red(), _backcolor.green(), _backcolor.blue());
		if (m_backBrush) ::DeleteObject(m_backBrush);
		m_backBrush = ::CreateSolidBrush(backcolor);
		::RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW);
	}

	void OSDWindow::setScrollLength(const Size &contentSize, const Size &workingSize)
	{
		SCROLLINFO scrollInfo;
		SCROLLBARINFO scrollBarInfo;
		scrollBarInfo.cbSize = sizeof(SCROLLBARINFO);

		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_RANGE | SIF_PAGE;
		scrollInfo.nMin = 0;
		scrollInfo.nMax = contentSize.width() - 1;
		scrollInfo.nPage = workingSize.width();
		::SetScrollInfo(m_hwnd, SB_HORZ, &scrollInfo, TRUE);

		scrollInfo.fMask = SIF_POS | SIF_RANGE;
		::GetScrollInfo(m_hwnd, SB_HORZ, &scrollInfo);
		::GetScrollBarInfo(m_hwnd, OBJID_HSCROLL, &scrollBarInfo);
		this->ref().m_hScrollPos = scrollInfo.nPos;
		this->ref().m_hScrollbarVisible = !(scrollBarInfo.rgstate[0] & STATE_SYSTEM_INVISIBLE);

		//
		scrollInfo.cbSize = sizeof(SCROLLINFO);
		scrollInfo.fMask = SIF_RANGE | SIF_PAGE;
		scrollInfo.nMin = 0;
		scrollInfo.nMax = contentSize.height() - 1;
		scrollInfo.nPage = workingSize.height();
		::SetScrollInfo(m_hwnd, SB_VERT, &scrollInfo, TRUE);

		scrollInfo.fMask = SIF_POS | SIF_RANGE;
		::GetScrollInfo(m_hwnd, SB_VERT, &scrollInfo);
		::GetScrollBarInfo(m_hwnd, OBJID_VSCROLL, &scrollBarInfo);
		this->ref().m_vScrollPos = scrollInfo.nPos;
		this->ref().m_vScrollbarVisible = !(scrollBarInfo.rgstate[0] & STATE_SYSTEM_INVISIBLE);

		createBackBuffer();
	}

	u32 OSDWindow::getScrollBarWidth(u32 which)
	{
		u32 width, arrow, thumb;
		if (which == 0) { //Horizontal scrollbar
			width = ::GetSystemMetrics(SM_CYHSCROLL);
			return width;
		}

		//Vertical scrollbar
		width = ::GetSystemMetrics(SM_CXVSCROLL);
		arrow = ::GetSystemMetrics(SM_CYVSCROLL);
		thumb = ::GetSystemMetrics(SM_CYVTHUMB);
		return width;
	}




	LRESULT OSDWindow::OnMove(WPARAM wParam, LPARAM lParam)
	{
		if (ref().m_onMove)
			ref().m_onMove(ref());

		return ::DefWindowProc(m_hwnd, WM_MOVE, wParam, lParam);
	}

	LRESULT OSDWindow::OnSize(WPARAM wParam, LPARAM lParam)
	{
		//todo: know if it was an user or application resize

		//-called after SetMenu
		//-needs statusbar update
		createBackBuffer(); //recreate back buffer

		Window& ref = this->ref();
		StatusBar* status = ref.statusBar();
		if (status)
			::SetWindowPos(status->osdRef().handle(), 0, 0, 0, 0, 0, SWP_NOZORDER | SWP_FRAMECHANGED);

		DWORD resizeType = wParam;
		//The Windows Resizing Types, from MSDN:
		/*
			4 - SIZE_MAXHIDE - Message is sent to all pop-up windows when some other window is maximized.
			2 - SIZE_MAXIMIZED - The window has been maximized.
			3 - SIZE_MAXSHOW - Message is sent to all pop-up windows when some other window has been restored to its former size.
			1 - SIZE_MINIMIZED - The window has been minimized.
			0 - SIZE_RESTORED - The window has been resized, but neither the SIZE_MINIMIZED nor SIZE_MAXIMIZED value applies.
		*/

		//LPARAM contains the new size of the CLIENT area
		//Don't use these values because they don't take status bar and scrollbars into account
		//	Size newFrameSize = Size(LOWORD(lParam),HIWORD(lParam));

		//GetTotal window Size
		//Window may have clamp'd window size, so we need to update ref.m_size even if it wasn't a user resize
		RECT rc;
		::GetWindowRect(m_hwnd, &rc);
		Size newSize = Size(rc.right - rc.left, rc.bottom - rc.top);
		ref.m_curSize = newSize;
		if (resizeType == 0) {
			ref.m_size = newSize;
		}

		if (!ref.m_applicationResize) {
			if (resizeType == 0 || resizeType == SIZE_MAXIMIZED) {
				if (ref.layout()) {
					Size newFrameSize = ref.getFrameSize();
					ref.layout()->setSize(newFrameSize);
				}
			}
		}
		ref.m_applicationResize = false;

		if (ref.m_onSize)
			ref.m_onSize(ref);

		return ::DefWindowProc(m_hwnd, WM_SIZE, wParam, lParam);
	}

	LRESULT OSDWindow::OnClose(WPARAM wParam, LPARAM lParam)
	{
		//If an application processes this message, it should return zero.

		ref().setVisible(false);
		if (ref().m_onClose) {
			ref().m_onClose(ref());
			return 0;
		}

		return 0;
	}

	LRESULT OSDWindow::OnHScroll(WPARAM wParam, LPARAM lParam)
	{
		//Return Value
		//If an application processes this message, it should return zero. 

		HWND hwnd;
		SCROLLINFO si;
		int curPos;

		//check if this message is from a control
		if (lParam != NULL) {
			UIObj *uiobj = reinterpret_cast<UIObj*>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
			if (uiobj) {
				OSDUIObj& osdUIObj = uiobj->osdRef();
				return osdUIObj.HandleParentMessage(WM_HSCROLL, wParam, lParam);
			}
			return 0;
		}

		//this is a window scrollbar
		if (LOWORD(wParam) == SB_ENDSCROLL)
			return 0;

		hwnd = m_hwnd;

		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		::GetScrollInfo(hwnd, SB_HORZ, &si);

		// Save the position for comparison later on
		curPos = si.nPos;
		switch (LOWORD(wParam))
		{
		case SB_LINELEFT: si.nPos--; break; // user clicked the top arrow
		case SB_LINERIGHT: si.nPos++; break; // user clicked the bottom arrow
		case SB_PAGELEFT: si.nPos -= si.nPage; break; // user clicked the scroll bar shaft above the scroll box
		case SB_PAGERIGHT: si.nPos += si.nPage; break;  // user clicked the scroll bar shaft below the scroll box 
		case SB_THUMBTRACK:	si.nPos = si.nTrackPos; break; // user dragged the scroll box
		case SB_LEFT: si.nPos = si.nMin; break; // user clicked the HOME keyboard key
		case SB_RIGHT: si.nPos = si.nMax; break; // user clicked the END keyboard key
		}

		//Set the position and then retrieve it. Due to adjustments
		//by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;
		::SetScrollInfo(hwnd, SB_HORZ, &si, TRUE);
		::GetScrollInfo(hwnd, SB_HORZ, &si);

		if (curPos != si.nPos) {
			ref().m_hScrollPos = si.nPos;
			if (ref().m_onHScroll)
				ref().m_onHScroll(ref());
		}

		return 0;
	}

	LRESULT OSDWindow::OnVScroll(WPARAM wParam, LPARAM lParam)
	{
		HWND hwnd;
		SCROLLINFO si;
		int curPos;

		//check if this message is from a control
		if (lParam != NULL) {
			UIObj *uiobj = reinterpret_cast<UIObj*>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
			if (uiobj) {
				OSDUIObj& osdUIObj = uiobj->osdRef();
				return osdUIObj.HandleParentMessage(WM_VSCROLL, wParam, lParam);
			}
			return 0;
		}

		//this is a window scrollbar
		if (LOWORD(wParam) == SB_ENDSCROLL)
			return 0;

		hwnd = m_hwnd;

		si.cbSize = sizeof(si);
		si.fMask = SIF_ALL;
		::GetScrollInfo(hwnd, SB_VERT, &si);

		// Save the position for comparison later on
		curPos = si.nPos;
		switch (LOWORD(wParam))
		{
		case SB_LINEUP: si.nPos--; break; // user clicked the top arrow
		case SB_LINEDOWN: si.nPos++; break; // user clicked the bottom arrow
		case SB_PAGEUP: si.nPos -= si.nPage; break; // user clicked the scroll bar shaft above the scroll box
		case SB_PAGEDOWN: si.nPos += si.nPage; break;  // user clicked the scroll bar shaft below the scroll box 
		case SB_THUMBTRACK:	si.nPos = si.nTrackPos; break; // user dragged the scroll box
		case SB_TOP: si.nPos = si.nMin; break; // user clicked the HOME keyboard key
		case SB_BOTTOM: si.nPos = si.nMax; break; // user clicked the END keyboard key
		}

		//Set the position and then retrieve it. Due to adjustments
		//by Windows it may not be the same as the value set.
		si.fMask = SIF_POS;
		::SetScrollInfo(hwnd, SB_VERT, &si, TRUE);
		::GetScrollInfo(hwnd, SB_VERT, &si);

		if (curPos != si.nPos) {
			ref().m_vScrollPos = si.nPos;
			if (ref().m_onVScroll)
				ref().m_onVScroll(ref());
		}

		return 0;
	}



	void OSDWindow::onPaintEvent(PaintEvent& event)
	{
		//An application returns zero if it processes this message. 
		if (m_backBrush == NULL)
			return;

		PAINTSTRUCT ps;
		HDC hdc = ::BeginPaint(m_hwnd, &ps);

		HDC hdcBB = m_backBufferDC ? m_backBufferDC : hdc;

		//clear the background with specific color
		RECT rc;
		::GetClientRect(m_hwnd, &rc);
		Color backcolor = ref().backgroundColor();
		::FillRect(hdcBB, &rc, m_backBrush);

		flip(hdc);
		::EndPaint(m_hwnd, &ps);
	}

	LRESULT OSDWindow::OnEraseBkgnd(WPARAM wParam, LPARAM lParam)
	{
		//	return ::DefWindowProc(m_hwnd,WM_ERASEBKGND,wParam,lParam);
		return 1;
	}

	WIN_MSG(WM_NOTIFY, OSDWindow, OnNotify)  //0x004E
	{
		LPNMHDR lpnmhdr = (LPNMHDR)lParam;
		if (lpnmhdr->hwndFrom) {
			UIObj *uiobj = (UIObj*)::GetWindowLongPtr(lpnmhdr->hwndFrom, GWLP_USERDATA);
			if (uiobj)
			{
				OSDUIObj& obj = uiobj->osdRef();
				return obj.HandleParentMessage(WM_NOTIFY, wParam, lParam);
			}
		}
		return 0;
	}

	LRESULT OSDWindow::OnCommand(WPARAM wParam, LPARAM lParam) //0x0111
	{
		if ((HWND)lParam == NULL) {
			if (HIWORD(wParam) == 0) {
				//this is a menu item
				OSDUIObj* obj = OSDBase::find(LOWORD(wParam));
				if (obj)
					return obj->HandleParentMessage(WM_COMMAND, wParam, lParam);
			}
			else if (HIWORD(wParam) == 1) {
				//this is an accelerator
			}
		}
		else {
			//this is a control
			UIObj *uiobj = reinterpret_cast<UIObj*>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
			if (uiobj) {
				OSDUIObj& obj = uiobj->osdRef();
				return obj.HandleParentMessage(WM_COMMAND, wParam, lParam);
			}
		}

		//If an application processes this message, it should return zero. 
		return 0;
	}

	LRESULT OSDWindow::OnMouseWheel(WPARAM wParam, LPARAM lParam)
	{
		Window &window = ref();

		int delta = (int)(short)HIWORD(wParam);
		int keys = (int)(short)LOWORD(wParam);
		int x, y;
		x = LOWORD(lParam);
		y = HIWORD(lParam);

		POINT pt;
		pt.x = x;
		pt.y = y;
		::ScreenToClient(m_hwnd, &pt);

		HWND childWindow = ::ChildWindowFromPoint(m_hwnd, pt);
		if (childWindow == 0)
			childWindow = m_hwnd;

		if (childWindow != m_hwnd) {
			//A child Window
			return ::SendMessage(childWindow, WM_MOUSEWHEEL, wParam, lParam);
		}

		//Same Window
		SCROLLINFO si;
		si.cbSize = sizeof(SCROLLINFO);
		si.fMask = SIF_POS;
		::GetScrollInfo(m_hwnd, SB_VERT, &si);

		si.nPos -= delta;
		::SetScrollInfo(m_hwnd, SB_VERT, &si, TRUE);
		::GetScrollInfo(m_hwnd, SB_VERT, &si);

		window.m_vScrollPos = si.nPos;

		if (window.m_onVScroll)
			window.m_onVScroll(window);
		//ChildWindows must return 0;
		return 0;
	}

	HDC OSDWindow::GetDC()
	{
		return m_backBufferDC ? m_backBufferDC : ::GetDC(m_hwnd);
	}

	void OSDWindow::ReleaseDC(HDC hdc)
	{
		if (hdc != m_backBufferDC)
			::ReleaseDC(m_hwnd, hdc);
	}

}