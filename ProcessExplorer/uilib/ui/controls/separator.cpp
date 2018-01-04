
//Separator
// File: separator.cpp
// Last edit: 30/06/2017 00:55 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	Separator::Separator() :Label(*new OSDSeparator(*this))
	{
		m_thickness = 1;
		m_lineColor = Color(0, 0, 0);

		//
		osdRef().create();
		setDefaultFont();
	}

	void Separator::setLineColor(const Color &color)
	{
		m_lineColor = color;
		osdRef().setLineColor(color);
	}

	void Separator::setLineThickness(u32 size)
	{
		m_thickness = size;
		osdRef().setLineThickness(size);
	}

	Size Separator::getAutoSize()
	{
		Size textSize;
		if (m_text == "") {
			textSize = UITools::getTextSize(" ", m_font.desc());
			return Size(SizeTypeMax, textSize.height());
		}
		textSize = this->calcTextSize();
		return Size(SizeTypeMax, textSize.height());
	}

}