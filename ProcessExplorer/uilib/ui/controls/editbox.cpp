
//EditBox
// File: editbox.cpp
// Last edit: 03/07/2017 00:42 (UTC-3)
// Author: CGR

#include "..\\uibase.h"

namespace uilib {

	EditBox::EditBox() :Control(*new OSDEditBox(*this))
	{
		m_receivebuffer = &m_text;
		m_textColor = Color(0, 0, 0);
		m_maxChars = UNLIMITEDCHARS;
		m_editable = true;
		//m_placeholderTextColor = Color(199, 199, 255);
		m_placeholderTextColor = Color(66, 66, 66);
		m_placeholderTextBehavior = HIDE_ON_FIRST_CHAR;
		m_onChange = NULL;
		m_passwordMode = false;
		m_style = 1;

		osdRef().create();
		setDefaultFont();
	}

	EditBox::EditBox(OSDEditBox& ref) : Control(ref)
	{
		m_receivebuffer = &m_text;
		m_textColor = Color(0, 0, 0);
		m_maxChars = UNLIMITEDCHARS;
		m_editable = true;
		m_placeholderTextColor = Color(66, 66, 66);
		m_placeholderTextBehavior = HIDE_ON_FIRST_CHAR;
		m_onChange = NULL;
		m_passwordMode = false;
		m_style = 1;
	}

	Size EditBox::getAutoSize()
	{
		Size textSize = UITools::getTextSize(" ", m_font.desc());
		return Size(SizeTypeMax, textSize.height() + 8);
	}

	void EditBox::setText(const string &text)
	{
		*m_receivebuffer = text;
		osdRef().setText(text);
	}

	void EditBox::setReceiveBuffer(string *pBuffer)
	{
		if (pBuffer == NULL) pBuffer = &m_text;
		//if setting same buffer as before, return
		if (m_receivebuffer == pBuffer) return;
		m_receivebuffer = pBuffer;
		osdRef().setText(*pBuffer); //update the text
	}

	void EditBox::setTextColor(const Color &color)
	{
		m_textColor = color;
		osdRef().setTextColor(color);
	}

	void EditBox::setMaxChars(uint maxChars)
	{
		m_maxChars = maxChars;
		osdRef().setMaxChars(maxChars);
	}

	void EditBox::setEditable(bool editable)
	{
		m_editable = editable;
		osdRef().setEditable(editable);
	}

	void EditBox::setPlaceholderText(const string &text)
	{
		m_placeholderText = text;
		osdRef().setPlaceholderText(text);
	}

	void EditBox::setPlaceholderTextColor(const Color &color)
	{
		m_placeholderTextColor = color;
		osdRef().setPlaceholderTextColor(color);
	}

	void EditBox::setPlaceholderTextBehavior(PlaceholderTextBehavior behavior)
	{
		m_placeholderTextBehavior = behavior;
		osdRef().setPlaceholderTextBehavior(behavior);
	}

	void EditBox::setPasswordMode(bool passwordMode)
	{
		m_passwordMode = passwordMode;
		osdRef().setPasswordMode(passwordMode);
	}

	void EditBox::update()
	{
		osdRef().setText(*m_receivebuffer);
	}

	void EditBox::setOnChange(EditBoxFunc onChange)
	{
		m_onChange = onChange;
	}

}