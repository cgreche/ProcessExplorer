
// Status Bar
// File: statusbar.cpp
// Last edit: 01/10/2013 07:26 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	StatusBar::StatusBar() :Control(*new OSDStatusBar(*this))
	{
		m_parent = NULL;
		m_visible = false;
		m_text = "";
		m_margin = Rect(4, 0, 4, 0);
		m_color = Color(0, 0, 0);
		m_format = 0;

		osdRef().create();
		setDefaultFont();
	}

	StatusBar::~StatusBar()
	{
	}

	void StatusBar::setText(const string &text)
	{
		m_text = text;
		osdRef().setText(text);
	}

	void StatusBar::setTextColor(const Color &c)
	{
		m_color = c;
		osdRef().setTextColor(c);
	}

	void StatusBar::setMargin(const Rect &margin)
	{
		m_margin = margin;
		osdRef().setMargin(margin);
	}


	void StatusBar::setFormat(u32 format)
	{
		m_format = format;
		osdRef().setFormat(format);
	}

}