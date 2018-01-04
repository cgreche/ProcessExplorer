
//TextLink
// File: textlink.h
// Last Edit: 08/10/2017 08:15 (UTC-3)
// Author: CGR

#ifndef __UILIB_TEXTLINK_H__
#define __UILIB_TEXTLINK_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

namespace uilib {

	class Label;
	class TextLink;

	typedef void(*TextLinkFunc)(TextLink &link);

	class TextLink : public Label
	{
	public:
		enum Properties
		{
			Simple = 0,
			Hover = 1,
			Pushed = 2,
			Visited = 4,
		};

	protected:
		Color m_overColor;
		Font m_overFont;
		Color m_visitedColor;
		Font m_visitedFont;
		Color m_pushedColor;
		Font m_pushedFont;

		u32 m_visitcount;

		Properties m_properties;

		TextLinkFunc m_onClick;

	public:
		TextLink();

		void setOnClick(TextLinkFunc onClick) { m_onClick = onClick; }

		void setHoverColor(const Color &color);
		void setHoverFont(const FontDesc &fd);
		void setVisitedColor(const Color &color);
		void setVisitedFont(const FontDesc &fd);
		void setProperties(TextLink::Properties props);

		Color hoverColor() const { return m_overColor; }
		Font& hoverFont() { return m_overFont; }
		u32 visitCount() const { return m_visitcount; }
		Color visitedColor() const { return m_visitedColor; }
		Font& visitedFont() { return m_visitedFont; }
		Color pushedColor() const { return m_pushedColor; }
		Font& pushedFont() { return m_pushedFont; }
		TextLink::Properties properties() const { return m_properties; }

		void increaseVisits() { ++m_visitcount; }
		bool visited() { return m_visitcount > 0; }

		friend class OSDTextLink;
		OSDTextLink& osdRef() const { return reinterpret_cast<OSDTextLink&>(Label::osdRef()); }
	};

}

#endif