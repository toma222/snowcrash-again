
#include "RuntimeApplication.hpp"
#include "snowcrash/graphics/GraphicsLayer.hpp"

#include <snowcrash/application/Application.hpp>

namespace runtime
{

RuntimeApplication::RuntimeApplication() = default;
RuntimeApplication::~RuntimeApplication() = default;

void RuntimeApplication::Init()
{
	m_context->AddLayer(new snowcrash::GraphicsLayer());

    return;
}

}
