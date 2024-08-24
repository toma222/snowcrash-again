
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/core/Logging.hpp>

namespace SC
{

    template <typename T>
    class Queue
    {
    public:
        Queue(int queueSize)
            : m_capacity(queueSize)
        {
            m_bottom = new T[queueSize];
        }

        ~Queue()
        {
            delete[] m_bottom;
        }

        bool Add(T element)
        {
            if (m_elements == m_capacity)
                return false;
            m_bottom[m_elements] = element;
            m_elements++;

            return true;
        }

        bool Next()
        {
            if (m_elements == 0)
                return false;

            m_elements--;
            return true;
        }

        bool HasNext() { return m_elements != 0; }

        T TopElement() { return m_bottom[m_elements]; }

        int GetElementsInQueue() const { return m_elements; }
        const int GetCapacity() const { return m_capacity; }
        T *GetQueue() const { return m_bottom; }

    private:
        T *m_bottom;

        int m_elements{0}; // the amount of elements left in the queue
        const int m_capacity;
    };

}
