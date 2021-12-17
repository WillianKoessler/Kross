#pragma once
#include "Kross.h"

//#include "Panels/Panel.h"

namespace Kross {
	class DockSpace;
	class RendererStats;
	class SceneHierarchy;
	class EntityProperties;
	class EditorLayer final : public Layer
	{
		Ref<Texture::T2D> m_Placeholder;
		Ref<FrameBuffer> m_Frame;
		Ref<Scene> m_Scene;
		Camera::Editor m_Camera;
		glm::vec2 m_ViewportSize;

		//std::vector<Scope<Panel>> m_pPanels;
		DockSpace *ds = nullptr;
		RendererStats *rs = nullptr;
		SceneHierarchy *sh = nullptr;
		EntityProperties *ep = nullptr;
	public:
		EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(double) override;
		virtual void OnImGuiRender(double) override;
		virtual void OnEvent(Event& e) override;
		void Dockspace();
		void Viewport();
	};
}
