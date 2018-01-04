
//OSDLabel
// File: osdlabel.cpp
// Last edit: 08/10/2017 08:13 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	WNDPROC g_originalstaticproc = 0;


	OSDLabel::OSDLabel(Label &ref) :OSDControl(ref)
	{

	}

	void OSDLabel::create()
	{
		createWindow(*this, TEXT("static"), 0, 0);
	}

	void OSDLabel::update()
	{
		OSDControl::update();
		setText(ref().text());
	}


	void OSDLabel::setText(const string& text)
	{
		::SetWindowText(m_hwnd, text.data());

		/*
		if(ref().m_transparent && ref().m_parent) {
			HWND parent = ref().parent()->osdRef().handle();

			RECT winrect;
			RECT labelrc;
			GetWindowRect(parent,&winrect);
			GetWindowRect(m_hwnd,&labelrc);
			labelrc.left -= winrect.left;
			labelrc.right -= winrect.left;
			labelrc.top -= winrect.top;
			labelrc.bottom -= winrect.top;
			::InvalidateRect(parent,&labelrc,TRUE);
		}
		*/

		::InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void OSDLabel::setTextColor(const Color &color)
	{
		redraw();
	}

	void OSDLabel::setSpacing(int spacing)
	{
		redraw();
	}

	void OSDLabel::setJustification(int justification)
	{
		redraw();
	}

	void OSDLabel::setAlignment(int alignment)
	{
		redraw();
	}



	Size OSDLabel::calcTextSize()
	{
		//	SIZE size;
		RECT rc = { 0,0,0,0 };
		HDC hdc = ::GetDC(m_hwnd);
		HFONT hFontOld = (HFONT)::SelectObject(hdc, ref().font().osdRef().object());
		::DrawText(hdc, ref().m_text.data(), -1, &rc, DT_CALCRECT);
		//	::GetTextExtentPoint32(hdc,ref().m_text(),ref().m_text.len(),&size);
		::SelectObject(hdc, hFontOld);
		::ReleaseDC(m_hwnd, hdc);
		return Size(rc.right, rc.bottom);
	}


	LRESULT OSDLabel::OnEnable(WPARAM wParam, LPARAM lParam)
	{
		this->redraw();
		return 0;
	}


	void OSDLabel::onPaintEvent(PaintEvent &event)
	{
		PAINTSTRUCT ps;
		RECT rc = { 0,0,0,0 };
		HDC hdc;
		HWND hwnd;

		hwnd = m_hwnd;
		hdc = ::BeginPaint(hwnd, &ps);
		::GetClientRect(hwnd, &rc);

		Color facecolor;
		Color bgcolor;

		Size textSize;
		Label &label = this->ref();
		uint alignment = label.alignment();

		if (label.enabled())
			facecolor = label.textColor();
		else {
			DWORD sysc = ::GetSysColor(COLOR_GRAYTEXT);
			facecolor = Color(GetRValue(sysc), GetGValue(sysc), GetBValue(sysc));
		}

		//Set background color
		if (label.transparent() && label.parent()) {
			bgcolor = label.parent()->backgroundColor();
		}
		else {
			bgcolor = label.backgroundColor();
		}

		//manually draw the background
		HBRUSH hBrush = ::CreateSolidBrush(RGB(bgcolor.red(), bgcolor.green(), bgcolor.blue()));
		::FillRect(hdc, &rc, hBrush);
		::DeleteObject(hBrush);

		//Set text color/font/extras
		::SetBkMode(hdc, TRANSPARENT);
		::SetTextColor(hdc, RGB(facecolor.red(), facecolor.green(), facecolor.blue()));
		::SelectObject(hdc, label.font().osdRef().object());
		::SetTextCharacterExtra(hdc, label.spacing());
		//Draw text
		UINT format = DT_LEFT | DT_TOP;
		if (alignment & Label::Right) format |= DT_RIGHT;
		if (alignment & Label::HCenter) format |= DT_CENTER;
		if (alignment & Label::Bottom) format |= DT_BOTTOM;
		format |= DT_WORDBREAK;
		RECT textRect = rc;
		::DrawText(hdc, label.text().data(), -1, &textRect, format | DT_CALCRECT);
		if (alignment & Label::VCenter)
			rc.top = ((rc.bottom - rc.top) - (textRect.bottom - textRect.top)) / 2;
		::DrawText(hdc, label.text().data(), -1, &rc, format);
		::EndPaint(hwnd, &ps);
	}

	LRESULT OSDLabel::OnNCPaint(WPARAM wParam, LPARAM lParam)
	{
		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_NCPAINT, wParam, lParam);
	}

	LRESULT OSDLabel::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_CTLCOLORSTATIC) {
			//return (LRESULT)::GetStockObject(NULL_BRUSH);
		}

		return 0;
	}

	LRESULT OSDLabel::OnNCHitTest(WPARAM wParam, LPARAM lParam)
	{
		// static controls return HTTRANSPARENT (prevents receiving mouse msgs), so change to HTCLIENT
		return HTCLIENT;
	}








	void OSDSeparator::create()
	{
		createWindow(*this, TEXT("static"), 0, 0);
		//	::SetWindowLong(m_hwnd,GWL_WNDPROC,(LONG)testProc);
		//	m_wndProc = testProc;
	}

	void OSDSeparator::update()
	{
		OSDControl::update();
	}

	void OSDSeparator::setLineThickness(u32 thickness)
	{
		redraw();
	}

	void OSDSeparator::setLineColor(const Color &color)
	{
		redraw();
	}


	LRESULT OSDSeparator::OnEnable(WPARAM wParam, LPARAM lParam)
	{
		//do nothing
		return 0;
	}

	void OSDSeparator::onPaintEvent(PaintEvent &event)
	{
		HWND hwnd;
		HDC hdc;
		PAINTSTRUCT ps;

		RECT rcitem, rcline, rctext;
		SIZE text_size = { 0 };
		int nSpaceWidth = 0;
		const char *text;

		hwnd = m_hwnd;
		hdc = ::BeginPaint(hwnd, &ps);
		::GetClientRect(hwnd, &rcitem);

		Separator& separator = ref();

		Color linecolor = separator.lineColor();
		Color textcolor = separator.textColor();
		Color backcolor;
		text = separator.text().data();

		HPEN newPen = ::CreatePen(PS_SOLID, separator.thickness(), RGB(linecolor.red(), linecolor.green(), linecolor.blue()));
		HPEN oldPen = (HPEN)::SelectObject(hdc, newPen);

		//Set background mode/color
		if (separator.transparent() && separator.parent()) {
			::SetBkMode(hdc, TRANSPARENT);

			backcolor = separator.parent()->backgroundColor();
			HBRUSH hBrush = ::CreateSolidBrush(RGB(backcolor.red(), backcolor.green(), backcolor.blue()));
			::FillRect(hdc, &rcitem, hBrush);
			::DeleteObject(hBrush);
		}
		else {
			backcolor = separator.backgroundColor();
			::SetBkMode(hdc, OPAQUE);
			::SetBkColor(hdc, RGB(backcolor.red(), backcolor.green(), backcolor.blue()));
		}

		::SelectObject(hdc, separator.font().osdRef().object());
		::SetTextColor(hdc, RGB(textcolor.red(), textcolor.green(), textcolor.blue()));

		::SetTextCharacterExtra(hdc, separator.spacing());
		::SetTextJustification(hdc, separator.justification(), 0);

		if (*text)
		{
			::DrawText(hdc, text, strlen(text), &rctext, DT_CALCRECT);
			::GetTextExtentPoint32(hdc, text, strlen(text), &text_size);

			//get 'space' width
			::GetCharWidth32(hdc, (UINT)0x20, (UINT)0x20, &nSpaceWidth);
		}
		else
		{
			::DrawText(hdc, " ", 1, &rctext, DT_CALCRECT);
		}


		rcline = rcitem;
		rcline.top = rcline.bottom = (rctext.bottom - rctext.top) / 2;



		//make the line
		//make the first
		::MoveToEx(hdc, rcline.left, rcline.top, NULL);
		::LineTo(hdc, 10, rcline.top);
		//make the second
		::MoveToEx(hdc, 10 + (nSpaceWidth * 2) + text_size.cx, rcline.top, NULL);
		::LineTo(hdc, rcline.right, rcline.top);


		//draw the text
		rcitem.left += 10 + nSpaceWidth;
		::DrawText(hdc, text, strlen(text), &rcitem, DT_LEFT);


		//clean up
		::SelectObject(hdc, oldPen);
		::DeleteObject(newPen);
		::EndPaint(hwnd, &ps);
	}


	LRESULT OSDSeparator::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_CTLCOLORSTATIC) {
			//	this->redraw();
			//	return 0;//(LRESULT)::GetStockObject(NULL_BRUSH);
		}

		return 0;
	}

	LRESULT OSDTextLink::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == WM_CTLCOLORSTATIC) {
			//	this->redraw();
			//	return 0;//(LRESULT)::GetStockObject(NULL_BRUSH);
		}

		return 0;
	}






	void OSDTextLink::create()
	{
		createWindow(*this, TEXT("static"), 0, WS_TABSTOP);

		m_bClick = false;
		m_bHover = false;
	}

	void OSDTextLink::update()
	{
		OSDLabel::update();
	}




	void OSDTextLink::setHoverColor(const Color &color)
	{
		::InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void OSDTextLink::setHoverFont(const FontDesc &fd)
	{
		::InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void OSDTextLink::setVisitedColor(const Color &color)
	{
		::InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void OSDTextLink::setVisitedFont(const FontDesc &fd)
	{
		::InvalidateRect(m_hwnd, NULL, FALSE);
	}

	void OSDTextLink::setProperties(TextLink::Properties props)
	{
		if (ref().m_properties != props)
			::InvalidateRect(m_hwnd, NULL, FALSE);
	}


	LRESULT OSDTextLink::OnKillFocus(WPARAM wParam, LPARAM lParam)
	{
		::InvalidateRect(m_hwnd, NULL, TRUE);
		return ::CallWindowProc(m_originalWndProc, m_hwnd, WM_KILLFOCUS, wParam, lParam);
	}

	void OSDTextLink::onPaintEvent(PaintEvent &event)
	{
		PAINTSTRUCT ps;
		RECT rc;
		HDC hdc;
		HWND hwnd;

		TextLink &link = ref();

		if (!link.enabled())
			return;

		hwnd = m_hwnd;
		hdc = ::BeginPaint(hwnd, &ps);
		::GetClientRect(hwnd, &rc);
		//	::FillRect(hdc, &rc, GetSysColorBrush(COLOR_3DFACE));

		TextLink::Properties props = link.properties();
		Color facecolor;
		Color bgcolor;

		Font *font;
		if (props & TextLink::Hover && m_bHover)
		{
			if (props & TextLink::Pushed && m_bClick)
			{
				facecolor = link.pushedColor();
				font = &link.pushedFont();
			}
			else
			{
				facecolor = link.hoverColor();
				font = &link.hoverFont();
			}
		}
		else
		{
			if (props & TextLink::Visited && link.visited())
			{
				facecolor = link.visitedColor();
				font = &link.visitedFont();
			}
			else
			{
				facecolor = link.textColor();
				font = &link.font();
			}
		}

		//Set background mode/color
		if (link.transparent() && link.parent()) {
			bgcolor = link.parent()->backgroundColor();
		}
		else {
			bgcolor = link.backgroundColor();
		}

		HBRUSH hBrush = ::CreateSolidBrush(RGB(bgcolor.red(), bgcolor.green(), bgcolor.blue()));
		::FillRect(hdc, &rc, hBrush);
		::DeleteObject(hBrush);

		//Set text color/font/extras
		::SetBkMode(hdc, TRANSPARENT);
		::SetTextColor(hdc, RGB(facecolor.red(), facecolor.green(), facecolor.blue()));
		::SelectObject(hdc, font->osdRef().object());
		::SetTextCharacterExtra(hdc, link.spacing());

		//Draw text
		::DrawText(hdc, link.text().data(), -1, &rc, DT_LEFT);
		if (::GetFocus() == hwnd) {
			// it seems, that microsoft failed to mention:
			// DrawFocusRect is using text and background color!
			::SetBkColor(hdc, GetSysColor(COLOR_3DFACE));
			::SetTextColor(hdc, GetSysColor(COLOR_BTNTEXT));
			//	::SetBkMode(ps.hdc,TRANSPARENT);
			//	::SetBkColor(ps.hdc,RGB(255,255,255));
			//	::SetTextColor(ps.hdc,RGB(0,0,0));
			::DrawFocusRect(hdc, &rc);
		}
		::EndPaint(hwnd, &ps);
	}

	LRESULT OSDTextLink::OnSetCursor(WPARAM wParam, LPARAM lParam)
	{
		return TRUE;
	}

	void OSDTextLink::onMouseMoveEvent(MouseEvent &event)
	{
		HWND hwnd;
		RECT rc = { 0,0,0,0 };

		hwnd = m_hwnd;

		TextLink& link = ref();
		Size size = link.size();
		rc.right = size.width();
		rc.bottom = size.height();

		//	::GetCursorPos(&pt);
		//	::ScreenToClient(hwnd,&pt);
		//	if(::PtInRect(&rc,pt)) {
		if (m_bHover == 1)
			return;
		m_bHover = 1;
		::InvalidateRect(hwnd, NULL, TRUE);

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.hwndTrack = hwnd;
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		::TrackMouseEvent(&tme);

		::SetCursor(::LoadCursor(NULL, IDC_HAND));
		//	}
		//	else
		//	{
		//		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
		//		if(m_bHover == 1)
		//			::PostMessage(hwnd,WM_MOUSELEAVE,0,0);
		//	}

		return;
	}

	void OSDTextLink::onMouseLeaveEvent(MouseEvent &event)
	{
		m_bHover = 0;
		::InvalidateRect(m_hwnd, NULL, TRUE);
	}

	void OSDTextLink::onMousePressEvent(MouseEvent &event) {
		::SetFocus(m_hwnd);
		::SetCapture(m_hwnd);
		::InvalidateRect(m_hwnd, NULL, TRUE);

		m_bClick = true;
	}

	void OSDTextLink::onMouseReleaseEvent(MouseEvent &event) {
		if (!m_bClick)
			return;

		::ReleaseCapture();
		RECT rc = { 0,0,0,0 };
		POINT pt;

		Size size = ref().size();
		rc.right = size.width();
		rc.bottom = size.height();

		::GetCursorPos(&pt);
		::ScreenToClient(m_hwnd, &pt);
		if (PtInRect(&rc, pt)) {
			ref().increaseVisits();
			if (ref().m_onClick)
				ref().m_onClick(ref());
		}

		m_bClick = false;
	}

}