
#pragma once

#include <snowcrash/core/Core.hpp>
#include <cstdint>
#include <iterator>

namespace SC
{
    struct UUID final
    {
        UUID(u64 t_uuid);
        UUID();
        operator uint64_t() const { return uuid; }
        uint64_t uuid;
    };
}

namespace std
{
    template <typename T>
    struct hash;

    template <>
    struct hash<snowcrash::UUID>
    {
        std::size_t operator()(const snowcrash::UUID &uuid) const
        {
            return (uint64_t)uuid;
        }
    };

}