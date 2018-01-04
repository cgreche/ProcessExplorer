
//TextLink
// File: textlink.cpp
// Last edit: 30/06/2017 01:00 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	TextLink::TextLink()
		: Label(*new OSDTextLink(*this))
	{
		m_visitcount = 0;
		m_properties = (Properties)(Hover | Pushed);
		m_onClick = NULL;

		m_overColor = m_visitedColor = m_pushedColor = m_textColor;
		FontDesc defFontDesc = m_font.desc();
		m_overFont.set(defFontDesc);
		m_visitedFont.set(defFontDesc);
		m_pushedFont.set(defFontDesc);

		//
		osdRef().create();
		setDefaultFont();
	}

	void TextLink::setHoverColor(const Color &color)
	{
		m_overColor = color;
		osdRef().setHoverColor(color);
	}

	void TextLink::setHoverFont(const FontDesc &fd)
	{
		m_overFont.set(fd);
		osdRef().setHoverFont(fd);
	}

	void TextLink::setVisitedColor(const Color &color)
	{
		m_visitedColor = color;
		osdRef().setVisitedColor(color);
	}

	void TextLink::setVisitedFont(const FontDesc &fd)
	{
		m_visitedFont.set(fd);
		osdRef().setVisitedFont(fd);
	}

	void TextLink::setProperties(TextLink::Properties properties)
	{
		m_properties = properties;
		osdRef().setProperties(properties);
	}

}