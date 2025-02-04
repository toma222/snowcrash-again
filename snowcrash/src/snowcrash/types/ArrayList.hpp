
#pragma once

#include <cstring>
#include <utility>
#include <snowcrash/core/Core.hpp>

namespace SC
{

    // holds a resizable list of elements
    template <typename T>
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
            m_data = new T[1];
            m_capacity = 1;
        }

        ~ArrayList()
        {
            if (m_data != nullptr)
                delete[] m_data;
        }

        T &Get(int index) const
        {
            if (index >= m_index)
                return m_data[0];
            return m_data[index];
        }

        void Add(const T &item)
        {
            if (m_index >= m_capacity)
            {
                Resize(m_index + 1);
            }

            m_data[m_index] = item;
            m_index++;
        }

        void Add(const T &&item)
        {
            if (m_index >= m_capacity)
            {
                Resize(m_index + 1);
            }

            m_data[m_index] = item;
            m_index++;
        }

        void Resize(int size)
        {
            T *copy = new T[size];
            for (u32 i = 0; i < m_index; i++)
                copy[i] = m_data[i];
            delete[] m_data;
            m_data = copy;
            m_capacity = size;
        }

        // removes an element from a list
        void Erase(int i, u32 size = 1)
        {
            MoveRange(i, i + size, m_capacity - i - size);
        }

        T &operator[](int i) const { return m_data[i]; }
        int GetIndex() const { return m_index; }
        void SetIndex(int i) { m_index = i; } // this is a really dangourus function so be carefull <3
        int GetCapacity() const { return m_capacity; }

        T *GetArray() const { return m_data; }

        bool Empty() const { return (m_index == 0); }

        ArrayList<T> operator=(const ArrayList<T> &arr)
        {
            ArrayList<T> t(arr.m_capacity);
            memccpy(t.m_data, arr.m_data, arr.m_capacity * sizeof(T));
            t.m_index = arr.m_index;
            return t;
        }

    private:
        void MoveRange(u32 dest, u32 source, u32 count)
        {
            if (count)
                memmove(m_data + dest, m_data + source, count * sizeof(T));
        }

    private:
        T *m_data{nullptr};
        int m_index{0}; // points to were the last element is
        int m_capacity{1};
    };

}
