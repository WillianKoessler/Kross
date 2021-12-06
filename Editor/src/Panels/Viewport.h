#pragma once
#include "Panel.h"

namespace Kross {
    class Viewport : public Panel
    {
        Ref<FrameBuffer> m_Canvas;
        glm::uvec2 m_ViewportSize;
    public:
        Viewport(const char* name, const Ref<FrameBuffer>& frame);
        virtual void Show(double ts) override;
    };
}
