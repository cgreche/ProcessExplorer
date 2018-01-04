
//OSD Scrollbar
// File: osdscrollbar.cpp
// Last edit: 18/12/2017 04:28 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"
#include <math.h>

namespace uilib {

	void OSDScrollBar::create()
	{
		createWindow(*this, WC_SCROLLBAR, 0, WS_TABSTOP | (ref().orientation() == ScrollBar::Vertical ? SBS_VERT : SBS_HORZ));
	}

	void OSDScrollBar::update()
	{
		OSDControl::update();
		setScrollLength(ref().scrollLength());
		setPageSize(ref().pageSize());
		setScrollPosition(ref().scrollPosition());
	}

	void OSDScrollBar::setScrollLength(int length)
	{
		SCROLLINFO scrollinfo;
		scrollinfo.cbSize = sizeof(SCROLLINFO);
		scrollinfo.fMask = SIF_RANGE;
		scrollinfo.nMin = 0;
		scrollinfo.nMax = length > 0 ? length - 1 : 0;
		::SetScrollInfo(m_hwnd, SB_CTL, &scrollinfo, TRUE);

		scrollinfo.fMask = SIF_POS;
		::GetScrollInfo(m_hwnd, SB_CTL, &scrollinfo);
		this->ref().m_scrollPos = scrollinfo.nPos;
	}

	void OSDScrollBar::setPageSize(int pageSize)
	{
		SCROLLINFO scrollinfo;
		scrollinfo.cbSize = sizeof(SCROLLINFO);
		scrollinfo.fMask = SIF_PAGE;
		scrollinfo.nPage = pageSize;
		::SetScrollInfo(m_hwnd, SB_CTL, &scrollinfo, TRUE);
	}


	void OSDScrollBar::setScrollPosition(int pos)
	{
		SCROLLINFO scrollinfo;
		scrollinfo.cbSize = sizeof(SCROLLINFO);
		scrollinfo.fMask = SIF_POS;
		scrollinfo.nPos = pos;
		SetScrollInfo(m_hwnd, SB_CTL, &scrollinfo, TRUE);
		GetScrollInfo(m_hwnd, SB_CTL, &scrollinfo);
		this->ref().m_scrollPos = scrollinfo.nPos;
	}

	Size OSDScrollBar::getAutoSize()
	{
		if (ref().orientation() == ScrollBar::Vertical) {
			int w = ::GetSystemMetrics(SM_CXVSCROLL);
			return Size(w, SizeTypeMax);
		}
		else {
			int h = ::GetSystemMetrics(SM_CXHSCROLL);
			return Size(SizeTypeMax,h);
		}
	}

	LRESULT OSDScrollBar::HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam)
	{
		if (msg == SBM_SETSCROLLINFO) {
			int a = 1;
		}
		if (msg == WM_VSCROLL || msg == WM_HSCROLL) {

			if (LOWORD(wParam) == SB_ENDSCROLL)
				return 0;
			//windows may have clamp'd the position
			SCROLLINFO si;
			si.cbSize = sizeof(SCROLLINFO);
			si.fMask = SIF_POS|SIF_PAGE| SIF_TRACKPOS;
			::GetScrollInfo(m_hwnd, SB_CTL, &si);
			this->ref().m_scrollPos = si.nPos;


			int oldPos = si.nPos;
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
			::SetScrollInfo(m_hwnd, SB_CTL, &si, TRUE);
			::GetScrollInfo(m_hwnd, SB_CTL, &si);

			if (oldPos != si.nPos) {
				std::vector<ScrollFunc> &callbackList = this->ref().m_onScrollCallbackList;
				for (int i = 0; i < callbackList.size(); ++i)
					(*callbackList[i])(this->ref(), oldPos, si.nPos);

			}

			/*
			Return value

			If an application processes this message, it should return zero.
			*/

			return 0;
		}
		return ::DefWindowProc(ref().parent()->osdRef().handle(), msg, wParam, lParam);
	}

}