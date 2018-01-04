
//Layout
// File: layout.cpp
// Last edit: 26/11/2017 01:35 (UTC-3)
// Author: CGR

#include "../uibase.h"

namespace uilib {

	float Layout::left_align(0.0f);
	float Layout::center_align(0.5f);
	float Layout::right_align(1.0f);



	//LayoutItem

	Layout *LayoutItem::layout()
	{
		return NULL;
	}

	Control *LayoutItem::control()
	{
		return NULL;
	}

	Size LayoutItem::getRequiredSize()
	{
		Size wantedSize = this->size();
		Size retSize;

		Size autoSize = getAutoSize();

		//Firstly, process the width
		//Check if size is defined as constant value
		if (isSizeTypeItem(wantedSize.width())) {
			retSize.setWidth(currentSize().width());
		}
		else if (isSizeTypeAuto(wantedSize.width())) {
			retSize.setWidth(autoSize.width());
		}
		else {
			retSize.setWidth(wantedSize.width());
		}

		//height
		if (isSizeTypeItem(wantedSize.height())) {
			retSize.setHeight(currentSize().height());
		}
		else if (isSizeTypeAuto(size().height())) {
			retSize.setHeight(autoSize.height());
		}
		else {
			retSize.setHeight(wantedSize.height());
		}

		return retSize;
	}

	//LayoutItemControl

	void LayoutItemControl::setParent(Control *parent) { m_control->setParent(parent); }
	void LayoutItemControl::setPosition(const Point &position) { m_control->setPosition(position); }
	void LayoutItemControl::setSize(const Size &size) { m_control->setSize(size); }
	void LayoutItemControl::setVisible(bool visible) { m_control->setVisible(visible); }
	void LayoutItemControl::setEnabled(bool enabled) { m_control->setEnabled(enabled); }
	Point LayoutItemControl::position() const { return m_control->position(); }
	Size LayoutItemControl::currentSize() const { return m_control->size(); }
	Size LayoutItemControl::getAutoSize() { return m_control->getAutoSize(); }
	Control* LayoutItemControl::control() { return m_control; }


	//LayoutItemLayout

	void LayoutItemLayout::setParent(Control *parent) { m_layout->setParent(parent); }
	void LayoutItemLayout::setPosition(const Point &position) { m_layout->setPosition(position); }
	void LayoutItemLayout::setSize(const Size &size) { m_layout->setSize(size); }
	void LayoutItemLayout::setVisible(bool visible) { m_layout->setVisible(visible); }
	void LayoutItemLayout::setEnabled(bool enabled) { m_layout->setEnabled(enabled); }
	Point LayoutItemLayout::position() const { return m_layout->position(); }
	Size LayoutItemLayout::currentSize() const { return m_layout->size(); }
	Size LayoutItemLayout::getAutoSize() { return m_layout->getAutoSize(); }
	Layout* LayoutItemLayout::layout() { return m_layout; }


	void Layout::requestUpdate() {
		m_dirty = true;
		if (m_parentLayout)
			m_parentLayout->requestUpdate();
	}

	//Layout
	LayoutItemList::iterator Layout::_find(Control &control) {
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			if (child.control() && child.control() == &control) {
				break;
			}
		}
		return it;
	}

	LayoutItemList::iterator Layout::_find(Layout &layout) {
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			if (child.layout() && child.layout() == &layout) {
				break;
			}
		}
		return it;
	}

	LayoutItemList::iterator Layout::_insert(LayoutItemList::iterator position, Control &control, const Size &size, int spacing) {
		LayoutItemList::iterator itSelf = _find(control);
		if (itSelf != m_childList.end()) {
			if (itSelf == position)
				return position;
			delete *itSelf;
			m_childList.erase(itSelf);
		}
		else {
			control.setParent(parentControl());
			control.m_parentLayout = this; //must be set after setParent()
		}

		requestUpdate();
		return m_childList.insert(position, new LayoutItemControl(control, size, spacing));
	}

	LayoutItemList::iterator Layout::_insert(LayoutItemList::iterator position, Layout &layout, const Size &size, int spacing) {
		LayoutItemList::iterator itSelf = _find(layout);
		if (itSelf != m_childList.end()) {
			if (itSelf == position)
				return position;
			delete *itSelf;
			m_childList.erase(itSelf);
		}
		else {
			layout.setParent(parentControl());
			layout.m_parentLayout = this; //must be set after setParent()
		}

		requestUpdate();
		return m_childList.insert(position, new LayoutItemLayout(layout, size, spacing));
	}

	Layout::Layout()
		: m_topMargin(0), m_leftMargin(0), m_bottomMargin(0), m_rightMargin(0),
		m_defSpacing(5), m_alignment(Layout::left_align),
		m_parentControl(NULL), m_parentLayout(NULL),
		m_dirty(true)
	{
	}

	Layout::~Layout()
	{
		//	setVisible(false); //Don't show controls being disappended from the windows
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			if (child.control())
				child.control()->m_parentLayout = NULL;
			if (child.layout())
				child.layout()->m_parentLayout = NULL;
			delete &child;
		}
	}

	void Layout::setParent(Control *parent)
	{
		if (parent == m_parentControl)
			return;
		m_parentControl = parent;
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			child.setParent(parent);
		}
		requestUpdate();
	}

	void Layout::setPosition(const Point &position)
	{
		Point oldPos = m_position;
		m_position = position;

		if (m_childList.empty())
			return;

		Point diffPos = position - oldPos;

		LayoutItemList::iterator it;
		//Move objects
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			Point childPos = child.position() + diffPos;
			child.setPosition(childPos);
		}
	}

	void Layout::setVisible(bool visible)
	{
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			child.setVisible(visible);
		}
	}

	void Layout::setEnabled(bool enabled)
	{
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			child.setEnabled(enabled);
		}
	}

	void Layout::update() {
		if (m_dirty) {
			if (m_parentLayout && m_parentLayout->m_dirty)
				m_parentLayout->update();
			else
				setSize(size());
			m_dirty = false;
		}
	}

	Point Layout::position() const
	{
		return m_position;
	}

	Size Layout::size() const
	{
		return m_currentSize;
	}

	Layout *Layout::layout()
	{
		return this;
	}

	void Layout::setMargin(int margin)
	{
		m_topMargin = m_leftMargin = m_bottomMargin = m_rightMargin = margin;
		requestUpdate();
	}

	void Layout::setMargin(int vMargin, int hMargin)
	{
		m_topMargin = m_bottomMargin = vMargin;
		m_leftMargin = m_rightMargin = hMargin;
		requestUpdate();
	}

	void Layout::setMargin(int topMargin, int leftMargin, int bottomMargin, int rightMargin)
	{
		m_topMargin = topMargin;
		m_leftMargin = leftMargin;
		m_bottomMargin = bottomMargin;
		m_rightMargin = rightMargin;
		requestUpdate();
	}

	void Layout::setTopMargin(int margin)
	{
		m_topMargin = margin;
		requestUpdate();
	}

	void Layout::setLeftMargin(int margin)
	{
		m_leftMargin = margin;
		requestUpdate();
	}

	void Layout::setBottomMargin(int margin)
	{
		m_bottomMargin = margin;
		requestUpdate();
	}

	void Layout::setRightMargin(int margin)
	{
		m_rightMargin = margin;
		requestUpdate();
	}

	void Layout::setDefaultSpacing(int spacing)
	{
		m_defSpacing = spacing;
		requestUpdate();
	}

	void Layout::getMargins(int *pTopMargin, int *pLeftMargin, int *pBottomMargin, int *pRightMargin)
	{
		if (pTopMargin) *pTopMargin = m_topMargin;
		if (pLeftMargin) *pLeftMargin = m_leftMargin;
		if (pBottomMargin) *pBottomMargin = m_bottomMargin;
		if (pRightMargin) *pRightMargin = m_rightMargin;
	}

	void Layout::setAlignment(float alignment)
	{
		m_alignment = max(0.0f, min(1.0f, alignment));
		requestUpdate();
	}

	void Layout::append(Control& control, const Size &size, int spacing)
	{
		_insert(m_childList.end(), control, size, spacing);
	}

	void Layout::remove(Control &control)
	{
		LayoutItemList::iterator it = _find(control);
		if (it != m_childList.end()) {
			LayoutItem &child = **it;
			child.control()->m_parentLayout = NULL; //must be set before setParent()
			child.control()->setParent(NULL);
			delete &child;
			m_childList.erase(it);
			requestUpdate();
		}
	}

	void Layout::insertBefore(Control &position, Control &control, const Size &size, int spacing) {
		LayoutItemList::iterator it = _find(position);
		_insert(it, control, size, spacing);
	}

	void Layout::insertBefore(Layout &position, Control &control, const Size &size, int spacing) {
		LayoutItemList::iterator it = _find(position);
		_insert(it, control, size, spacing);
	}

	void Layout::insertAfter(Control &position, Control &control, const Size &size, int spacing) {
		LayoutItemList::iterator it = _find(position);
		if (it != m_childList.end())
			_insert(it + 1, control, size, spacing);
	}

	void Layout::insertAfter(Layout &position, Control &control, const Size &size, int spacing) {
		LayoutItemList::iterator it = _find(position);
		if (it != m_childList.end())
			_insert(it + 1, control, size, spacing);
	}

	void Layout::append(Layout& layout, const Size &size, int spacing)
	{
		_insert(m_childList.end(),layout, size, spacing);
	}

	void Layout::remove(Layout &layout)
	{
		LayoutItemList::iterator it = _find(layout);
		if (it != m_childList.end()) {
			LayoutItem &child = **it;
			child.layout()->m_parentLayout = NULL; //must be set before setParent()
			child.layout()->setParent(NULL);
			delete &child;
			m_childList.erase(it);
			requestUpdate();
		}
	}

	void Layout::insertBefore(Control &position, Layout &layout, const Size &size, int spacing) {
		LayoutItemList::iterator it = _find(position);
		_insert(it, layout, size, spacing);
	}

	void Layout::insertBefore(Layout &position, Layout &layout, const Size &size, int spacing) {
		LayoutItemList::iterator it = _find(position);
		_insert(it, layout, size, spacing);
	}

	void Layout::insertAfter(Control &position, Layout &layout, const Size &size, int spacing) {
		LayoutItemList::iterator it = _find(position);
		if (it != m_childList.end())
			_insert(it + 1, layout, size, spacing);
	}
	
	void Layout::insertAfter(Layout &position, Layout &layout, const Size &size, int spacing) {
		LayoutItemList::iterator it = _find(position);
		if (it != m_childList.end())
			_insert(it + 1, layout, size, spacing);
	}

	Size Layout::getAutoSize()
	{
		Size ret;
		LayoutItemList::iterator it;
		for (it = m_childList.begin(); it != m_childList.end(); ++it) {
			LayoutItem &child = **it;
			Size autoSize = child.getAutoSize();
			Point pos = child.position();
			int width = pos.x() + autoSize.width();
			int height = pos.y() + autoSize.height();
			if (width > ret.width())
				ret.setWidth(width);
			if (height > ret.height())
				ret.setHeight(height);
		}
		return ret;
	}

	Control *Layout::parentControl() const
	{
		return m_parentControl;
	}

}