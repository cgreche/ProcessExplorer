
//VerticalLayout
// File: verticallayout.h
// Last edit: 18/06/2017 00:51 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __VERTICALLAYOUT_H__
#define __VERTICALLAYOUT_H__

namespace uilib {

	class VerticalLayout : public LinearLayout
	{
	public:
		VerticalLayout();
		~VerticalLayout();

		virtual void setSize(const Size &size);
		virtual Size getAutoSize();
	};

}

#endif