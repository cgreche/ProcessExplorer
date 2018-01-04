
// ScrollView
// File: scrollview.h
// Last edit: 25/10/2017 05:37 (UTC-3)
// Author: CGR

#ifndef __UILIB_SCROLLVIEW_H__
#define __UILIB_SCROLLVIEW_H__

#include "control.h"
#include "scrollbar.h"

namespace uilib {

	class ScrollView : public Control
	{
		VerticalLayout dummyLayout;
	protected:

		bool m_showVerticalScrollBar;
		bool m_showHorizontalScrollBar;
		Size m_workArea;

		friend class OSDScrollView;
		OSDScrollView &osdRef() const { return reinterpret_cast<OSDScrollView&>(Control::osdRef()); }

	public:
		VerticalLayout viewLayout;
			HorizontalLayout hLayout;
				Layout *contentLayout;
				ScrollBar verticalScrollBar;
			ScrollBar horizontalScrollBar;

	public:
		ScrollView();
		~ScrollView();
		virtual void setLayout(Layout *layout);
		virtual void setSize(Size size);
	};

}

#endif