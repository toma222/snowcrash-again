
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/resource/Resource.hpp>
#include <snowcrash/types/HashMap.hpp>
#include <snowcrash/types/ArrayList.hpp>
#include <snowcrash/types/String.hpp>

namespace SC
{
    class ResourceArray
    {
    public:
        ResourceArray();
        ~ResourceArray();

        // puts a resource in a queue that can be loaded
        // it will not put in the full path, it will only put the file name
        void QueueResourceForLoad(ResourceLoader *loader, String path);
        void LoadResources();

        // is hashing or strcmp faster????
        template <class R>
        const Resource *GetResource(const String &path)
        {
            return const_cast<const R *>(m_hashToResource.Get(path));
        }

    private:
        HashMap<Resource *, String> m_hashToResource;

        // stores resources that we would like to load
        ArrayList<Pair<ResourceLoader *, String>> m_resourceLoadQueue;
    };
}
