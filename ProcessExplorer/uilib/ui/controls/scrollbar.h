
// Scrollbar
// File: scrollbar.h
// Last Edit: 18/12/2017 03:30 (UTC-3)
// Author: CGR

#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__

namespace uilib {
	class ScrollBar;
	class OSDScrollBar;

	typedef void(*ScrollFunc)(ScrollBar &scrollbar, int oldPos, int newPos);

	class ScrollBar : public Control
	{
	public:
		enum Orientation {
			Vertical,
			Horizontal
		};

	protected:
		Orientation m_orientation;
		int m_length;
		int m_pageSize;
		std::vector<ScrollFunc> m_onScrollCallbackList;

		int m_scrollPos;
	public:
		ScrollBar(Orientation orientation = Vertical);

		//virtual overrides
		virtual Size getAutoSize();

		void setScrollLength(int length);
		void setPageSize(int pageSize);
		void setScrollPosition(int scrollPos);

		//actions
		void addOnScroll(ScrollFunc onScroll);

		Orientation orientation() const { return m_orientation; }
		int scrollLength() const { return m_length; }
		int pageSize() const { return m_pageSize; }
		int scrollPosition() const { return m_scrollPos; }

		friend class OSDScrollBar;
		OSDScrollBar& osdRef() const { return reinterpret_cast<OSDScrollBar&>(Control::osdRef()); }
	};

}

#endif