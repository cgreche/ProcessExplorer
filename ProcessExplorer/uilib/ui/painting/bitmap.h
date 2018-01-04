#ifndef __BITMAP_H__
#define __BITMAP_H__

#include "..\\..\\types.h"
#include "palette.h"

namespace uilib {

	class Bitmap
	{
		int m_refCount;

	protected:
		virtual ~Bitmap();

		void *m_privateData;

	public:
		enum Format
		{
			Invalid,      // invalid forma
			Indexed8,     // 8bpp indexed
			RGB32        // 32bpp 8-8-8 RGB
		};

		Bitmap(Format format, u32 width, u32 height);

		void setData(u8 *data, u32 size);
		void setPalette(Palette* palette);

		void setColorMask(bool use, u32 rgb = 0);

		Format format() const { return m_format; }
		u32 width() const { return m_width; }
		u32 height() const { return m_height; }
		u32 bpp() const { return m_bpp; }
		u32 pitch() const { return m_pitch; }
		u32 dataSize() const { return m_dataSize; }
		u8 *data() const { return m_data; }
		u8 *data(u32 x, u32 y) const;
		Palette* palette() const { return m_palette; }

		bool useColorMask() const { return m_useColorMask; }
		u32 colorMask() const { return m_colorMask; }

		int ref();
		int deref();

	protected:
		Format m_format;
		u32 m_bpp;
		u32 m_width;
		u32 m_height;
		u32 m_pitch;
		u8 *m_data;
		u32 m_dataSize;
		Palette *m_palette; //palette if there is one

		bool m_useColorMask;
		u32 m_colorMask;

		void setup(Format format, u32 width, u32 height);
	};

}

#endif