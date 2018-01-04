
// ScrollView
// File: scrollview.cpp
// Last edit: 25/10/2017 05:37 (UTC-3)
// Author: CGR

#include "../uibase.h"

namespace uilib {

	void onVScroll(ScrollBar &scrollBar, int oldPos, int newPos) {
		ScrollView *scroll = (ScrollView*)scrollBar.param();
		Layout *contentLayout = scroll->contentLayout;
		if (contentLayout)
			contentLayout->setPosition(Point(contentLayout->position().x(), -newPos));
	}

	void onHScroll(ScrollBar &scrollBar, int oldPos, int newPos) {
		ScrollView *scroll = (ScrollView*)scrollBar.param();
		Layout *contentLayout = scroll->contentLayout;
		if (contentLayout)
			contentLayout->setPosition(Point(-newPos,contentLayout->position().y()));
	}

	ScrollView::ScrollView()
		: Control(*new OSDScrollView(*this))
		, horizontalScrollBar(ScrollBar::Horizontal) {
		contentLayout = &dummyLayout;

		verticalScrollBar.addOnScroll(onVScroll);
		verticalScrollBar.setParam(this);
		//verticalScrollBar.setStyle(verticalScrollBar.style() | CS_Border);
		horizontalScrollBar.addOnScroll(onHScroll);
		horizontalScrollBar.setParam(this);

		verticalScrollBar.setBackgroundColor(Color(255, 0, 0));

		hLayout.append(*contentLayout,MaximumSize,0);
		hLayout.append(verticalScrollBar);
		viewLayout.append(hLayout,AutoSize,0);
		//viewLayout.append(horizontalScrollBar);
		Control::setLayout(&viewLayout);

		m_showVerticalScrollBar = true;
		m_showHorizontalScrollBar = false;

		osdRef().create();
	}

	ScrollView::~ScrollView() {

	}

	void ScrollView::setSize(Size size) {
		Size contentSize = contentLayout->size();
		Size workingSize = size;

		if (m_showVerticalScrollBar) {
			workingSize.setWidth(workingSize.width() - verticalScrollBar.getAutoSize().width());
		}

		if (m_showHorizontalScrollBar) {
			workingSize.setHeight(workingSize.height() - horizontalScrollBar.getAutoSize().height());
		}

		if (m_showVerticalScrollBar) {
			verticalScrollBar.setScrollLength(contentSize.height());
			verticalScrollBar.setPageSize(workingSize.height());
		}

		if (m_showHorizontalScrollBar) {
			horizontalScrollBar.setScrollLength(contentSize.width() - workingSize.width());
			horizontalScrollBar.setPageSize(workingSize.width());
		}

		m_workArea = workingSize;
		viewLayout.update();
	}

	void ScrollView::setLayout(Layout *layout) {
		hLayout.remove(*contentLayout);
		if (!layout)
			contentLayout = &dummyLayout;
		else
			contentLayout = layout;
		hLayout.insertBefore(verticalScrollBar,*contentLayout,Size(SizeTypeAuto,SizeTypeAuto),0);
		layout->update(); //make sure layout has its size calculated
		setSize(size());
	}

}