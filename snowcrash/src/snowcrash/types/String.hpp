
#pragma once

#include <cstring>
#include <snowcrash/core/Core.hpp>

#include <snowcrash/core/Logging.hpp>

namespace SC
{

	// Holds a string
	struct String
	{
	public:
		String();
		String(const char *str);
		~String();

		int GetSize() const { return m_size; }
		const char *c_str() const { return m_buffer; }

		u32 Hash() const;
		static const u32 StaticHash(const char *str);

		// substring, cat, find char, find string
		String Substring(int len) const;
		String Substring(int index, int len) const;

		int FindIndexOfChar(char c) const;
		char GetCharAtIndex(int i) const;

		int FindIndexOfString(const char *string) const;

		String AppendString(const String &s) const;

	private:
		void Resize(int size); // resizes the buffer; also sets the m_size variable

	public:
		void operator=(const char *string)
		{
			int strSize = strlen(string);
			Resize(strSize + 1); // null terminator
			strcpy(m_buffer, string);
			m_buffer[strSize + 1] = '\0';
		}

		void operator=(const String &string)
		{
			*this = string.c_str();
		}

	private:
		int m_size{0}; // the size of the string including the null terminator
		char *m_buffer{nullptr};
	};

}
