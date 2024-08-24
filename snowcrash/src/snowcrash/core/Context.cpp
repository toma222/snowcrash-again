
#include "Context.hpp"

namespace SC
{

    Context::Context()
    {
    }

    Context::~Context()
    {
        SC_TRACE("Cleaning layer stack");
        for (int i = 0; i < layerStack.GetIndex(); i++)
            delete layerStack.GetArray()[i];
    }

}
