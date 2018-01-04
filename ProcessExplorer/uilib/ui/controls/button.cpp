
//Button
// File name: button.cpp
// Last edit: 01/08/2014 01:59 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	Button::Button() :Control(*new OSDButton(*this))
	{
		m_onClick = NULL;

		osdRef().create();
		setDefaultFont();
	}

	Button::Button(OSDButton& ref) :Control(ref)
	{
		m_onClick = NULL;
	}


	void Button::setText(const string &text)
	{
		m_text = text;
		osdRef().setText(text);
	}

	void Button::setTextColor(const Color &color)
	{
		m_textColor = color;
		osdRef().setTextColor(color);
	}



	Size Button::getAutoSize()
	{
		Size textSize;
		if (m_text == "") {
			textSize = UITools::getTextSize(" ", m_font.desc());
			textSize.setWidth(80);
		}
		else
			textSize = UITools::getTextSize(m_text, m_font.desc());

		return Size(textSize.width() + 8, textSize.height() + 8);
	}

}