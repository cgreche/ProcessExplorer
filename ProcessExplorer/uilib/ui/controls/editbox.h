
//EditBox
// File: editbox.h
// Last edit: 02/07/2017 19:00 (UTC-3)
// Author: CGR

#ifndef __UILIB_EDITBOX_H__
#define __UILIB_EDITBOX_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

namespace uilib {

#define UNLIMITEDCHARS 0

	class EditBox;

	typedef void(*EditBoxFunc)(EditBox& editBox);

	/*
	class Observable
	{

	public:
		void addObserver();
	};
	*/

	enum PlaceholderTextBehavior
	{
		HIDE_ON_FIRST_CHAR,
		HIDE_ON_FOCUS,
		ALWAYS_SHOWN
	};

	class EditBox : public Control
	{

	protected:
		string m_text; //default buffer
		string *m_receivebuffer;
		Color m_textColor;
		uint m_maxChars;
		bool m_editable;

		string m_placeholderText;
		Color m_placeholderTextColor;
		PlaceholderTextBehavior m_placeholderTextBehavior;

		string mask;

		EditBoxFunc m_onChange;

		bool m_passwordMode;

	public:
		EditBox();
		EditBox(OSDEditBox& ref);

		//Control
		virtual Size getAutoSize();

		//
		void update();

		//Events setter
		void setOnChange(EditBoxFunc onChange);

		//Attributes setter
		void setText(const string &text);
		void setReceiveBuffer(string *pBuffer);
		void setTextColor(const Color &color);
		void setMaxChars(uint nChars);
		void setEditable(bool editable);
		void setPlaceholderText(const string &text);
		void setPlaceholderTextColor(const Color &color);
		void setPlaceholderTextBehavior(PlaceholderTextBehavior behavior);
		void setPasswordMode(bool passwordMode);

		//Getters
		string& text() { return *m_receivebuffer; }
		Color textColor() const { return m_textColor; }
		u32 maxChars() const { return m_maxChars; }
		bool editable() const { return m_editable; }
		const string& placeholderText() const { return m_placeholderText; }
		Color placeholderTextColor() const { return m_placeholderTextColor; }
		PlaceholderTextBehavior placeholderTextBehavior() const { return m_placeholderTextBehavior; }
		bool passwordMode() const { return m_passwordMode; }

		friend class OSDEditBox;
		OSDEditBox& osdRef() const { return reinterpret_cast<OSDEditBox&>(Control::osdRef()); }
	};

}

#endif