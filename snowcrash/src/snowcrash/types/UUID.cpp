
#include "UUID.hpp"

#include <random>
#include <unordered_map>

namespace SC
{
    static std::random_device s_randomDevice;
    static std::mt19937_64 s_engine(s_randomDevice());
    static std::uniform_int_distribution<uint64_t> s_uniformDistribution;

    UUID::UUID(u64 t_uuid)
        : uuid(t_uuid) {}

    UUID::UUID()
        : uuid(s_uniformDistribution(s_engine)) {}
}