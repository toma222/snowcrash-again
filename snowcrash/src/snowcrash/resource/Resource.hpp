
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

        const u32 GetResourceHash() { return m_hash; }

    private:
        const u32 m_hash;
    };

    // loads the resource
    class ResourceLoader
    {
    public:
        virtual void LoadResource(Resource **resource, String path) = 0;
    };
} // namespace SC
