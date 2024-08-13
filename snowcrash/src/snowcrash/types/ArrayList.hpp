
#pragma once

#include <cstring>
#include <snowcrash/core/Core.hpp>

namespace SC
{

// holds a resizable list of elements
template<typename T>
struct ArrayList final
{
public:
    ArrayList(int size)
    {
        m_data = new T[size];
        m_capacity = size;
    }

	ArrayList()
	{
		m_data = new T[5];
		m_capacity = 5;
	}

    T Get(int index)
    {
        if(index >= m_index) return m_data[0];
        return m_data[index];
    }

	void Add(T item)
	{
		m_index++;

		if(m_index > m_capacity)
		{
			Resize(m_index+1);
		}

		m_data[m_index] = item;
	}

    void Resize(int size)
    {
        T *copy = new T[size];
        memcpy(copy, m_data, sizeof(T) * size);
        delete[] m_data;
        m_data = copy;
		m_capacity = size;
    }

    T operator[](int i) const { return m_data[i]; }
    int GetIndex() const { return m_index; }
    int GetCapacity() const { return m_capacity; }

private:
    T *m_data;
    int m_index {0}; // points to were the last element is
    int m_capacity {1};
};

}
