
//Button
// File: button.h
// Last edit: 01/08/2014 01:35 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __BUTTON_H__
#define __BUTTON_H__

namespace uilib {

	class Button;
	typedef void(*ButtonFunc)(Button &button);

	class Button : public Control
	{
	protected:
		string m_text;
		Color m_textColor;
		ButtonFunc m_onClick;

	public:
		Button();
		Button(OSDButton& ref);

		void setOnClick(ButtonFunc onClick) { m_onClick = onClick; }
		void setText(const string &text);
		void setTextColor(const Color &color);

		const string& text() const { return m_text; }
		Color textColor() const { return m_textColor; }

		Size getAutoSize();

		friend class OSDButton;
		OSDButton& osdRef() const { return reinterpret_cast<OSDButton&>(Control::osdRef()); }
	};

}

#endif