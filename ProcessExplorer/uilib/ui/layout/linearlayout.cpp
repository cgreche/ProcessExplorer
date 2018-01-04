
//LinearLayout
// File: linearlayout.cpp
// Last edit: 18/06/2017 00:50 (UTC-3)
// Author: CGR

#include "../uibase.h"

namespace uilib {

	void LinearLayout::calcMinMaxSize()
	{
		//if (!m_dirty)
		//	return;

		//todo: max size is buggy
		Size definedSize, minSize, biggestChildSize;
		uint undefinedWidthCount = 0, undefinedHeightCount = 0;
		float definedPercentWidth = 0;
		float definedPercentHeight = 0;

		LayoutItemList::iterator it;

		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &li = **it;

			Layout *layout = li.layout();
			Control *control = li.control();

			//Add spacing if the element is not the last in the list
			if ((it + 1) != m_childList.end()) {
				int spacing = li.spacing();
				if (isSizeTypeAuto(spacing))
					spacing = m_defSpacing;
				definedSize.setWidth(definedSize.width() + spacing);
				definedSize.setHeight(definedSize.height() + spacing);
			}

			Size requiredSize = li.getRequiredSize();
			Size childSize = requiredSize;
			if (isSizeTypeMaximum(requiredSize.width())) {
				++undefinedWidthCount;
				childSize.setWidth(0);
				//todo: if layout, set minSize
			}
			else if (isSizeTypePercent(requiredSize.width())) {
				childSize.setWidth(0);
				definedPercentWidth += PercentValue(requiredSize.width());
			}

			if (isSizeTypeMaximum(requiredSize.height())) {
				++undefinedHeightCount;
				childSize.setHeight(0);
				//todo: if layout, set minSize
			}
			else if (isSizeTypePercent(requiredSize.height())) {
				childSize.setHeight(0);
				definedPercentHeight += PercentValue(requiredSize.height());
			}

			//
			definedSize += childSize;
			biggestChildSize.setWidth(max(biggestChildSize.width(), childSize.width()));
			biggestChildSize.setHeight(max(biggestChildSize.height(), childSize.height()));
		}

		minSize += definedSize;

		m_minSize = minSize;
		m_maxSize = biggestChildSize;
		m_definedSize = definedSize;
		m_undefinedWidthCount = undefinedWidthCount;
		m_undefinedHeightCount = undefinedHeightCount;
		m_definedPercentWidth = definedPercentWidth;
		m_definedPercentHeight = definedPercentHeight;
	}

}