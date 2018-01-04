
//Label
// File: label.h
// Last Edit: 30/06/2017 01:03 (UTC-3)
// Author: CGR

#ifndef __UILIB_LABEL_H__
#define __UILIB_LABEL_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

namespace uilib {

	class Label;

	class Label : public Control
	{
	public:
		enum Alignment {
			Left = 0x00,
			Right = 0x01,
			HCenter = 0x02,
			Justify = 0x04,

			Top = 0x00,
			Bottom = 0x10,
			VCenter = 0x20,

			Center = HCenter | VCenter
		};
	protected:
		string m_text;
		Color m_textColor;
		int m_spacing;
		int m_justification;
		uint m_alignment;

		Label(OSDLabel& ref);

	public:
		Label();

		virtual Size getAutoSize();

		string &text() { return m_text; }
		Color textColor() const { return m_textColor; }
		int spacing() const { return m_spacing; }
		int justification() const { return m_justification; }
		uint alignment() const { return m_alignment; }

		void setText(const string &text);
		void setTextColor(const Color &color);
		void setSpacing(int spacing);
		void setJustification(int justification);
		void setAlignment(uint alignment);

		Size calcTextSize();

		friend class OSDLabel;
		OSDLabel& osdRef() const { return reinterpret_cast<OSDLabel&>(Control::osdRef()); }
	};

}

#endif