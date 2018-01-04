
//OSDGroupBox
// File: osdgroupbox.cpp
// Last edit: 24/07/2014 08:59 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	LRESULT OSDGroupBox::OnEraseBkgnd(WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_ERASEBKGND, wParam, lParam);
	}

	LRESULT OSDGroupBox::OnPaint(WPARAM wParam, LPARAM lParam)
	{
		HDC hdc = ::GetDC(m_hwnd);
		//	Control *me = &ref();
		//	Control *parent = ref().parent();
		//	bool isTranparent = me->transparent();
		RECT rc;
		::GetUpdateRect(m_hwnd, &rc, FALSE);
		//	::GetClientRect(m_hwnd,&rc);
		::FillRect(hdc, &rc, (HBRUSH)m_backBrush);
		::ReleaseDC(m_hwnd, hdc);

		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_PAINT, wParam, lParam);
	}



	LRESULT OSDGroupBox::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_CTLCOLORSTATIC) {
			return (LRESULT)m_backBrush;
		}

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

		return ::DefWindowProc(ref().parent()->osdRef().handle(), msg, wParam, lParam);
	}



	OSDGroupBox::OSDGroupBox(GroupBox &ref) : OSDControl(ref)
	{

	}

	void OSDGroupBox::create()
	{
		createWindow(*this, TEXT("button"), 0, BS_GROUPBOX);
	}

	void OSDGroupBox::update()
	{
		OSDControl::update();
		setText(ref().m_text.data());
	}



	void OSDGroupBox::setText(const string& text)
	{
		::SetWindowText(m_hwnd, text.data());
	}

}