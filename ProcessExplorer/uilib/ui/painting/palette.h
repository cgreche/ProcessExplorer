
//Palette
// File name: palette.h
// Last edit: 26/07/2014 21:53 (UTC-3)
// Author: CGR

#ifndef __PALETTE_H__
#define __PALETTE_H__

#include "..\\..\\types.h"

#include "rgb.h"

namespace uilib {

	class Palette
	{
	private:
		int m_refCount;

	protected:
		Rgb *m_colors;
		u32 m_nColors;

		~Palette();

	public:
		Palette(u32 nColors);

		void setColor(u32 index, Rgb color);
		Rgb getColor(u32 index) const;
		u32 colorCount() const;

		int ref();
		int deref();
	};

}

#endif