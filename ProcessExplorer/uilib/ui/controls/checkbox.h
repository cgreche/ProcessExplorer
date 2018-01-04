
//Check Box
// File: checkbox.h
// Last edit: 03/01/2014 13:20 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __CHECKBOX_H__
#define __CHECKBOX_H__

namespace uilib {

	class CheckBox : public Button
	{
	protected:
		bool m_checked;

	public:
		CheckBox();

		//Control
		virtual Size getAutoSize();

		//CheckBox
		void setChecked(bool checked);
		bool checked() const;

		friend class OSDCheckBox;
		OSDCheckBox& osdRef() const { return reinterpret_cast<OSDCheckBox&>(Button::osdRef()); }
	};

}

#endif