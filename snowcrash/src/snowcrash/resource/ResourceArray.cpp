
#include "ResourceArray.hpp"

#include <snowcrash/resource/Resource.hpp>

namespace SC
{
    ResourceArray::ResourceArray() = default;

    ResourceArray::~ResourceArray()
    {
        SC_TRACE("Deleting loaded resources");
        ArrayList<Resource *> resources;
        m_hashToResource.ToArray(resources);

        for (int i = 0; i < resources.GetIndex(); i++)
        {
            delete resources[0];
        }
    }

    /*
    void ResourceArray::QueueResourceForLoad(ResourceLoader *loader, String path)
    {
        m_resourceLoadQueue.Add(Pair<ResourceLoader *, String>(loader, path));
    }
    */

    void ResourceArray::LoadResources()
    {
        for (int i = 0; i < m_resourceLoadQueue.GetIndex(); i++)
        {
            auto &entry = m_resourceLoadQueue.Get(i);
            SC_TRACE("Loading resource [%i/%i][%s]", i + 1, m_resourceLoadQueue.GetIndex(), entry.second.c_str());

            Resource *resource{nullptr};
            entry.first->LoadResource(&resource, entry.second);

            if (resource == nullptr)
            {
                SC_WARN("Resource load returned error");
            }
            else
            {
                // different OS's will be wacky about path separators
                int sl = entry.second.LastIndexOfChar('/') + 1;
                int size = entry.second.GetSize() - sl - 1;
                // SC_TRACE("Loaded resource %s %i", entry.second.Substring(sl, size).c_str(), size);
                m_hashToResource.Emplace(resource,
                                         entry.second.Substring(sl, size));
            }

            delete entry.first;
        }
    }
} // namespace SC
