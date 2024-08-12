
#pragma once

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

    T Get(int index)
    {
        if(index >= m_index) return m_data[0];
        return m_data[index];
    }

    void Resize(int size)
    {
        T *copy = new m_data[size];
        memcpy(copy, m_data, sizeof(T) * size);
        delete m_data[];
        m_data = copy;
    }

    T operator[](int i) const { return m_index[i]; }
    int GetIndex() const { return m_index; }
    int GetCapacity() const { return m_capacity; }

private:
    T *m_data;
    int m_index {0};
    int m_capacity {0};
};

}
