
//VerticalLayout
// File: verticallayout.cpp
// Last edit: 18/06/2017 00:51 (UTC-3)
// Author: CGR

#include "../uibase.h"

namespace uilib {

	//Vertical Layout
	VerticalLayout::VerticalLayout()
	{
	}

	VerticalLayout::~VerticalLayout()
	{
	}

	void VerticalLayout::setSize(const Size &size)
	{
		//if (size == m_currentSize && !m_dirty)
		//	return;
		calcMinMaxSize();

		m_currentSize = size;

		Point curPos = m_position;
		Size curSize = size;
		curPos.setX(curPos.x() + m_leftMargin);
		curPos.setY(curPos.y() + m_topMargin);
		curSize.setWidth(curSize.width() - (m_leftMargin + m_rightMargin));
		curSize.setHeight(curSize.height() - (m_topMargin + m_bottomMargin));

		Size definedSize = m_definedSize;

		definedSize.setWidth(definedSize.width() + (int)(curSize.width()*(m_definedPercentWidth + 0.5f)/100.f));
		definedSize.setHeight(definedSize.height() + (int)(curSize.height()*(m_definedPercentHeight + 0.5f)/100.f));

		LayoutItemList::iterator it;

		//Move and size objects
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;

			Layout *layout = child.layout();
			Control *control = child.control();

			Size childSize = child.getRequiredSize();

			if (isSizeTypeMaximum(childSize.width()))
				childSize.setWidth(curSize.width());
			else if (isSizeTypePercent(childSize.width()))
				childSize.setWidth((curSize.width()*PercentValue(childSize.width())) / 100);

			if (isSizeTypeMaximum(childSize.height()))
				childSize.setHeight(max(curSize.height() - definedSize.height(), 0) / m_undefinedHeightCount);
			else if (isSizeTypePercent(childSize.height()))
				childSize.setHeight((curSize.height()*PercentValue(childSize.height())) / 100);

			int posX = curPos.x() + (int)((curSize.width() - childSize.width())*m_alignment);
			int posY = curPos.y();
			child.setPosition(Point(posX, posY));
			child.setSize(childSize);

			int spacing = isSizeTypeAuto(child.spacing()) ? m_defSpacing : child.spacing();
			curPos.setY(curPos.y() + childSize.height() + spacing);
		}

		m_dirty = false;
	}

	Size VerticalLayout::getAutoSize()
	{
		Size ret;

		calcMinMaxSize();

		if (m_undefinedWidthCount != 0)
			ret.setWidth(SizeTypeMax);
		else
			ret.setWidth(m_maxSize.width() + m_leftMargin + m_rightMargin);

		if (m_undefinedHeightCount != 0)
			ret.setHeight(SizeTypeMax);
		else
			ret.setHeight(m_minSize.height() + m_topMargin + m_bottomMargin);

		return ret;
	}

}