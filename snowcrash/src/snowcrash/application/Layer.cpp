
#include "Layer.hpp"
#include "snowcrash/core/Object.hpp"

namespace SC
{

Layer::~Layer() = default;
Layer::Layer(Context *context)
	: Object(context) {}

}
