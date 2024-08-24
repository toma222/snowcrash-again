
#include "RuntimeApplication.hpp"
#include "snowcrash/graphics/GraphicsLayer.hpp"

#include <snowcrash/application/Application.hpp>

namespace runtime
{
    RuntimeApplication::RuntimeApplication(SC::Context *context)
        : Application(context) {}

    RuntimeApplication::~RuntimeApplication() = default;

    void RuntimeApplication::Init()
    {
        m_context->AddLayer(new snowcrash::GraphicsLayer(m_context));

        return;
    }

}
