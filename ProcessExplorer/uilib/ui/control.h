
//Control
// File name: control.h
// Last edit: 21/10/2017 01:26 (UTC-3)
// Author: CGR

#ifndef __UILIB_CONTROL_H__
#define __UILIB_CONTROL_H__

#include <list>

namespace uilib {

	class Control;
	class PaintingDevice;

	typedef std::list<Control*> ControlList;

	enum VisibilityMode
	{
		ShowNormal = 0,
		Minimized, //to be implemented
		Maximized, //to be implemented
		Fullscreen //Window Only
	};

	enum ControlStyles
	{
		CS_Control = 0x00,
		CS_Window = 0x01,
		CS_Resizable = 0x02,
		CS_Caption = 0x04,
		CS_SysMenu = 0x08,
		CS_MinimizeButton = 0x10,
		CS_MaximizeButton = 0x20,
		CS_Popup = 0x080,
		CS_Frameless = 0x1000,

		CS_Border = 0x10000
	};

	class Control : public UIObj
	{
	protected:
		Control *m_parent;
		Layout *m_layout;
		Layout *m_parentLayout;
		ControlList m_childControls;

		Font m_font;
		bool m_visible;
		bool m_enabled;

		Point m_position; //relative
		Size m_size; //size set by user
		Size m_minSize;
		Size m_maxSize;
		Size m_curSize; //current size (be it normal, minimum or maximum size)
		VisibilityMode m_visibilityMode;

		u32 m_flags;
		u32 m_style;
		Color m_backgroundColor;
		bool m_transparent;

		void *m_param;

		//to be changed in the future maybe
		bool m_applicationResize;

		PaintingDevice *m_paintDevice;

	public:
		virtual void onCloseEvent();
		virtual void onPaintEvent(PaintEvent &event);
		virtual void onKeyPressEvent(KeyEvent &event);
		virtual void onKeyReleaseEvent(KeyEvent &event);
		virtual void onMouseMoveEvent(MouseEvent &event);
		virtual void onMousePressEvent(MouseEvent &event);
		virtual void onMouseReleaseEvent(MouseEvent &event);
		virtual void onMouseDoubleClickEvent(MouseEvent &event);
		virtual void onMouseEnterEvent(MouseEvent &event);
		virtual void onMouseLeaveEvent(MouseEvent &event);
		virtual void onScroll(ScrollEvent &event);

	public:
		friend class Layout;
		friend class Painter;
		Control();
		Control(OSDControl &ref);
		virtual ~Control();

		virtual void setParent(Control *parent);
		virtual void setVisible(bool visible);
		virtual void setEnabled(bool enabled);

		virtual void setPosition(const Point &position);
		virtual void setSize(const Size &size);
		virtual void setMinimumSize(const Size &size);
		virtual void setMaximumSize(const Size &size);
		virtual void setVisibilityMode(VisibilityMode visibilityMode);
		virtual void setFrameSize(const Size &Size);
		virtual Size getFrameSize();
		virtual void setStyle(u32 style);
		virtual void setBackgroundColor(const Color& color);
		virtual void setTransparent(bool transparent);
		virtual void setFont(const FontDesc &desc);
		virtual void setDefaultFont();

		virtual void setFocus();

		void setParam(void *param);
		virtual void setLayout(Layout* layout);

		void append(Control &control);
		void remove(Control &control);
		bool contains(Control &control);

		Control *parent() const;
		bool visible() const;
		bool enabled() const;
		Point position() const;
		Size size() const;
		VisibilityMode visibilityMode() const;
		u32 style() const;
		Color backgroundColor() const;
		bool transparent() const;
		Font& font();
		void *param() const;
		Layout *layout() const;

		Control *getTopParent();
		virtual Size getAutoSize();

		PaintingDevice *paintDevice() const { return m_paintDevice; }

		bool isMinimized() const;
		bool isMaximized() const;
		bool isWindow() const { return m_flags&CS_Window; }

		void redraw();

		friend class OSDControl;
		OSDControl& osdRef() const { return reinterpret_cast<OSDControl&>(UIObj::osdRef()); }

		friend class OSDPainter;
	};

}

#endif