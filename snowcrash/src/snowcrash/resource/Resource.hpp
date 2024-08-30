
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/String.hpp>
#include <snowcrash/types/Pair.hpp>

namespace SC
{
    // Manages a resource
    class Resource
    {
    public:
        explicit Resource(u32 hash);
        virtual ~Resource() = default;

    private:
        const u32 m_hash;
    };

    // loads the resource
    class ResourceLoader
    {
    public:
        virtual void LoadResource(Resource **resource, String path) = 0;
    };

    // Puts resources into an array and then provides a function that loads them
    struct ResourceArray
    {
    public:
        using ResourcePair = Pair<Resource *, String>;

        template <class R>
        void AddResource(R *resourcePointer, String path)
        {
            m_resourceArray.Add(ResourcePair{
                static_cast<Resource *>(resourcePointer),
                path});
        }

        void LoadResources();

    private:
        ArrayList<ResourcePair> m_resourceArray;
    };
} // namespace SC
