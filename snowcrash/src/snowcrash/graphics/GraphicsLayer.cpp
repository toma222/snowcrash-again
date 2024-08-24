
#include "GraphicsLayer.hpp"
#include "snowcrash/application/Layer.hpp"

namespace SC
{

	GraphicsLayer::GraphicsLayer(Context *context)
		: Layer(context)
	{
	}

	GraphicsLayer::~GraphicsLayer()
	{
		SC_TRACE("Clean graphics layer");
	}

	void GraphicsLayer::Init()
	{
	}

	void GraphicsLayer::Update()
	{
		// m_context->QueueEvent<EventApplicationError>(new EventApplicationError("graphics layer wanted to quit lol"));
	}

}
