
//OSD ScrollView
// File: osdscrollview.cpp
// Last edit: 25/10/2017 05:57 (UTC-3)
// Author: CGR

#include "../../uibase.h"

namespace uilib {

	OSDScrollView::OSDScrollView(ScrollView &ref)
		: OSDControl(ref)
	{
	}

	void OSDScrollView::create()
	{
		createWindow(*this, WC_STATIC, 0, WS_TABSTOP | SBS_VERT);
	}

	void OSDScrollView::update()
	{
		OSDControl::update();
	}

	Size OSDScrollView::getAutoSize()
	{
		if (m_hwnd) {
			return this->ref().parent()->size();
		}
		return Size(0, 0);
	}

}