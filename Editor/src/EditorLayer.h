#pragma once
#include "Kross.h"

#include "Panels/Panel.h"

namespace Kross {
	class EditorLayer final : public Layer
	{
		Ref<Texture::T2D> m_Placeholder;
		Ref<FrameBuffer> m_Frame;
		std::vector<Scope<Panel>> m_pPanels;
		Ref<Scene> m_Scene;
		Camera::Editor m_Camera;
		glm::vec2 m_ViewportSize;
	public:
		EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(double) override;
		virtual void OnImGuiRender(double) override;
		virtual void OnEvent(Event& e) override;
	};
}
