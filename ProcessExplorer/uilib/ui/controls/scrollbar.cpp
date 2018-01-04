
// Scrollbar
// File: scrollbar.cpp
// Last Edit: 26/07/2014 22:14 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	ScrollBar::ScrollBar(Orientation orientation) : Control(*new OSDScrollBar(*this))
	{
		m_orientation = orientation;
		m_length = 0;
		m_scrollPos = 0;

		//
		osdRef().create();
	}

	void ScrollBar::setScrollLength(int length)
	{
		m_length = length;
		osdRef().setScrollLength(length);
	}

	void ScrollBar::setPageSize(int pageSize) {
		m_pageSize = pageSize;
		osdRef().setPageSize(pageSize);
	}

	void ScrollBar::setScrollPosition(int pos)
	{
		m_scrollPos = pos;
		osdRef().setScrollPosition(pos);
	}

	void ScrollBar::addOnScroll(ScrollFunc onScroll)
	{
		m_onScrollCallbackList.push_back(onScroll);
	}

	//virtual
	Size ScrollBar::getAutoSize()
	{
		return osdRef().getAutoSize();
	}

}
