
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
        ResourceLoader() = default;
        virtual ~ResourceLoader() = default;

        virtual void LoadResource(Resource **resource, String path) = 0;

        const String GetError() const { return error; }

    protected:
        String error;
    };
} // namespace SC
