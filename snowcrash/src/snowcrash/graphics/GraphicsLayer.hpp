
#pragma once

#include "snowcrash/application/Layer.hpp"
#include "snowcrash/graphics/Window.hpp"
#include <snowcrash/core/Core.hpp>
#include <snowcrash/core/Context.hpp>

namespace SC
{

class GraphicsLayer : public Layer
{
public:
	GraphicsLayer();
	~GraphicsLayer();

	void Init() override;
	void Update(Context *context) override;
};

}

