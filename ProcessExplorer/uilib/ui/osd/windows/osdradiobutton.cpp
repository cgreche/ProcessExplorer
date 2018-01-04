
//OSDButton
// File: osdbutton.cpp
// Last edit: 04/08/2014 21:56 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	OSDRadioButton::OSDRadioButton(RadioButton &ref) : OSDButton(ref)
	{
	}

	void OSDRadioButton::create()
	{
		createWindow(*this, TEXT("button"), 0, WS_TABSTOP | BS_RADIOBUTTON);
	}

	Size OSDRadioButton::getAutoSize()
	{
		Size textSize = UITools::getTextSize(ref().text(), ref().font().desc());
		//pre-calculated
		return Size(textSize.width() + 24, textSize.height() + 24);
	}

	LRESULT OSDRadioButton::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NOTIFY) {
			int code = ((LPNMHDR)lParam)->code;

			if (code == NM_CUSTOMDRAW) {
				//comctl32 6.0
				LPNMCUSTOMDRAW lpNMCustomDraw = (LPNMCUSTOMDRAW)lParam;
				switch (lpNMCustomDraw->dwDrawStage) {
				case CDDS_PREPAINT:
				{
					::SetBkColor(lpNMCustomDraw->hdc, RGB(0, 0, 0));
					return CDRF_NEWFONT;
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

		if (msg == WM_CTLCOLORSTATIC) {
			if (ref().transparent() && ref().parent()) {
				HBRUSH hBrush = ref().parent()->osdRef().backgroundBrush();
				if (hBrush)
					return (LRESULT)hBrush;
			}

			return (LRESULT)(m_backBrush ? m_backBrush : ::GetStockObject(WHITE_BRUSH));
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

		return ::CallWindowProc(m_originalWndProc, m_hwnd, msg, wParam, lParam);
	}


	void OSDRadioButton::setChecked(bool checked)
	{
		::SendMessage(m_hwnd, BM_SETCHECK, (WPARAM)(checked != false), (LPARAM)0);
	}

	u32 OSDRadioButton::radius()
	{
		return 12 / 2;
	}

}