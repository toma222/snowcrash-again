
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/resource/Resource.hpp>
#include <snowcrash/types/HashMap.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/types/String.hpp>
#include <snowcrash/types/Pair.hpp>

namespace SC
{
    class ResourceArray
    {
    public:
        ResourceArray();
        ~ResourceArray();

        // puts a resource in a queue that can be loaded
        // it will not put in the full path, it will only put the file name
        template <class RL>
        void QueueResourceForLoad(RL *loader, String path)
        {
            ResourceLoader *l = dynamic_cast<ResourceLoader *>(loader);
            m_resourceLoadQueue.Add(Pair<ResourceLoader *, String>(l, path));
        }

        void LoadResources();

        // is hashing or strcmp faster????
        template <class R>
        const R *GetResource(String path) const
        {
            return dynamic_cast<R *>(m_hashToResource.Get(path));
        }

    private:
        HashMap<Resource *, String> m_hashToResource;

        // stores resources that we would like to load
        ArrayList<Pair<ResourceLoader *, String>> m_resourceLoadQueue;
    };
}
