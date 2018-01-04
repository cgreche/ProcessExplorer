
//Radio Button
// File: button.h
// Last edit: 13/10/2013 08:08 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __RADIOBUTTON_H__
#define __RADIOBUTTON_H__

namespace uilib {

	class OSDRadioButton;

	class RadioButton : public Button
	{
	protected:

		bool m_checked;

	public:
		RadioButton();

		//Control
		virtual Size getAutoSize();

		//RadioButton
		void setChecked(bool checked);
		bool checked() const;
		u32 radius();

		friend class OSDRadioButton;
		OSDRadioButton& osdRef() const { return reinterpret_cast<OSDRadioButton&>(Button::osdRef()); }
	};

}

#endif