
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/Timestamp.hpp>
#include <snowcrash/core/Object.hpp>
#include <snowcrash/types/String.hpp>

#include <entt/entity/registry.hpp>

#include <snowcrash/scene/Entity.hpp>
#include <snowcrash/types/UUID.hpp>

namespace SC
{
    class System;

    class World
    {
    public:
        World();
        ~World();

        Entity CreateEntity(String name);
        Entity CreateEntityUUID(String name, UUID id);

        // delta time is in microseconds
        void UpdateSystems(Timestamp deltaTime);
        void InitSystems();

        template <class S>
        void AddSystem(S *system)
        {
            m_systems.Add(dynamic_cast<System *>(system));
        }

    private:
        ArrayList<System *> m_systems;
        entt::registry *m_registry;
    };

    class System : public Object
    {
        SC_OBJECT(System, Object)

    public:
        System(Context *context);
        ~System();

        virtual void Update(World *world, Timestamp deltaTime) = 0;
        virtual void Init() = 0;
    };
} // namespace SC
