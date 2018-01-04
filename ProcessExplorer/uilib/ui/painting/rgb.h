
//RGB
// File name: rgb.h
// Last edit: 26/07/2014 21:53 (UTC-3)
// Author: CGR

#ifndef __RGB_H__
#define __RGB_H__

namespace uilib {

	typedef unsigned long Rgb;

	inline Rgb makeRgb(int r, int g, int b) {
		return (0xff << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
	}

	inline Rgb makeRgba(int r, int g, int b, int a) {
		return ((a & 0xff) << 24) | ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
	}

	inline int getR(Rgb rgb) { return ((rgb >> 16) & 0xff); }
	inline int getG(Rgb rgb) { return ((rgb >> 8) & 0xff); }
	inline int getB(Rgb rgb) { return (rgb & 0xff); }

}

#endif