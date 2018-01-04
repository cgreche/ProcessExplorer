
//Font
// File name: font.h
// Last edit: 30/11/2014 03:59 (UTC-3)
// Author: CGR

#ifndef __FONT_H__
#define __FONT_H__

#ifndef __UIBASE_H__
#error This file must be included only by "uibase.h"
#endif

namespace uilib {

#define FONT_BOLD 1
#define FONT_ITALIC 2
#define FONT_UNDERLINE 4
#define FONT_STRIKEOUT 8

	typedef struct _FontDesc
	{
		const char *name;
		uint size;
		u32 flags;

		_FontDesc() :name(0), size(0), flags(0) {}
		_FontDesc(const char *n, u32 s, u32 f) :name(n), size(s), flags(f) {}
	}FontDesc;

	class Font
	{
		OSDFont *m_osdRef;

	protected:
		FontDesc m_desc;

	public:
		Font(const FontDesc &desc);
		Font(const Font& font);
		Font();
		~Font();
		bool set(const FontDesc &desc);

		FontDesc desc() const { return m_desc; }

		OSDFont& osdRef() const { return *m_osdRef; }

		Font& operator=(const Font& font);
	};

}

#endif