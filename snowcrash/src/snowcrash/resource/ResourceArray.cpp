
#include "ResourceArray.hpp"

namespace SC
{
    ResourceArray::ResourceArray() = default;

    ResourceArray::~ResourceArray()
    {
        for (int i = 0; i < m_resourceLoadQueue.GetIndex(); i++)
        {
            delete m_resourceLoadQueue[i].first;
        }
    }

    void QueueResourceForLoad(ResourceLoader *loader, String path)
    {
    }

    void LoadResources()
    {
    }
} // namespace SC
