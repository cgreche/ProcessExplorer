
//Group Box
// File: groupbox.cpp
// Last edit: 05/08/2014 01:27 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	GroupBox::GroupBox() : Control(*new OSDGroupBox(*this))
	{
		osdRef().create();
		setDefaultFont();
	}

	void GroupBox::setText(const string &text)
	{
		m_text = text;
		osdRef().setText(text);
	}

}