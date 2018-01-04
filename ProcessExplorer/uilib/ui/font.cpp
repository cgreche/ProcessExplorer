
//Font
// File: font.cpp
// Last edit: 30/11/2014 04:34 (UTC-3)
// Author: CGR

#include "uibase.h"

namespace uilib {

	Font::Font(const FontDesc &desc)
		: m_osdRef(NULL)
	{
		set(desc);
	}

	Font::Font(const Font& font)
		: m_osdRef(&font.osdRef())
	{
		m_desc = ((Font&)font).desc();
		osdRef().addRef();
	}

	Font::Font()
		: m_osdRef(NULL)
	{
		set(FontDesc("Arial", 10, 0));
	}

	Font::~Font()
	{
		osdRef().release();
	}

	bool Font::set(const FontDesc &desc)
	{
		if (m_osdRef)
			m_osdRef->release();
		m_desc = desc;
		m_osdRef = new OSDFont(desc);
		return m_osdRef != NULL;
	}


	Font& Font::operator=(const Font& font)
	{
		m_osdRef->release();
		m_desc = font.desc();
		m_osdRef = &font.osdRef();
		m_osdRef->addRef();
		return *this;
	}

}