
//OSDButton
// File: osdbutton.cpp
// Last edit: 04/08/2014 21:54 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	OSDButton::OSDButton(Button &ref) : OSDControl(ref)
	{
	}

	void OSDButton::create()
	{
		extern void createWindow(OSDControl &osdControl, const TCHAR *windowClass, DWORD exStyle, DWORD style);
		createWindow(*this, TEXT("button"), 0, WS_TABSTOP | BS_PUSHBUTTON);
	}

	void OSDButton::update()
	{
		OSDControl::update();
		setText(ref().text());
	}



	void OSDButton::setText(const string &text)
	{
		::SetWindowText(m_hwnd, text.data());
	}

	void OSDButton::setTextColor(const Color &color)
	{
	}


	LRESULT OSDButton::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NOTIFY) {
			int code = ((LPNMHDR)lParam)->code;

			if (code == NM_CUSTOMDRAW) {
				//comctl32 6.0
				LPNMCUSTOMDRAW lpNMCustomDraw = (LPNMCUSTOMDRAW)lParam;
				switch (lpNMCustomDraw->dwDrawStage) {
				case CDDS_PREPAINT:
				{
					return CDRF_DODEFAULT;
				}break;
				case CDDS_PREERASE:
				{
					return CDRF_DODEFAULT;
				}break;
				default:
					int a = lpNMCustomDraw->dwDrawStage;
				}

			}
		}

		if (msg == WM_COMMAND) {
			if (ref().m_onClick)
				ref().m_onClick(ref());
			return FALSE;
		}

		if (msg == WM_CTLCOLORBTN)
		{
			if (ref().parent()) {
				HBRUSH hBrush = ref().parent()->osdRef().backgroundBrush();
				if (hBrush)
					return (LRESULT)hBrush;
			}

			return ::DefWindowProc(ref().parent()->osdRef().handle(), msg, wParam, lParam);
			//		if(ref().parent()) {
					//	return (LRESULT)::GetStockObject(BLACK_BRUSH);
			//		}
		}

		return FALSE;
	}

}