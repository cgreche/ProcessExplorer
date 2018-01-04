
//String
// File: string.h
// Last Edit: 02/11/2017 02:31 (UTC-3)
// Author: CGR

#ifndef __STRING_H__
#define __STRING_H__

#include "..\\types.h"

namespace uilib {

	enum {
		ENC_SYSTEM,
		ENC_UTF8,
		ENC_UTF16
	};

	class string {

		char *m_buf;
		uint m_bufferSize;
		uint m_len;

	public:
		static s32 convert(u8 destencoding, u8 *pDest, u16 size, u8 srcencoding, const u8 *pSrc)
		{
			extern s32 system_copyconvertstr(u8 destencoding, u8 *pDest, u16 size, u8 srcencoding, const u8 *pSrc);
			return system_copyconvertstr(destencoding, pDest, size, srcencoding, pSrc);
		}

		string(const char *source);
		string(const string &source);
		string(char c);
		string();
		~string();

		bool reserve(uint size);
		void resize(int size);

		string& insert(int at, const string &str);
		string& insert(int at, const char *str, int count);
		string& insert(int at, char ch);
		string& append(const string &str);
		string& append(const char *str, int count);
		string& append(char ch);

		string& replace(int index, int len, const char *str, int strLen = -1);
		string& replace(int index, int len, const string &str) { return replace(index, len, str.constData()); }

		string toUpper() const;
		string toLower() const;

		int toInteger() const;
		float toFloat() const;

		static string fromInteger(int n, int base = 10);
		static string fromFloat(float f, int precision = 6);

		string& operator=(const string &source);
		string& operator=(const char *source);
		string& operator=(char c);
		string& operator+=(const string &source);
		string& operator+=(const char *source);
		string& operator+=(char ch);
		bool operator==(const string& source) const;
		bool operator==(const char *source) const;
		bool operator!=(const string& source) const;
		bool operator!=(const char *source) const;
		friend const string operator+(const string &s1, const string &s2);
		friend const string operator+(const string &s1, const char *s2);

		inline uint length() const { return m_len; }
		inline uint size() const { return m_len + 1; }
		inline uint bufferSize() const { return m_bufferSize; }
		inline char *data() { return m_buf; }
		inline const char* data() const { return (const char*)m_buf; }
		inline const char* constData() const { return (const char*)m_buf; }
	};

	inline const string operator+(const string& s1, const string &s2) {
		string ret(s1);
		ret += s2;
		return ret;
	}

	inline const string operator+(const string& s1, const char *s2) {
		string ret(s1);
		ret += s2;
		return ret;
	}

	inline const string operator+(const char *s1, const string &s2) {
		string ret(s1);
		ret += s2;
		return ret;
	}

	inline const string operator+(const string& s, char c) {
		string ret(s);
		ret += c;
		return ret;
	}

	inline const string operator+(char c, const string& s) {
		string ret(s);
		ret.insert(0, c);
		return ret;
	}
}

#endif
