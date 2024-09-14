
#pragma once

#include <snowcrash/core/Core.hpp>

#include <entt/entity/registry.hpp>

namespace SC
{
    // A handle class for entity
    struct Entity
    {
    public:
        Entity() = default;
        ~Entity() = default;

        Entity(entt::registry *registry, entt::entity entity)
            : m_registry(registry), m_handle(entity) {}

        // ! wacky error here, but the component struct has to hold data
        // ! or else this function will return void :3
        template <typename C>
        C &GetComponent()
        {
            return m_registry->get<C>(m_handle);
        }

        template <class C, typename... Args>
        C &AddComponent(Args &&...args)
        {
            return m_registry->emplace<C>(m_handle, std::forward<Args>(args)...);
        }

        template <typename C>
        bool HasComponent()
        {
            return m_registry->any_of<C>(m_handle);
        }

        operator bool() { return !(m_handle == entt::null); }

    public:
        entt::registry *m_registry{nullptr};
        entt::entity m_handle{entt::null};
    };
} // namespace SC
