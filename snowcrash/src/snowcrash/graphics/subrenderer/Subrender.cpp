
#include "Subrender.hpp"

namespace SC
{
    Subrender::Subrender(GraphicsLayer *layer)
        : m_graphicsLayer(layer) {}

    Subrender::~Subrender()
    {
        for (int i = 0; i < m_subpasses.GetIndex(); i++)
        {
            delete m_subpasses[i];
        }
    }

    void Subrender::RenderPasses(VkCommandBuffer buffer)
    {
        for (int i = 0; i < m_subpasses.GetIndex(); i++)
        {
            m_subpasses[i]->Render(buffer);
        }
    }

    Pass::Pass(Subrender *subrender)
        : m_subrender(subrender) {}

} // namespace SC
