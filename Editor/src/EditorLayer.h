#pragma once
#include "Kross.h"

#include "Panels/Panel.h"

namespace Kross {
	class EditorLayer final : public Layer
	{
		Ref<Texture::T2D> m_Placeholder;
		glm::vec2 m_ViewportSize;
		Ref<FrameBuffer> m_Frame;
		std::vector<Panel*> m_pPanels;
		Ref<Scene> m_Scene;
		Camera::Editor m_Camera;
	public:
		EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(double) override;
		virtual void OnImGuiRender(double) override;
		virtual void OnEvent(Event& event) override;
	};
}
