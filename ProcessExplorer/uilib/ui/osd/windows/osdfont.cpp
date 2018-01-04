
//OSD Font
// File: osdfont.cpp
// Last edit: 30/11/2014 04:14 (UTC-3)
// Author: CGR

#include "..\\..\\uibase.h"

namespace uilib {

	OSDFont::OSDFont(const FontDesc& fontDesc)
		: m_refCount(1)
	{
		__create(fontDesc);
	}

	OSDFont::~OSDFont()
	{
		if (m_hFont)
			::DeleteObject(m_hFont);
	}

	int OSDFont::addRef()
	{
		return ++m_refCount;
	}

	int OSDFont::release()
	{
		int refCount = --m_refCount;
		if (refCount == 0)
			delete this;
		return refCount;
	}

	bool OSDFont::__create(const FontDesc& fontDesc)
	{
		if (!fontDesc.name || !*fontDesc.name)
			return false;

		int fontHeight = (int)-(fontDesc.size * 96.0 / 72.0 + 0.5);

		m_hFont = ::CreateFont(fontHeight, 0, 0, 0,
			(fontDesc.flags&FONT_BOLD) ? FW_BOLD : FW_NORMAL,
			(fontDesc.flags&FONT_ITALIC),
			(fontDesc.flags&FONT_UNDERLINE),
			(fontDesc.flags&FONT_STRIKEOUT),
			0,
			OUT_DEFAULT_PRECIS,
			CLIP_DEFAULT_PRECIS,
			DEFAULT_QUALITY,
			0,
			fontDesc.name);

		return m_hFont != NULL;
	}

}