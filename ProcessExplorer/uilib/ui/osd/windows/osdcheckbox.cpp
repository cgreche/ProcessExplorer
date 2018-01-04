
//OSDCheckBox
// File: osdcheckbox.cpp
// Last edit: 13/12/2014 04:17 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	OSDCheckBox::OSDCheckBox(CheckBox &ref) : OSDButton(ref)
	{
	}

	void OSDCheckBox::create()
	{
		createWindow(*this, TEXT("button"), 0, WS_TABSTOP | BS_AUTOCHECKBOX);
	}

	void OSDCheckBox::update()
	{
		OSDButton::update();
		setChecked(ref().m_checked);
	}

	Size OSDCheckBox::getAutoSize()
	{
		//todo: size correctly
		Size textSize = UITools::getTextSize(ref().text(), ref().font().desc());
		Size boxSize = Size(13, 13);
		//pre-calculated
		return Size(boxSize.width() + textSize.width() / 20 + textSize.width(),
			max(textSize.height(), boxSize.height()));
	}

	LRESULT OSDCheckBox::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_NOTIFY) {
			int code = ((LPNMHDR)lParam)->code;

			if (code == NM_CUSTOMDRAW) {
				//comctl32 6.0
				LPNMCUSTOMDRAW lpNMCustomDraw = (LPNMCUSTOMDRAW)lParam;
				switch (lpNMCustomDraw->dwDrawStage) {
				case CDDS_PREPAINT:
				{
					//returning CDRF_NEWFONT doesn't make the desired result
					//CDRF_DODEFAULT or CDRF_NEWFONT doesn't draw the text with our new color
					Color textColor;
					Color bgColor = ref().backgroundColor();
					if (ref().enabled())
						textColor = ref().textColor();
					else {
						DWORD sysc = ::GetSysColor(COLOR_GRAYTEXT);
						textColor = Color(GetRValue(sysc), GetGValue(sysc), GetBValue(sysc));
					}

					::SetTextColor(lpNMCustomDraw->hdc, RGB(textColor.red(), textColor.green(), textColor.blue()));
					::SetBkColor(lpNMCustomDraw->hdc, RGB(bgColor.red(), bgColor.green(), bgColor.blue()));
					Size textSize = UITools::getTextSize(ref().text(), ref().font().desc());
					Size boxSize = Size(13, 13);
					u32 spacer = boxSize.width() + textSize.width() / 20;
					RECT rc = { 0 };
					rc.left = spacer;
					rc.top = 0;
					rc.right = lpNMCustomDraw->rc.right;
					rc.bottom = lpNMCustomDraw->rc.bottom;
					::DrawText(lpNMCustomDraw->hdc, ref().text().data(), -1, &rc, DT_LEFT);

					return CDRF_SKIPDEFAULT;
				}break;

				case CDDS_ITEMPREPAINT:
				{
					return CDRF_NEWFONT;
				};

				case CDDS_POSTPAINT:
				{
					return CDRF_SKIPDEFAULT;
				}break;

				case CDDS_PREERASE:
				{
					return CDRF_DODEFAULT;
				}break;

				default:
					int a = lpNMCustomDraw->dwDrawStage;
				}

			}

			return ::DefWindowProc(ref().parent()->osdRef().handle(), msg, wParam, lParam);
		}

		if (msg == WM_COMMAND) {
			//		uint code = HIWORD(wParam);
			//		if(code == 
			int checked = ::SendMessage(m_hwnd, BM_GETCHECK, 0, 0);
			//		if(checked == BST_CHECKED) {
			//		}
			//		else if(checked == BST_INDETERMINATE) {
						//todo: support tri-state checkbox
			//		}
					//else //BST_UNCHECKED

			ref().m_checked = checked != BST_UNCHECKED;
			if (ref().m_onClick)
				ref().m_onClick(ref());
			return FALSE;
		}

		if (msg == WM_CTLCOLORSTATIC) {
			Color textColor;
			if (ref().enabled())
				textColor = ref().textColor();
			else {
				DWORD sysc = ::GetSysColor(COLOR_GRAYTEXT);
				textColor = Color(GetRValue(sysc), GetGValue(sysc), GetBValue(sysc));
			}
			::SetTextColor((HDC)wParam, RGB(textColor.red(), textColor.green(), textColor.blue()));

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
		}

		return ::DefWindowProc(ref().parent()->osdRef().handle(), msg, wParam, lParam);
	}


	void OSDCheckBox::setChecked(bool checked)
	{
		::SendMessage(m_hwnd, BM_SETCHECK, checked != 0 ? BST_CHECKED : BST_UNCHECKED, (LPARAM)0);
	}

}