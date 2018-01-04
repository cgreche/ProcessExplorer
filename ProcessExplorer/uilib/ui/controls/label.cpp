
//Label
// File: label.cpp
// Last edit: 04/07/2017 01:10 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	Label::Label() :Control(*new OSDLabel(*this))
	{
		m_transparent = true;
		m_textColor = Color(0, 0, 0);
		m_spacing = 0;
		m_justification = 0;
		m_alignment = Label::Left;

		//
		osdRef().create();
		setDefaultFont();
	}

	Label::Label(OSDLabel &ref) :Control(ref)
	{
		m_transparent = true;
		m_textColor = Color(0, 0, 0);
		m_spacing = 0;
		m_justification = 0;
		m_alignment = Label::Left;
	}

	Size Label::getAutoSize()
	{
		return calcTextSize();
	}

	void Label::setText(const string &text)
	{
		m_text = text;
		osdRef().setText(text);
	}

	void Label::setTextColor(const Color &color)
	{
		m_textColor = color;
		osdRef().setTextColor(color);
	}

	void Label::setSpacing(int spacing)
	{
		m_spacing = spacing;
		osdRef().setSpacing(spacing);
	}

	void Label::setJustification(int justification)
	{
		m_justification = justification;
		osdRef().setJustification(justification);
	}

	void Label::setAlignment(uint alignment)
	{
		m_alignment = alignment;
		osdRef().setAlignment(alignment);
	}

	Size Label::calcTextSize()
	{
		return osdRef().calcTextSize();
	}

}
