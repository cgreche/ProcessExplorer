
//OSDControl
// File: osdcontrol.cpp
// Last edit: 27/06/2017 02:14 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	WIN_MSG_DEF(WM_MOVE, OSDControl, OnMove) //0x0003
		WIN_MSG_DEF(WM_SIZE, OSDControl, OnSize) //0x0005
		WIN_MSG_DEF(WM_SETFOCUS, OSDControl, OnSetFocus) //0x0007
		WIN_MSG_DEF(WM_KILLFOCUS, OSDControl, OnKillFocus) //0x0008
		WIN_MSG_DEF(WM_ENABLE, OSDControl, OnEnable); //0x000A
	WIN_MSG_DEF(WM_ERASEBKGND, OSDControl, OnEraseBkgnd);

	WIN_MSG_DEF(WM_SETCURSOR, OSDControl, OnSetCursor) //0x0020

		WIN_MSG(WM_NOTIFY, OSDControl, OnNotify) //0x004E
	{
		LPNMHDR lpnmhdr = (LPNMHDR)lParam;
		if (lpnmhdr->hwndFrom) {
			char a[256];
			GetWindowText(lpnmhdr->hwndFrom, a, 256);

			//todo: change maybe
			//is this an event? make it ona proper Event structure
			//if not, work on a better integration between osdgridview and osd header
			//Explicity call HDN_xxxA/W because it only sends Unicode version messages
			//when styles are on
			if (lpnmhdr->code == HDN_ITEMCHANGEDA
				|| lpnmhdr->code == HDN_ITEMCHANGEDW
				) {
				int col, width;
				GridView *grid = (GridView*)::GetWindowLongPtr(m_hwnd, GWLP_USERDATA);
				if (lpnmhdr->code == HDN_ITEMCHANGEDA) {
					LPNMHEADERA lpnmheader = (LPNMHEADERA)lpnmhdr;
					col = lpnmheader->iItem;
					width = lpnmheader->pitem->cxy;
				}
				else {
					LPNMHEADERW lpnmheader = (LPNMHEADERW)lpnmhdr;
					col = lpnmheader->iItem;
					width = lpnmheader->pitem->cxy;
					grid->onColumnResize(col, width);
				}

			}
			else {
				int a = 1;
			}

			UIObj *uiobj = (UIObj*)::GetWindowLongPtr(lpnmhdr->hwndFrom, GWLP_USERDATA);
			if (uiobj)
			{
				OSDUIObj& obj = uiobj->osdRef();
				return obj.HandleParentMessage(WM_NOTIFY, wParam, lParam);
			}
		}

		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_NOTIFY, wParam, lParam);
	}

	WIN_MSG_DEF(WM_NCHITTEST, OSDControl, OnNCHitTest) //0x0084
		WIN_MSG_DEF(WM_NCPAINT, OSDControl, OnNCPaint) //0x0085

		WIN_MSG_DEF(WM_CHAR, OSDControl, OnChar) //0x0102
		WIN_MSG(WM_COMMAND, OSDControl, OnCommand) //0x0111
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

		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_COMMAND, wParam, lParam);
	}

	WIN_MSG(WM_HSCROLL, OSDControl, OnHScroll)
	{
		//check if this message is from another control
		if (lParam != NULL) {
			UIObj *uiobj = reinterpret_cast<UIObj*>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
			if (uiobj) {
				OSDUIObj& osdUIObj = uiobj->osdRef();
				return osdUIObj.HandleParentMessage(WM_HSCROLL, wParam, lParam);
			}
			return 0;
		}
		//this is a window scrollbar

		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_HSCROLL, wParam, lParam);
	}

	WIN_MSG(WM_VSCROLL, OSDControl, OnVScroll)
	{
		//check if this message is from another control
		if (lParam != NULL) {
			UIObj *uiobj = reinterpret_cast<UIObj*>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
			if (uiobj) {
				OSDUIObj& osdUIObj = uiobj->osdRef();
				return osdUIObj.HandleParentMessage(WM_VSCROLL, wParam, lParam);
			}
			return 0;
		}
		//this is a window scrollbar

		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_VSCROLL, wParam, lParam);
	}

	WIN_MSG(WM_CTLCOLOREDIT, OSDControl, OnCtlColorEdit)
	{
		UIObj *uiobj = reinterpret_cast<UIObj*>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
		if (uiobj) {
			OSDUIObj& obj = uiobj->osdRef();
			return obj.HandleParentMessage(WM_CTLCOLOREDIT, wParam, lParam);
		}

		return (LRESULT)::GetStockObject(NULL_BRUSH);
	}

	WIN_MSG(WM_CTLCOLORBTN, OSDControl, OnCtlColorBtn)
	{
		UIObj *uiobj = reinterpret_cast<UIObj*>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
		if (uiobj) {
			OSDUIObj& obj = uiobj->osdRef();
			return obj.HandleParentMessage(WM_CTLCOLORBTN, wParam, lParam);
		}

		return (LRESULT)::GetStockObject(NULL_BRUSH);
	}

	WIN_MSG(WM_CTLCOLORSTATIC, OSDControl, OnCtlColorStatic)
	{
		//sent by a static or a disabled/read only edit control
		//Curious thing:
		//The combination of edit control + using window themes + windows 7 32bits (doesn't happen in 64 bits) (may happen in other windows versions)
		//will call this, even if the edit control is not disabled/read-only 
		UIObj *uiobj = reinterpret_cast<UIObj*>(GetWindowLongPtr((HWND)lParam, GWLP_USERDATA));
		if (uiobj) {
			OSDUIObj& obj = uiobj->osdRef();
			return obj.HandleParentMessage(WM_CTLCOLORSTATIC, wParam, lParam);
		}

		return (LRESULT)::GetStockObject(NULL_BRUSH);
	}

	WIN_MSG_DEF(WM_MOUSEMOVE, OSDControl, OnMouseMove)
		WIN_MSG_DEF(WM_LBUTTONDOWN, OSDControl, OnLButtonDown)
		WIN_MSG_DEF(WM_LBUTTONUP, OSDControl, OnLButtonUp)
		WIN_MSG_DEF(WM_RBUTTONDOWN, OSDControl, OnRButtonDown)
		WIN_MSG_DEF(WM_RBUTTONUP, OSDControl, OnRButtonUp)
		WIN_MSG_DEF(WM_MBUTTONDOWN, OSDControl, OnMButtonDown)
		WIN_MSG_DEF(WM_MBUTTONUP, OSDControl, OnMButtonUp)
		WIN_MSG_DEF(WM_MOUSEWHEEL, OSDControl, OnMouseWheel)
		WIN_MSG_DEF(WM_MOUSELEAVE, OSDControl, OnMouseLeave)
		WIN_MSG_DEF(WM_PASTE, OSDControl, OnPaste)



	void OSDControl::onPaintEvent(PaintEvent &event)
	{
		event.ignore();
	}

	OSDControl::~OSDControl()
	{
	}

	void OSDControl::create()
	{
		extern void createWindow(OSDControl &osdControl, const TCHAR *windowClass, DWORD exStyle, DWORD style);
		createWindow(*this, TEXT("static"), 0, WS_TABSTOP | SS_OWNERDRAW);
	}

	void OSDControl::destroy()
	{
		if (m_hwnd) {
			::DestroyWindow(m_hwnd);
			m_hwnd = NULL;
		}

		if (m_backBrush) {
			::DeleteObject(m_backBrush);
			m_backBrush = NULL;
		}
	}

	void OSDControl::update()
	{
		setEnabled(ref().enabled());
		setVisibilityMode(ref().visibilityMode());
		setFont(ref().font().desc());
		setPosition(ref().position());
		setSize(ref().size());
		setStyle(ref().style());
		setBackgroundColor(ref().backgroundColor());
		setTransparent(ref().transparent());
		setVisible(ref().visible());
	}


	void OSDControl::setVisible(bool visible)
	{
		//	::ShowWindow(m_hwnd, visible ? SW_SHOWNORMAL : SW_HIDE);
		if (!visible)
			::ShowWindow(m_hwnd, SW_HIDE);
		else
			setVisibilityMode(ref().m_visibilityMode);
	}

	void OSDControl::setEnabled(bool enabled)
	{
		::EnableWindow(m_hwnd, (BOOL)(enabled != false));
	}


	void OSDControl::setFocus()
	{
		::SetFocus(m_hwnd);
	}

	void OSDControl::setFont(const FontDesc &desc)
	{
		::SendMessage(m_hwnd, WM_SETFONT, (WPARAM)ref().font().osdRef().object(), (LPARAM)TRUE);
	}


	void OSDControl::setPosition(const Point& position)
	{
		::SetWindowPos(m_hwnd, NULL, position.x(), position.y(), 0, 0, SWP_NOZORDER | SWP_NOSIZE);
	}

	void OSDControl::setSize(const Size& size)
	{
		::SetWindowPos(m_hwnd, NULL, 0, 0, size.width(), size.height(), SWP_NOZORDER | SWP_NOMOVE | SWP_FRAMECHANGED);
	}

	Size OSDControl::getFrameSize()
	{
		RECT junk = { 0,0,0,0 };
		UINT winstyle, winexstyle;
		winstyle = (LONG_PTR)::GetWindowLongPtr(m_hwnd, GWL_STYLE);
		winexstyle = (LONG_PTR)::GetWindowLongPtr(m_hwnd, GWL_EXSTYLE);
		::AdjustWindowRectEx(&junk, winstyle, FALSE, winexstyle);

		RECT rc;
		::GetWindowRect(m_hwnd, &rc);

		Size size((rc.right - rc.left) - (junk.right - junk.left), (rc.bottom - rc.top));
		//and we finally have the client metrics
		return size;
	}

	void OSDControl::setVisibilityMode(VisibilityMode visibilityMode)
	{
		if (!ref().visible())
			return;

		//if we are setting fullscreen mode, set some window styles
		if (visibilityMode == Fullscreen) {
			this->setStyle(ref().style() & ~(CS_Caption | CS_Resizable | CS_SysMenu));
		}
		else {
			//else, reset the window styles, we could have been in fullscreen mode before
			//resetting the style, will set the window to resize aswell, so set the size here before this happens
			if (visibilityMode == ShowNormal)
				this->setSize(ref().m_size);

			this->setStyle(ref().style());
		}

		DWORD showMode;
		switch (visibilityMode) {
		case ShowNormal: showMode = SW_SHOWNORMAL; break;
		case Minimized: showMode = SW_SHOWMINIMIZED; break;
		case Maximized: showMode = SW_SHOWMAXIMIZED; break;
		case Fullscreen: showMode = SW_SHOWMAXIMIZED; break;
		default:
			showMode = SW_SHOWNORMAL;
			break;
		}

		::ShowWindow(m_hwnd, showMode);
	}

	void OSDControl::setStyle(u32 style)
	{
		DWORD oldStyle, winStyle;
		oldStyle = winStyle = ::GetWindowLongPtr(m_hwnd, GWL_STYLE);
		//remove "global" window styles, which are the ones we are setting
		winStyle &= ~(WS_POPUP | WS_CAPTION | WS_THICKFRAME | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

		if (style & CS_Popup) {
			winStyle |= WS_POPUP;
		}
		else {
			if (style & CS_Caption)
				winStyle |= WS_CAPTION;
			if (style & CS_Resizable)
				winStyle |= WS_THICKFRAME;
			if (style & CS_SysMenu)
				winStyle |= WS_SYSMENU;
			if (style & CS_MinimizeButton)
				winStyle |= WS_MINIMIZEBOX;
			if (style & CS_MaximizeButton)
				winStyle |= WS_MAXIMIZEBOX;
		}

		if (style & CS_Border)
			winStyle |= WS_BORDER;

		//	if(ref().m_parent) {
		//		winStyle &= ~(WS_POPUP|WS_CAPTION|WS_THICKFRAME|WS_SYSMENU|WS_MINIMIZE|WS_MAXIMIZE);
		//	}

			//don't update the styles if we don't need to
		if (winStyle == oldStyle)
			return;

		::SetWindowLongPtr(m_hwnd, GWL_STYLE, (LONG_PTR)winStyle);
		::SetWindowPos(m_hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}

	void OSDControl::setBackgroundColor(const Color& color)
	{
		Color _backcolor = ref().backgroundColor();
		COLORREF backcolor = RGB(_backcolor.red(), _backcolor.green(), _backcolor.blue());
		if (m_backBrush) ::DeleteObject(m_backBrush);
		m_backBrush = ::CreateSolidBrush(backcolor);
		::RedrawWindow(m_hwnd, NULL, NULL, RDW_INVALIDATE | RDW_ALLCHILDREN | RDW_UPDATENOW); //force all children to repaint with new background color
	}

	void OSDControl::setTransparent(bool transparent)
	{
		this->redraw();
	}

	void OSDControl::setParent(Control *parent)
	{
		//The first method makes some controls look like junk, and other controls to break
		/*
		u32 style = ::GetWindowLong(m_hwnd,GWL_STYLE);
		style = parent ? ((style&~(WS_CAPTION|WS_CLIPSIBLINGS))|WS_CHILD) : (style&~WS_CHILD);
	//	::SetWindowLongPtr(m_hwnd,GWL_STYLE,(UINT_PTR)style);

		::SetWindowLongPtr(m_hwnd,GWL_STYLE,(UINT_PTR)WS_CHILD|WS_VISIBLE);
		HWND hwndparent = parent ? parent->GetRef()->handle() : NULL;
		::SetParent(m_hwnd,hwndparent);
		*/

		//this second method invalidates child of child windows
	//	destroy();
	//	create();
	//	update();

		//lets try the first one again
	/*	u32 style = ::GetWindowLongPtr(m_hwnd,GWL_STYLE);
		if(parent) {
			style &= ~(WS_POPUP|WS_THICKFRAME|WS_SYSMENU|WS_MINIMIZE|WS_MAXIMIZE|WS_CAPTION);
			style |= WS_CHILD;
		}
		else {
			style &= ~WS_CHILD;
			style &= ~WS_VISIBLE;
		}

		::SetWindowLongPtr(m_hwnd,GWL_STYLE,(UINT_PTR)style);
		HWND hwndparent = parent ? parent->osdRef().handle() : NULL;
		::SetParent(m_hwnd,hwndparent);
		::SetWindowPos(m_hwnd,0,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER|SWP_SHOWWINDOW);

		update(); //update pos/size
		*/


		//method 3: lets do the second method on us and all child controls
		destroy();
		create();
		update();

		ControlList &childControls = ref().m_childControls;
		ControlList::iterator it;
		for (it = childControls.begin(); it != childControls.end(); ++it) {
			Control *control = *it;
			control->osdRef().setParent(&this->ref());
		}
	}

	void OSDControl::redraw()
	{
		::InvalidateRect(m_hwnd, NULL, TRUE);
	}

}