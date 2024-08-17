
#pragma once

#include <cstring>
#include <snowcrash/core/Core.hpp>

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

private:
	void Resize(int size); // resizes the buffer; also sets the m_size variable
	

public:
	void operator=(const char *string)
	{
		int strSize = strlen(string);
		Resize(strSize+1); // null terminator
		m_buffer[strSize+1] = '\0';
	}

private:
	int m_size {0}; // the size of the string including the null terminator
	char *m_buffer {nullptr};
};

}

