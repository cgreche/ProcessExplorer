
//Radio Button
// File name: radiobutton.cpp
// Last edit: 03/01/2014 13:20 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	RadioButton::RadioButton() :Button(*new OSDRadioButton(*this))
	{
		m_checked = false;
		osdRef().create();
		setDefaultFont();
	}


	Size RadioButton::getAutoSize()
	{
		return osdRef().getAutoSize();
	}


	void RadioButton::setChecked(bool checked)
	{
		m_checked = checked;
		osdRef().setChecked(checked);
	}

	bool RadioButton::checked() const
	{
		return m_checked;
	}

	u32 RadioButton::radius()
	{
		return osdRef().radius();
	}

}