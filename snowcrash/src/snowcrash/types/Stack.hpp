
#pragma once

#include <snowcrash/core/Core.hpp>

namespace SC
{

// Makes a stack in a fixed amount of memory
template<typename T>
class Stack
{
public:
	Stack(int elements)
		: m_size(elements)
	{
		m_base = new T[elements];
	}

	~Stack() = default;

	bool Push(T item)
	{
		if(m_elements == m_size) return false;
		m_elements++;
		m_base[m_elements] = item;

		return true;
	}

	bool Pop()
	{
		if(m_elements == 0) return false;
		m_elements--;
		return true;
	}

	T Top()
	{
		return m_base[m_elements];
	}

	void Reset()
	{
		m_elements = 0;
	}

	bool Empty() { return (m_elements == m_size); }

	T *GetStackBase() const { return m_base; }
	int GetStackElements() const { return m_elements; }

private:
	T *m_base;
	int m_elements;
	const int m_size;
};

}

