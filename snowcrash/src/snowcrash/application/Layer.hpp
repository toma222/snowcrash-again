
#pragma once

#include <snowcrash/core/Object.hpp>
#include <snowcrash/core/Core.hpp>
// #include <snowcrash/core/Context.hpp>

namespace SC
{

    struct Context;

    // Contains layers of the application
    //      The game layer
    //      The editor layer
    //      The audio layer
    //
    // Layer holds logic for receiving events
    // Layer has an update function

    class Layer : public Object
    {
        SC_OBJECT(Layer, Object)

    public:
        virtual ~Layer();
        explicit Layer(Context *context);

        virtual void Init() = 0;
        virtual void Update() = 0;

        // This function is used by the editor application to draw the layers debug information
        virtual void DrawGuiFrame() {};

        bool GetActive() const { return m_active; }
        void SetActive(bool v) { m_active = v; }

    protected:
        bool m_active;
    };

}
