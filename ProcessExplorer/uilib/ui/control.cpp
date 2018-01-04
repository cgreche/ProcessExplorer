
//Control
// File name: control.cpp
// Last edit: 01/10/2017 14:36 (UTC-3)
// Author: CGR

#include "uibase.h"

namespace uilib {

	void Control::onPaintEvent(PaintEvent &event) { osdRef().onPaintEvent(event); }
	void Control::onCloseEvent() { setVisible(false); }
	void Control::onKeyPressEvent(KeyEvent &event) { osdRef().onKeyPressEvent(event); }
	void Control::onKeyReleaseEvent(KeyEvent &event) { osdRef().onKeyReleaseEvent(event); }
	void Control::onMouseMoveEvent(MouseEvent &event) { osdRef().onMouseMoveEvent(event); }
	void Control::onMousePressEvent(MouseEvent &event) { osdRef().onMousePressEvent(event); }
	void Control::onMouseReleaseEvent(MouseEvent &event) { osdRef().onMouseReleaseEvent(event); }
	void Control::onMouseDoubleClickEvent(MouseEvent &event) { osdRef().onMouseDoubleClickEvent(event); }
	void Control::onMouseEnterEvent(MouseEvent &event) { osdRef().onMouseEnterEvent(event); }
	void Control::onMouseLeaveEvent(MouseEvent &event) { osdRef().onMouseLeaveEvent(event); }
	void Control::onScroll(ScrollEvent &event) { osdRef().onScroll(event); }

	Control::Control() :UIObj(*new OSDControl(*this))
	{
		m_parent = NULL;
		m_layout = NULL;
		m_parentLayout = NULL;
		m_visible = true;
		m_enabled = true;
		m_position = Point(0, 0);
		m_size = Size(0, 0);
		m_curSize = Size(0, 0);
		m_minSize = Size(0, 0);
		m_maxSize = Size(0, 0);
		m_visibilityMode = ShowNormal;
		m_style = 0;
		m_backgroundColor = Color(255, 255, 255);
		m_transparent = true;
		m_param = NULL;
		m_applicationResize = false;
		m_paintDevice = NULL;

		osdRef().create();
		setDefaultFont();
		setBackgroundColor(m_backgroundColor);
	}

	Control::Control(OSDControl& osdRef) :UIObj(osdRef)
	{
		m_parent = NULL;
		m_layout = NULL;
		m_parentLayout = NULL;
		m_visible = true;
		m_enabled = true;
		m_position = Point(0, 0);
		m_size = Size(0, 0);
		m_curSize = Size(0, 0);
		m_minSize = Size(0, 0);
		m_maxSize = Size(0, 0);
		m_visibilityMode = ShowNormal;
		m_style = 0;
		m_backgroundColor = Color(255, 255, 255);
		m_transparent = true;
		m_param = NULL;
		m_applicationResize = false;
		m_paintDevice = NULL;
		m_font.set(g_pUIdefFontDesc);
	}

	Control::~Control()
	{
		setVisible(false); //Don't show controls being disappended from the windows

		//remove all child controls
		ControlList::iterator it;
		for (it = m_childControls.begin(); it != m_childControls.end(); ) {
			Control *control = *it++;
			control->setVisible(false); //Don't show controls being disappended from the windows
			control->setParent(NULL);
		}

		//Force-set parent to NULL, because OSDControl::SetParent may call methods of inherited classes
		//that may have been destructed already.
		if (m_parent) {
			Control *parent = m_parent;
			m_parent = NULL;
			parent->remove(*this);
		}
	}

	void Control::setParent(Control *parent)
	{
		//parent layout will remove us from the parent control aswell
		if (m_parentLayout && m_parentLayout->parentControl() != parent) {
			m_parentLayout->remove(*this);
			return;
		}

		Control *curParent = m_parent;
		if (curParent == parent)
			return;
		m_parent = parent;

		if (curParent) {
			setVisible(false);
			curParent->remove(*this);
		}
		if (parent) {
			parent->append(*this);
		}

		osdRef().setParent(parent);
		setVisible(visible() && parent != NULL);
	}

	void Control::setFont(const FontDesc &desc)
	{
		m_font.set(desc);
		osdRef().setFont(desc);
	}

	void Control::setDefaultFont()
	{
		setFont(m_parent ? m_parent->font().desc() : g_pUIdefFontDesc);
	}

	void Control::setVisible(bool visible)
	{
		m_visible = visible;
		osdRef().setVisible(visible);
	}

	void Control::setEnabled(bool enabled)
	{
		m_enabled = enabled;
		osdRef().setEnabled(enabled);
	}


	void Control::setPosition(const Point &position)
	{
		m_position = position;
		osdRef().setPosition(position);
	}

	void Control::setSize(const Size &size)
	{
		/* Commented, so layout can be updated
		if(m_size == size)
			return;
		*/

		m_applicationResize = true; //not a user resize
		m_size = size;

		if (m_visibilityMode == ShowNormal) {
			m_curSize = size;
			osdRef().setSize(size);
		}

		if (layout()) {
			Size newFrameSize = getFrameSize();
			layout()->setSize(newFrameSize);
		}
	}

	void Control::setFrameSize(const Size &size)
	{
		Size frameSize = getFrameSize();
		if (frameSize == size)
			return;
		Size windowSize = m_size;
		windowSize -= frameSize;
		windowSize += size;
		setSize(windowSize);
	}

	void Control::setMinimumSize(const Size &size)
	{
		if (m_minSize == size)
			return;
		m_minSize = size;
		setSize(Size(min(size.width(), m_curSize.width()),
			min(size.height(), m_curSize.height())));
	}

	void Control::setMaximumSize(const Size &size)
	{
		if (m_maxSize == size)
			return;
		m_maxSize = size;
		setSize(Size(max(size.width(), m_curSize.width()),
			max(size.height(), m_curSize.height())));
	}

	void Control::setVisibilityMode(VisibilityMode visibilityMode)
	{
		if (visibilityMode == m_visibilityMode)
			return;
		m_visibilityMode = visibilityMode;
		osdRef().setVisibilityMode(visibilityMode);
	}

	void Control::setStyle(u32 style)
	{
		m_style = style;
		osdRef().setStyle(style);
	}

	void Control::setBackgroundColor(const Color& color)
	{
		m_backgroundColor = color;
		osdRef().setBackgroundColor(color);
	}

	void Control::setTransparent(bool transparent)
	{
		m_transparent = transparent;
		osdRef().setTransparent(transparent);
	}

	void Control::setFocus()
	{
		osdRef().setFocus();
	}

	void Control::setParam(void *param)
	{
		m_param = param;
	}

	void Control::setLayout(Layout* layout)
	{
		if (m_layout)
			m_layout->setParent(NULL);

		if (layout) {
			layout->setParent(this); //must come before getting auto size
			Size layoutSize = layout->getAutoSize();
			Size frameSize = getFrameSize();
			Size finalSize = Size(max(layoutSize.width(), frameSize.width()), max(layoutSize.height(), frameSize.height()));
			layout->setSize(finalSize);
			if (finalSize != frameSize)
				setFrameSize(finalSize);
		}

		m_layout = layout;
	}

	Size Control::getFrameSize()
	{
		return osdRef().getFrameSize();
	}

	void Control::redraw()
	{
		osdRef().redraw();
	}

	void Control::append(Control &control)
	{
		if (contains(control))
			return;
		m_childControls.push_back(&control); // must come before setParent
		control.setParent(this);
	}

	void Control::remove(Control &control)
	{
		if (!contains(control))
			return;
		control.setParent(NULL);
		m_childControls.remove(&control);
	}

	bool Control::contains(Control &control)
	{
		ControlList::iterator it;
		for (it = m_childControls.begin(); it != m_childControls.end(); ++it) {
			if (*it == &control)
				return true;
		}

		return false;
	}

	Control* Control::parent() const { return m_parent; }
	bool Control::visible() const { return m_visible; }
	bool Control::enabled() const { return m_enabled; }
	Point Control::position() const { return m_position; }
	Size Control::size() const { return m_size; }
	VisibilityMode Control::visibilityMode() const { return m_visibilityMode; }
	u32 Control::style() const { return m_style; }
	Color Control::backgroundColor() const { return m_backgroundColor; }
	bool Control::transparent() const { return m_transparent; }
	Font& Control::font() { return m_font; }
	void *Control::param() const { return m_param; }
	Layout* Control::layout() const { return m_layout; }

	Size Control::getAutoSize()
	{
		Size ret;

		if (m_layout)
			ret = m_layout->size();

		if (ret == Size(0, 0))
			return MaximumSize;

		return ret;
	}

	bool Control::isMinimized() const
	{
		return m_visibilityMode == Minimized;
	}

	bool Control::isMaximized() const
	{
		return m_visibilityMode == Maximized;
	}

	Control *Control::getTopParent()
	{
		Control *parent = this->parent();
		if (!parent)
			return NULL;
		Control *temp = parent;
		while (parent->parent()) {
			parent = parent->parent();
		}
		return parent;
	}

}