
#include "Context.hpp"

namespace SC
{

    Context::Context()
        : eventManager()
    {
    }

    Context::~Context()
    {
        SC_TRACE("Cleaning layer stack");
        for (int i = 0; i < layerStack.GetIndex(); i++)
            delete layerStack[i];
    }

}
