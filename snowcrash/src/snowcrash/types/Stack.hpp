
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
		: m_size(elements) {}

	~Stack()
	{
		delete[] m_base;
	}

	bool Push(T item)
	{
		if(m_elements == m_size) return false;

		m_head++;
		m_elements++;
		*m_head = item;

		return true;
	}

	bool Pop()
	{
		if(m_head == m_base) return false;
		m_elements--;
		m_head--;
		return true;
	}

	void Reset()
	{
		m_head = m_base;
	}

	T *GetStackBase() const { return m_base; }
	int GetStackElements() const { return m_elements; }

private:
	T *m_head;
	T *m_base;
	int m_elements;
	const int m_size;
};

}

