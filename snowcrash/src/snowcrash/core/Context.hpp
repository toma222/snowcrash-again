
#pragma once

#include "snowcrash/application/Layer.hpp"
#include "snowcrash/types/ArrayList.hpp"
#include <snowcrash/core/Core.hpp>

namespace SC
{

// Contains all the global variables and state
struct Context
{
public:
	Context();

	template<typename T>
	void AddLayer(T *layer)
	{
		layerStack.Add(reinterpret_cast<Layer*>(layer));
	}

public:
	ArrayList<Layer*> layerStack;
};

}
