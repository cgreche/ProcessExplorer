
//LinearLayout
// File: linearlayout.h
// Last edit: 18/06/2017 00:49 (UTC-3)
// Author: CGR

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

#ifndef __LINEARLAYOUT_H__
#define __LINEARLAYOUT_H__

namespace uilib {

	class LinearLayout : public Layout
	{
	protected:
		void calcMinMaxSize();
		uint m_undefinedWidthCount;
		uint m_undefinedHeightCount;
		Size m_definedSize;
		Size m_minSize;
		Size m_maxSize;
		float m_definedPercentWidth;
		float m_definedPercentHeight;

	public:
		LinearLayout() { };
		virtual ~LinearLayout() { };
	};

}

#endif