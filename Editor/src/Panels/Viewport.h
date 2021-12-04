#pragma once
#include "Panel.h"

namespace Kross {
    class Viewport : public Panel
    {
        std::string m_strName;
        Ref<Texture::T2D> m_Canvas;
    public:
        Viewport(const std::string& name, const Ref<Texture::T2D>& canvas);
        virtual void Show(double ts) override;
    };
}
