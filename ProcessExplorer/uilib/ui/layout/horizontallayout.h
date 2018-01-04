
//HorizontalLayout
// File: horizontallayout.h
// Last edit: 16/06/2017 02:43 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __HORIZONTALLAYOUT_H__
#define __HORIZONTALLAYOUT_H__

namespace uilib {

	class HorizontalLayout : public LinearLayout
	{
	public:
		HorizontalLayout();
		~HorizontalLayout();

		virtual void setSize(const Size &size);
		virtual Size getAutoSize();
	};

}

#endif