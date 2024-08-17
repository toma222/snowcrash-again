
#include "String.hpp"
#include <cmath>
#include <cstdlib>
#include <cstring>

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

String::~String()
{
	delete[] m_buffer;
}

void String::Resize(int size)
{
	char *b = new char[size];
	memcpy(b, m_buffer, sizeof(char) * size);
	delete[] m_buffer;
	m_buffer = b;
	m_size = size;
}

u32 String::Hash() const
{
	constexpr int prime = 31;
    u32 h = 0;
    for(u32 c = 1; c < m_size; c++)
        h += m_buffer[c] * (int)pow(c, 3);
    return h;
}

const u32 String::StaticHash(const char *str)
{
	constexpr int prime = 31;
    u32 h = 0;
    for(u32 c = 1; c < strlen(str); c++)
        h += str[c] * (int)pow(c, 3);
    return h;
}

}

