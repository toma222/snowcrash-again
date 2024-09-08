
#pragma once

#include <snowcrash/core/Context.hpp>

#include <panel/Panel.hpp>

namespace editor
{
    // This is a panel that will render debug info for a context
    class ContextPanel : public Panel
    {
    public:
        ContextPanel(SC::Context *context);
        ~ContextPanel();

        void Update() override;

    private:
        SC::Context *m_context;
    };

} // namespace editor
