
// Status Bar
// File: statusbar.h
// Last edit: 02/10/2013 12:32 (UTC-3)
// Author: CGR


#ifndef __STATUSBAR_H__
#define __STATUSBAR_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

namespace uilib {

	enum {
		format_left = 0,
		format_center = 1,
		format_right = 2,
		format_vcenter = 4,
		format_bottom = 8
	};


	class StatusBar : public Control
	{

	protected:
		string m_text;
		Color m_color;
		Rect m_margin;
		u32 m_format;

	public:
		friend class Window;
		StatusBar();
		~StatusBar();

		void setMargin(const Rect &margin);
		void setText(const string &text);
		void setTextColor(const Color &color);
		void setFormat(u32 format);

		Rect margin() const { return m_margin; }
		u32 format() const { return m_format; }
		Color textcolor() const { return m_color; }
		const string &text() { return m_text; }

		friend class OSDStatusBar;
		OSDStatusBar &osdRef() const { return reinterpret_cast<OSDStatusBar&>(Control::osdRef()); }
	};

}

#endif