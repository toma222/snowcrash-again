
#pragma once

#include <glm/glm.hpp>
#include <snowcrash/types/ArrayList.hpp>

namespace SC
{
    struct ModelVertex
    {
        glm::vec3 position;
        glm::vec3 color;
        glm::vec2 texCoord;
        glm::vec3 normal;
    };

    struct Model
    {
        ArrayList<ModelVertex> m_vertices;
        ArrayList<u32> m_indicies;
    };

} // namespace SC
