#pragma once
#include "Panel.h"

namespace Kross {
    class Viewport : public Panel
    {
        Ref<FrameBuffer> m_Canvas;
        void* m_Placeholder;
        glm::vec2 m_ViewportSize;
        bool m_bBackFaceCull = false;
        Camera::Editor &p_editorCamera;
        void Menu();

    public:
        Viewport(const char* name, const Ref<FrameBuffer>& frame, Camera::Editor& camera);
        virtual void Show(double ts) override;
    };
}
