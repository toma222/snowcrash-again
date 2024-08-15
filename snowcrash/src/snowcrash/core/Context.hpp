
#pragma once

#include "snowcrash/application/Layer.hpp"
#include "snowcrash/types/ArrayList.hpp"
#include <snowcrash/core/Core.hpp>
#include <snowcrash/application/Event.hpp>

namespace SC
{

// Contains all the global variables and state
struct Context
{
public:
	Context();
	~Context();

	template<typename T>
	void AddLayer(T *layer)
	{
		layerStack.Add((Layer*)layer);
	}

public:
	ArrayList<Layer*> layerStack;
	EventMessenger eventMessenger;
};

}
