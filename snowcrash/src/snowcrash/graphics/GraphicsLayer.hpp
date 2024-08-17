
#pragma once

#include "snowcrash/application/Layer.hpp"
#include "snowcrash/core/Object.hpp"
#include "snowcrash/graphics/Window.hpp"
#include <snowcrash/core/Core.hpp>
#include <snowcrash/core/Context.hpp>

namespace SC
{

class GraphicsLayer : public Layer
{
SC_OBJECT(GraphicsLayer, Layer)

public:
	explicit GraphicsLayer(Context *context);
	~GraphicsLayer();

	void Init() override;
	void Update() override;
};

}

