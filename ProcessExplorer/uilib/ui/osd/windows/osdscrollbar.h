//OSD Scrollbar
// File: osdscrollbar.h
// Last edit: 18/12/2017 04:32 (UTC-3)
// Author: CGR

#ifndef __OSDSCROLLBAR_H__
#define __OSDSCROLLBAR_H__

namespace uilib {
	
	class OSDScrollBar : public OSDControl
	{
		virtual LRESULT HandleParentMessage(UINT msg, WPARAM wParam, LPARAM lParam);

	public:
		friend class ScrollBar;
		OSDScrollBar(ScrollBar &ref) : OSDControl(ref) { }
		ScrollBar& ref() const { return static_cast<ScrollBar&>(OSDControl::ref()); }

		void setScrollLength(int length);
		void setPageSize(int pageSize);
		void setScrollPosition(int pos);

		//OSDUIObj methods
		virtual void create();
		virtual void update();

		//OSDControl methods
		virtual Size getAutoSize();
	};

}

#endif