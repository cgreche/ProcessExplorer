
//OSDStatusBar
// File name: osdstatusbar.cpp
// Last edit: 05/12/2015 22:18 (UTC-3)
// Author: CGR


#include "..\\..\\uibase.h"

namespace uilib {

	void OSDStatusBar::onPaintEvent(PaintEvent& event)
	{
		UINT style, exstyle;
		HWND hwnd = m_hwnd;

		style = (UINT)GetWindowLongPtr(hwnd, GWL_STYLE);
		exstyle = (UINT)GetWindowLongPtr(hwnd, GWL_EXSTYLE);
		RECT rc;
		::GetClientRect(hwnd, &rc);
		//		AdjustWindowRectEx(&rc,style,FALSE,exstyle);

		PAINTSTRUCT ps;
		::BeginPaint(hwnd, &ps);
		//	FillRect(ps.hdc, &rc, (HBRUSH)GetStockObject(WHITE_BRUSH));

		u32 drawflags;
		u32 format = ref().format();

		rc.left += ref().margin().x1();
		rc.right -= ref().margin().x2();

		if (!(format & format_vcenter)) {
			rc.top += ref().margin().y1();
			rc.bottom -= ref().margin().y2();
		}

		drawflags = DT_SINGLELINE;
		if (format & format_center)
			drawflags |= DT_CENTER;
		if (format & format_vcenter)
			drawflags |= DT_VCENTER;

		if (ref().font().osdRef().object())
			::SelectObject(ps.hdc, ref().font().osdRef().object());

		Color textcolor = ref().textcolor();
		::SetTextColor(ps.hdc, RGB(textcolor.red(), textcolor.green(), textcolor.blue()));
		::SetBkMode(ps.hdc, TRANSPARENT);
		::DrawText(ps.hdc, ref().text().data(), -1, &rc, drawflags);
		::EndPaint(hwnd, &ps);
		return;
	}





	void OSDStatusBar::create()
	{
		extern void createWindow(OSDControl &osdControl, const TCHAR *windowClass, DWORD exStyle, DWORD style);
		createWindow(*this, STATUSCLASSNAME, 0, 0);
		//	::SetWindowLongPtr(m_hstatus, GWL_STYLE, GetWindowLong(m_hstatus, GWL_STYLE) | WS_DISABLED);
	}

	void OSDStatusBar::update()
	{
		OSDControl::update();
		setText(ref().text());
		setTextColor(ref().textcolor());
		setMargin(ref().margin());
		setFormat(ref().format());
	}

	void OSDStatusBar::setText(const string &text)
	{
		::InvalidateRect(m_hwnd, NULL, TRUE);
	}


	void OSDStatusBar::setTextColor(const Color &color)
	{
		::InvalidateRect(m_hwnd, NULL, TRUE);
	}

	void OSDStatusBar::setMargin(const Rect &margin)
	{
		::InvalidateRect(m_hwnd, NULL, TRUE);
	}

	void OSDStatusBar::setFont(const FontDesc &desc)
	{
		::SendMessage(m_hwnd, WM_SETFONT, (WPARAM)ref().font().osdRef().object(), (LPARAM)TRUE);
	}

	void OSDStatusBar::setFormat(u32 format)
	{
		::InvalidateRect(m_hwnd, NULL, TRUE);
	}

}