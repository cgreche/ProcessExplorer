#include <memory.h>
#include "bitmap.h"

namespace uilib {

	int Bitmap::ref()
	{
		return ++m_refCount;
	}

	int Bitmap::deref()
	{
		int refCount = --m_refCount;
		if (refCount == 0) {
			delete this;
		}

		return refCount;
	}

	Bitmap::Bitmap(Format format, u32 width, u32 height)
		: m_data(NULL),
		m_palette(NULL),
		m_useColorMask(false),
		m_colorMask(0)
	{
		m_refCount = 1;

		setup(format, width, height);
	}

	Bitmap::~Bitmap()
	{
		if (m_palette)
			m_palette->deref();

		if (m_data) {
			delete[] m_data;
			m_data = NULL;
		}
	}

	void Bitmap::setup(Format format, u32 width, u32 height)
	{
		u32 bpp;
		u32 pitch;

		/*
		switch(format) {
		case QImage::Format_Invalid:
		case QImage::NImageFormats:
			Q_ASSERT(false);
		case QImage::Format_Mono:
		case QImage::Format_MonoLSB:
			depth = 1;
			break;
		case QImage::Format_Indexed8:
			depth = 8;
			break;
		case QImage::Format_RGB32:
		case QImage::Format_ARGB32:
		case QImage::Format_ARGB32_Premultiplied:
			depth = 32;
			break;
		case QImage::Format_RGB555:
		case QImage::Format_RGB16:
		case QImage::Format_RGB444:
		case QImage::Format_ARGB4444_Premultiplied:
			depth = 16;
			break;
		case QImage::Format_RGB666:
		case QImage::Format_ARGB6666_Premultiplied:
		case QImage::Format_ARGB8565_Premultiplied:
		case QImage::Format_ARGB8555_Premultiplied:
		case QImage::Format_RGB888:
			depth = 24;
			break;
		}
		*/

		switch (format) {
		case Indexed8:
			bpp = 8;
			break;

		case RGB32:
			bpp = 32;
			break;

		default:
			break;
		}

		pitch = ((width*bpp + 31) / 32) * 4;

		m_format = format;
		m_width = width;
		m_height = height;
		m_bpp = bpp;
		m_pitch = pitch;

		u32 dataSize = pitch*m_height;
		if (m_data) {
			delete[] m_data;
		}
		m_data = new u8[dataSize];
		m_dataSize = dataSize;
	}


	void Bitmap::setData(u8 *data, u32 size)
	{
		if (size > m_dataSize)
			size = m_dataSize;
		memcpy(m_data, data, size);
		//	for(unsigned int i = 0; i < size; ++i) {
		//		m_data[i] = data[i];
		//	}
	}


	void Bitmap::setPalette(Palette *palette)
	{
		if (palette == m_palette)
			return;

		if (m_palette)
			m_palette->deref();
		m_palette = palette;
		if (palette)
			palette->ref();
	}

	void Bitmap::setColorMask(bool use, u32 rgb)
	{
		m_useColorMask = use;
		m_colorMask = rgb;
	}

	u8 *Bitmap::data(u32 x, u32 y) const
	{
		u32 bpp = this->bpp();
		u32 test = (y*m_pitch) + x*bpp / 8;
		return m_data + (y*m_pitch) + (x*bpp / 8);
	}

}