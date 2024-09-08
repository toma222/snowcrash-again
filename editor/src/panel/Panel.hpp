
#pragma once

namespace editor
{
    class Panel
    {
    public:
        Panel() = default;
        virtual ~Panel() = default;

        virtual void Update() = 0;
    };
} // namespace editor
