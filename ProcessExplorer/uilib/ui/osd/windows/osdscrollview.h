
//OSD ScrollView
// File: osdscrollview.cpp
// Last edit: 25/10/2017 05:57 (UTC-3)
// Author: CGR

#include "osdbase.h"

namespace uilib {

	class ScrollView;

	class OSDScrollView : public OSDControl
	{

	public:
		OSDScrollView(ScrollView& ref);
		ScrollView& ref() const { return reinterpret_cast<ScrollView&>(OSDControl::ref()); }

		//OSDControl methods
		virtual void create();
		virtual void update();

		virtual Size getAutoSize();
	};

}