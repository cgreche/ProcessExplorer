
//Group Box
// File: groupbox.h
// Last edit: 23/07/2014 10:59 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __GROUPBOX_H__
#define __GROUPBOX_H__

namespace uilib {

	class GroupBox : public Control
	{
	protected:
		string m_text;

	public:
		GroupBox();

		void setText(const string &text);
		void settextColor(const Color &color);

		const string& text() const { return m_text; }

		//
		friend class OSDGroupBox;
		OSDGroupBox& osdRef() const { return reinterpret_cast<OSDGroupBox&>(Control::osdRef()); }
	};

}

#endif