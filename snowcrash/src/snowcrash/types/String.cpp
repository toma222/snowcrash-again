
#include "String.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>

#include <snowcrash/core/Logging.hpp>

namespace SC
{

	String::String()
	{
		m_buffer = new char;
		m_buffer[0] = '\0';
		m_size = 1;
	}

	String::String(const char *str)
	{
		*this = str;
	}

	String::String(const String &str)
	{
		*this = str.c_str();
	}

	String::~String()
	{
		if (m_buffer != nullptr)
			delete[] m_buffer;
	}

	void String::Resize(int size)
	{
		char *b = new char[size];
		if (m_buffer != nullptr)
		{
			memcpy(b, m_buffer, sizeof(char) * size);
			delete[] m_buffer;
		}
		m_buffer = b;
		m_size = size;
	}

	String String::Substring(int len) const
	{
		char s[len];
		memcpy(&s, m_buffer, len);
		s[len] = '\0';
		return String(s);
	}

	String String::Substring(int index, int len) const
	{
		char s[len];
		memcpy(&s, m_buffer + index, len);
		s[len] = '\0';
		return String(s);
	}

	int String::FindIndexOfChar(char c) const
	{
		return (int)(strchr(m_buffer, c) - m_buffer);
	}

	char String::GetCharAtIndex(int i) const
	{
		return m_buffer[i];
	}

	int String::FindIndexOfString(const char *str) const
	{
		return (int)(strstr(m_buffer, str) - m_buffer);
	}

	String String::AppendString(const String &s) const
	{
		char str[s.m_size + m_size];
		strcpy(str, m_buffer);
		strcat(str, s.c_str());
		return String(str);
	}

	int String::LastIndexOfChar(char c) const
	{
		return (int)(strrchr(m_buffer, c) - m_buffer);
	}

	u32 String::Hash() const
	{
		constexpr int prime = 31;
		u32 h = 0;
		for (u32 c = 1; c < m_size; c++)
			h += m_buffer[c] * (int)pow(c, 3);
		return h;
	}

	const u32 String::StaticHash(const char *str)
	{
		constexpr int prime = 31;
		u32 h = 0;
		for (u32 c = 1; c < strlen(str); c++)
			h += str[c] * (int)pow(c, 3);
		return h;
	}

}
