
#pragma once

#include <snowcrash/scene/World.hpp>

namespace editor
{
    class Panel
    {
    public:
        Panel() = default;
        virtual ~Panel() = default;

        virtual void Update() = 0;

        void SetWorld(SC::World *_world) { world = _world; }

    private:
        SC::World *world{nullptr};
    };
} // namespace editor
