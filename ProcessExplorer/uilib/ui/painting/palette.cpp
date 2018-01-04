
//Palette
// File name: palette.cpp
// Last edit: 26/07/2014 21:53 (UTC-3)
// Author: CGR

#include "palette.h"

namespace uilib {

	int Palette::ref()
	{
		return ++m_refCount;
	}

	int Palette::deref()
	{
		int refCount = --m_refCount;
		if (refCount == 0) {
			delete this;
		}
		return refCount;
	}

	Palette::Palette(u32 nColors)
	{
		m_refCount = 1;

		m_colors = new Rgb[nColors];
		m_nColors = nColors;
	}

	Palette::~Palette()
	{
		if (m_colors) {
			delete[] m_colors;
			m_colors = NULL;
		}
	}

	void Palette::setColor(u32 index, Rgb color)
	{
		m_colors[index%m_nColors] = color;
	}

	Rgb Palette::getColor(u32 index) const
	{
		return m_colors[index%m_nColors];
	}

	u32 Palette::colorCount() const
	{
		return m_nColors;
	}

}