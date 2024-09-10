
#pragma once

#include <snowcrash/core/Core.hpp>
#include <snowcrash/types/String.hpp>
#include <snowcrash/types/UUID.hpp>

#include <glm/glm.hpp>

namespace SC
{
    struct ComponentID
    {
        ComponentID(String _name, UUID _id)
            : name(_name), id(_id) {}
        ~ComponentID() = default;
        ComponentID(const ComponentID &) = default;

        String name;
        UUID id;
    };

    struct ComponentTransform
    {
        ComponentTransform() = default;
        ComponentTransform(glm::vec3 _position,
                           glm::vec3 _rotation,
                           glm::vec3 _scale)
            : position(_position),
              rotation(_rotation),
              scale(_scale) {}
        ComponentTransform(const ComponentTransform &) = default;

        glm::vec3 position{0.0, 0.0, 0.0};
        glm::vec3 rotation{0.0, 0.0, 0.0};
        glm::vec3 scale{1.0, 1.0, 1.0};
    };

} // namespace SC
