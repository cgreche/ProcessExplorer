
//Separator
// File: separator.h
// Last Edit: 30/06/2017 00:50 (UTC-3)
// Author: CGR

#ifndef __UILIB_SEPARATOR_H__
#define __UILIB_SEPARATOR_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

namespace uilib {

	class Label;
	class Separator;

	class Separator : public Label
	{

	protected:
		Color m_lineColor;
		u32 m_thickness;

	public:
		Separator();

		virtual Size getAutoSize();
		void setLineThickness(u32 thickness);
		void setLineColor(const Color &color);

		Color lineColor() const { return m_lineColor; }
		u32 thickness() const { return m_thickness; }

		friend class OSDSeparator;
		OSDSeparator& osdRef() const { return reinterpret_cast<OSDSeparator&>(Label::osdRef()); }
	};

}

#endif