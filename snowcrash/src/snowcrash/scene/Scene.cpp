
#include "Scene.hpp"

#include <snowcrash/types/UUID.hpp>
#include <snowcrash/scene/Component.hpp>

namespace SC
{
    World::World()
    {
        m_registry = new entt::registry();
    }

    World::~World()
    {
        delete m_registry;
    }

    Entity World::CreateEntity(String name)
    {
        return CreateEntityUUID(name, UUID());
    }

    Entity World::CreateEntityUUID(String name, UUID id)
    {
        auto entity = m_registry->create();
        Entity entityHandle(m_registry, entity);

        entityHandle.AddComponent<ComponentID>(name, id);
        entityHandle.AddComponent<ComponentTransform>();

        return entityHandle;
    }

    void World::UpdateSystems(Timestamp deltaTime)
    {
        for (int i = 0; i < m_systems.GetIndex(); i++)
            m_systems[i]->Update(this, deltaTime);
    }

    System::System(Context *context)
        : Object(context) {}

    System::~System()
    {
    }
} // namespace SC
