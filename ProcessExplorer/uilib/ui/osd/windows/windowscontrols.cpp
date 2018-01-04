
//WindowsControls
// File: windowscontrols.cpp
// Last edit: 04/01/2018 01:59 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	DWORD controlDefaultStyle = WS_CLIPSIBLINGS | WS_CLIPCHILDREN;

#define MSG_HANDLE(_msg,_class,_method) \
	case _msg: return _class._method(wParam,lParam);

#define MSG(_msg) \
	case _msg: break;

	LRESULT CALLBACK GenericWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
	{
		Control *control = (Control*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
		if (!control)
			return ::DefWindowProc(hwnd, msg, wParam, lParam);

		OSDControl &osdControl = control->osdRef();

		Event *event = NULL;
		Point pos;
		u32 button = 0;
		u32 buttons;
		bool accepted;

		switch (msg)
		{
		case WM_GETMINMAXINFO:
		{
			MINMAXINFO *info = (MINMAXINFO*)lParam;
			//	info->ptMaxSize.x = 800;
			//	info->ptMinTrackSize.x = 800;
			//	info->ptMaxTrackSize.x = 200;
			break;
		};

		case WM_ENTERSIZEMOVE:
			return 0;
		case WM_EXITSIZEMOVE:
			return 0;

		case WM_SYSCOMMAND:
		{
			//SC_MOVE and SC_SIZE is used in conjunction with HT* and WMSZ_* respectively
			//example: SC_MOVE|HTCAPTION, SC_SIZE|WMSZ_LEFT
	//		if((wParam&0xfff0) == SC_MOVE)
	//			return 0;
	//		if((wParam&0xfff0) == SC_SIZE)
	//			return 0;

			break;
		}

		MSG(WM_DESTROY) //0x0002
			MSG_HANDLE(WM_MOVE, osdControl, OnMove) //0x0003
			MSG_HANDLE(WM_SIZE, osdControl, OnSize) //0x0005
			MSG_HANDLE(WM_SETFOCUS, osdControl, OnSetFocus) //0x0007
			MSG_HANDLE(WM_KILLFOCUS, osdControl, OnKillFocus) //0x0008
			MSG_HANDLE(WM_ENABLE, osdControl, OnEnable) //0x000A
			MSG(WM_SETTEXT) //0x000C
			MSG(WM_GETTEXT) //0x000D
			MSG(WM_GETTEXTLENGTH) //0x000E
			//MSG_HANDLE(WM_PAINT,osdControl,OnPaint) //0x000F
		case WM_PAINT:
		{
			event = new PaintEvent();
			control->onPaintEvent((PaintEvent&)*event);
			break;
		}
		//MSG_HANDLE(WM_CLOSE,osdControl,OnClose) //0x0010
		case WM_CLOSE:
			control->onCloseEvent();
			break;
			MSG_HANDLE(WM_ERASEBKGND, osdControl, OnEraseBkgnd) //0x0014
			//case WM_ERASEBKGND: return FALSE;
			/*
				case WM_ERASEBKGND:
					{
						return FALSE;
						HDC hdc = ::GetDC(hwnd);
						HBRUSH brush = ::CreateSolidBrush(RGB(255,100,0));
						RECT rc;
						//::GetUpdateRect(hwnd,&rc,FALSE);
						FillRect(hdc,&rc,brush);
						::DeleteObject(brush);
						::ReleaseDC(hwnd,hdc);
						return FALSE;
					};
					*/
				MSG(WM_SHOWWINDOW) //0x0018
				MSG_HANDLE(WM_SETCURSOR, osdControl, OnSetCursor) //0x0020
				MSG(WM_MOUSEACTIVATE) //0x0021
				MSG(WM_CHILDACTIVATE) //0x0022
		case WM_MEASUREITEM: //0x002C
			{
				int ctlId = wParam;
				break;
			}
			MSG(WM_WINDOWPOSCHANGING) //0x0046
				MSG(WM_WINDOWPOSCHANGED) //0x0047
				MSG_HANDLE(WM_NOTIFY, osdControl, OnNotify) //0x004E
				MSG(WM_CONTEXTMENU) //0x007B
				MSG(WM_NCDESTROY) //0x0082
				//MSG(WM_NCCALCSIZE) //0x0083
				MSG_HANDLE(WM_NCHITTEST, osdControl, OnNCHitTest) //0x0084
				MSG_HANDLE(WM_NCPAINT, osdControl, OnNCPaint) //0x0085
				//case WM_NCPAINT: return FALSE;
				MSG(WM_NCMOUSEMOVE) //0x00A0

				MSG(BM_SETSTATE) //0x00F3

		case WM_KEYDOWN: //0x0100
			{
				event = new KeyEvent((int)wParam);
				control->onKeyPressEvent((KeyEvent&)*event);
				break;
			}

		case WM_KEYUP: //0x0101
			{
				event = new KeyEvent((int)wParam);
				control->onKeyReleaseEvent((KeyEvent&)*event);
				break;
			}

		case WM_SYSKEYDOWN: //0x104
		{
			event = new KeyEvent((int)wParam);
			control->onKeyPressEvent((KeyEvent&)*event);
			break;
		}

		case WM_SYSKEYUP: //0x105
		{
			event = new KeyEvent((int)wParam);
			control->onKeyReleaseEvent((KeyEvent&)*event);
			break;
		}
				MSG_HANDLE(WM_CHAR, osdControl, OnChar) //0x0102
				MSG_HANDLE(WM_COMMAND, osdControl, OnCommand) //0x0111
				MSG_HANDLE(WM_HSCROLL, osdControl, OnHScroll) //0x0114
				MSG_HANDLE(WM_VSCROLL, osdControl, OnVScroll) //0x0115
			//	case WM_UPDATEUISTATE: /*0x128*/ break;
			//	MSG_HANDLE(WM_CTLCOLORMSGBOX,osdControl,OnCtlColorMsgBox) //0x0132
				MSG_HANDLE(WM_CTLCOLOREDIT, osdControl, OnCtlColorEdit) //0x0133
			//	MSG_HANDLE(WM_CTLCOLORLISTBOX,osdControl,OnCtlColorListBox) //0x0134
				MSG_HANDLE(WM_CTLCOLORBTN, osdControl, OnCtlColorBtn) //0x0135
			//	MSG_HANDLE(WM_CTLCOLORDLG,osdControl,OnCtlColorDlg) //0x0136
			//	MSG_HANDLE(WM_CTLCOLORSCROLLBAR,osdControl,OnCtlColorScrollbar) //0x0137
				MSG_HANDLE(WM_CTLCOLORSTATIC, osdControl, OnCtlColorStatic) //0x0138

		case WM_MOUSEMOVE: //0x200
			buttons = wParam;

			if (!osdControl.m_hover) {
				TRACKMOUSEEVENT tme;
				tme.cbSize = sizeof(TRACKMOUSEEVENT);
				tme.hwndTrack = hwnd;
				tme.dwFlags = TME_LEAVE;
				::TrackMouseEvent(&tme);
				event = new MouseEvent(Point((int)LOWORD(lParam), (int)HIWORD(lParam)), button, buttons);
				control->onMouseEnterEvent((MouseEvent&)*event);
				delete event;
			}
			osdControl.m_hover = true;
			event = new MouseEvent(Point((int)LOWORD(lParam), (int)HIWORD(lParam)), button, buttons);
			control->onMouseMoveEvent((MouseEvent&)*event);
			break;

		case WM_LBUTTONDOWN: //0x201
		case WM_RBUTTONDOWN: //0x204
		case WM_MBUTTONDOWN: //0x207
			if (msg == WM_LBUTTONDOWN) button = 1;
			else if (msg == WM_RBUTTONDOWN) button = 2;
			else button = 4;
			buttons = wParam; //todo: change: analyse data to set proper buttons
			event = new MouseEvent(Point((int)LOWORD(lParam), (int)HIWORD(lParam)), button, buttons);
			control->onMousePressEvent((MouseEvent&)*event);
			break;

		case WM_LBUTTONUP: //0x202
		case WM_RBUTTONUP: //0x205
		case WM_MBUTTONUP: //0x208
			if (msg == WM_LBUTTONUP) button = 1;
			else if (msg == WM_RBUTTONUP) button = 2;
			else button = 4;
			buttons = wParam; //todo: change
			event = new MouseEvent(Point((int)LOWORD(lParam), (int)HIWORD(lParam)), button, buttons);
			control->onMouseReleaseEvent((MouseEvent&)*event);
			break;

		case WM_LBUTTONDBLCLK: //0x203
		case WM_RBUTTONDBLCLK: //0x206
		case WM_MBUTTONDBLCLK: //0x209
			if (msg == WM_LBUTTONDBLCLK) button = 1;
			else if (msg == WM_RBUTTONDBLCLK) button = 2;
			else button = 4;
			buttons = wParam; //todo: change
			event = new MouseEvent(Point((int)LOWORD(lParam), (int)HIWORD(lParam)), button, buttons);
			control->onMouseDoubleClickEvent((MouseEvent&)*event);
			break;

			//		case WM_MOUSEWHEEL: //0x020a break;
			MSG(WM_SIZING) //0x0214
				MSG(WM_CAPTURECHANGED) //0x0215
				MSG(WM_MOVING) //0x0216
				MSG(WM_IME_SETCONTEXT) //0x0281
				MSG(WM_IME_NOTIFY) //0x0282
		//		MSG_HANDLE(WM_MOUSELEAVE,osdControl,OnMouseLeave) //0x02A3
		case WM_MOUSELEAVE: //0x2A3
			osdControl.m_hover = false;
			event = new MouseEvent(Point(0, 0), 0, 0);
			control->onMouseLeaveEvent((MouseEvent&)*event);
			break;

			MSG_HANDLE(WM_PASTE, osdControl, OnPaste) //0x0303

#ifdef _DEBUG
		default:
			int a = msg;
			break;
#endif
		};

		accepted = event && event->accepted();
		if (event)
			delete event;

		if (!accepted)
			return ::CallWindowProc(osdControl.originalWndProc(), hwnd, msg, wParam, lParam);

		return FALSE;
	};


	void createWindow(OSDControl &osdControl, const TCHAR *windowClass, DWORD exStyle, DWORD style)
	{
		HWND hwndParent = osdControl.ref().parent() ? osdControl.ref().parent()->osdRef().handle() : NULL;

		style |= controlDefaultStyle;
		if (hwndParent)
			style |= WS_CHILD;

		HWND hwnd = CreateWindowEx(exStyle, windowClass, TEXT(""),
			style,
			0, 0, 0, 0,
			hwndParent, (HMENU)NULL, GetModuleHandle(NULL), 0);
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)&osdControl.ref());

		osdControl.m_hwnd = hwnd;
		osdControl.setWndProc(GenericWndProc);
	}

}