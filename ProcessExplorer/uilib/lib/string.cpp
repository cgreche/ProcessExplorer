
//String
// File: string.cpp
// Last Edit: 07/01/2018 15:29 (UTC-3)
// Author: CGR

#include <memory.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>
#include "string.h"

#define max(a,b) (a > b ? a : b) //todo: remove

namespace uilib {

	class string_op
	{
	public:
		static int copy(char *dest, const char *src, int count = -1);
		static int compare(const char *string1, const char *string2, int count = 0xffffffff, bool cs = true);
		static bool match(const char *string1, const char *string2, bool cs = true);
		static bool contains(const char *string1, const char *string2, bool cs = true);
		static uint length(const char *string);
		static int integer(const char *string);
		static float decimal(const char *string);
	};


	int string_op::copy(char *dest, const char *src, int count)
	{
		if (!dest || !src || count == 0) return 0;
		if (!*src) {
			*dest = '\0';
			return 0;
		}
		if (count < 0)
			count = ::strlen(src) + 1;
		::strncpy(dest, src, count - 1);
		dest[count - 1] = '\0';
		return count;
	}


	int string_op::compare(const char *string1, const char *string2, int count, bool cs)
	{
		if (count == 0) return 0;
		uint len1 = strlen(string1);
		uint len2 = strlen(string2);
		uint max = len1 > len2 ? len1 : len2;
		if (max == 0)
			return 0;

		if (count < 0) count = max;
		else if ((uint)count > max) count = max;

		int ret = cs ? strncmp(string1, string2, (size_t)count) : _strnicmp(string1, string2, (size_t)count);

		if (ret == 0) {
			if (len2 > len1) ret |= 1;
			else if (len1 > len2) ret |= 2;
			if (len1 != len2) ret |= 4; //if part of strings contain
		}

		return ret;
	}



	bool string_op::match(const char *string1, const char *string2, bool cs)
	{
		return string_op::compare(string1, string2, 0xffffffff, cs) == 0;
	}

	bool string_op::contains(const char *string1, const char *string2, bool cs)
	{
		int res = string_op::compare(string1, string2, cs);
		return res == 0 || (res & 2);
	}

	uint string_op::length(const char *string)
	{
		return strlen(string);
	}

	int string_op::integer(const char *string)
	{
		int ret = 0;
		if (string[0] == '0') {
			if (string[1] == 'x')
				sscanf(string, "%x", &ret);
			else {
			}
		}
		else
			sscanf(string, "%d", &ret);
		return ret;
	}

	float string_op::decimal(const char *string)
	{
		return (float)atof(string);
	}




	bool string::reserve(uint size)
	{
		char *newbuf;
		if (size > m_bufferSize) {
			if (m_bufferSize != 0) {
				//if not first reserve
				//set 256 bytes padding
				size = (size + 255)&~255;
			}

			newbuf = new char[size];
			if (!newbuf) return false;
			memset(newbuf, 0, size);
			if (m_buf) {
				memcpy(newbuf, m_buf, m_bufferSize);
				delete[] m_buf;
			}
			m_buf = newbuf;
			m_bufferSize = size;
		}

		return true;
	}

	string::string(const char *source)
		: m_buf(NULL), m_bufferSize(0),
		m_len(0)
	{
		operator=(source);
	}

	string::string(const string &source)
		: m_buf(NULL), m_bufferSize(0),
		m_len(0)
	{
		operator=(source.data());
	}

	string::string(char c)
	{
		operator=(c);
	}

	string::string()
		: m_buf(NULL), m_bufferSize(0),
		m_len(0)
	{
		reserve(32);
	}

	string::~string()
	{
		if (m_buf) {
			delete[] m_buf;
			m_buf = NULL;
		}
	}



	void string::resize(int size)
	{
		if (size < 0)
			return;

		if (size > (int)(m_bufferSize)-1) {
			if (!reserve(size + 1))
				return;
		}
		m_len = size;
		m_buf[size] = '\0';
	}

	string& string::insert(int at, const string &str)
	{
		return insert(at, str.constData(), str.length());
	}

	string& string::insert(int at, const char *str, int count)
	{
		if (count <= 0)
			return *this;

		int newSize = max(m_len, (uint)at) + count;
		reserve(newSize + 1);
		::memmove(m_buf + at + count, m_buf + at, (newSize - at - count) * sizeof(char));
		::memcpy(m_buf + at, str, count * sizeof(char));
		m_buf[newSize] = '\0';
		m_len = newSize;
		return *this;
	}

	string& string::insert(int at, char ch)
	{
		int newSize = max(m_len, (uint)at) + 1;
		reserve(newSize + 1);
		::memmove(m_buf + at + 1, m_buf + at, (newSize - at - 1) * sizeof(char));
		m_buf[at] = ch;
		m_buf[newSize] = '\0';
		m_len = newSize;
		return *this;
	}

	string& string::append(const string& str)
	{
		uint count = str.length();
		reserve(m_len + count + 1);
		char *c = &m_buf[m_len];
		::memcpy(c, str.constData(), count);
		c[count] = '\0';
		m_len += count;
		return *this;
	}

	string& string::append(const char *str, int count)
	{
		if (str && count > 0) {
			unsigned int srcLen = string_op::length(str);
			if (srcLen < (unsigned int)count)
				count = srcLen;

			unsigned int myLen = this->length();
			reserve(myLen + count + 1);
			char *c = &m_buf[myLen];
			::memcpy(c, str, count);
			c[count] = '\0';
			m_len += count;
		}
		return *this;
	}

	string& string::replace(int index, int len, const char *str, int strLen) {

		if (index < 0 || index > m_len)
			return *this;
		if (index + len > m_len)
			len = m_len - index;

		if (strLen == -1)
			strLen = ::strlen(str);

		if (len == strLen) {
			memcpy(&m_buf[index], str, strLen);
		}
		else if (strLen < len) {
			memcpy(&m_buf[index ], str, strLen);
			memmove(&m_buf[index + strLen], &m_buf[index + len], m_len - (index + len));
			resize(m_len - (len - strLen));
		}
		else {
			resize(m_len + strLen - len);
			memmove(&m_buf[index + strLen], &m_buf[index + len], m_len - (index + len));
			memcpy(&m_buf[index], str, strLen);
		}

		return *this;
	}

	string& string::append(char c) {
		reserve(m_len + 2);
		m_buf[m_len] = c;
		m_buf[++m_len] = '\0';
		return *this;
	}

	string string::toUpper() const {
		const char *c = m_buf;
		string ret;
		ret.resize(this->size());
		char *d = ret.m_buf;
		while (*c) {
			*d++ = ::toupper(*c++);
		}
		*d = '\0';
		return ret;
	}

	string string::toLower() const {
		const char *c = m_buf;
		string ret;
		ret.resize(this->size());
		char *d = ret.m_buf;
		while (*c) {
			*d++ = ::tolower(*c++);
		}
		*d = '\0';
		return ret;
	}


	string& string::operator=(const char *source)
	{
		unsigned int len;
		if (!source) source = "";
		len = ::strlen(source);
		reserve(len + 1);
		::memcpy(m_buf, source, len);
		m_buf[len] = '\0';
		m_len = len;
		return *this;
	}

	string& string::operator=(const char c)
	{
		reserve(2);
		m_buf[0] = c;
		m_buf[1] = '\0';
		return *this;
	}

	string& string::operator=(const string &source)
	{
		return operator=(source.data());
	}

	string& string::operator+=(const string &source)
	{
		return operator+=(source.constData());
	}

	string& string::operator+=(const char *source)
	{
		string s(source);
		return append(s);
	}

	string& string::operator +=(char c)
	{
		return append(c);
	}

	bool string::operator ==(const string &source) const
	{
		return string_op::match((*this).data(), source.data(), true);
	}

	bool string::operator !=(const string &source) const
	{
		return !string_op::match((*this).data(), source.data(), true);
	}

	bool string::operator ==(const char *source) const
	{
		return string_op::match((*this).data(), source, true);
	}

	bool string::operator !=(const char *source) const
	{
		return !string_op::match((*this).data(), source, true);
	}




	int string::toInteger() const
	{
		return string_op::integer(this->data());
	}

	float string::toFloat() const
	{
		return string_op::decimal(this->data());
	}



	//static
	string string::fromInteger(int n, int base)
	{
		char vstr[12];
		if (base == 10) {
			::sprintf(vstr, "%d", n);
		}
		else if (base == 16) {
			::sprintf(vstr, "%x", n);
		}
		else if (base == 8) {
		}
		else if (base == 2) {
		}
		else {
		}

		string s(vstr);
		return s;
	}

	//static
	string string::fromFloat(float f, int precision)
	{
		//todo: fix precision in str
		char vstr[80];
		char fmt[80];
		::sprintf(fmt, "%%.%df", precision);
		::sprintf(vstr, fmt, f);

		string s(vstr);
		return s;
	}

}
