
#pragma once

#include <snowcrash/core/Object.hpp>
#include <snowcrash/application/Application.hpp>

namespace editor
{

    class EditorApplication : public snowcrash::Application
    {
        SC_OBJECT(EditorApplication, Application)

    public:
        // just use the default constructor
        explicit EditorApplication(SC::Context *context);
        ~EditorApplication();

        void Init() override;
    };

}
