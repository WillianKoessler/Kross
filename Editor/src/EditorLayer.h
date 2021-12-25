#pragma once
#include "Kross/Core/Core.h"
#include "Kross/Core/Layer.h"
#include "Kross/Events/Event.h"
#include "Kross/Events/KeyEvent.h"
#include "Kross/Scene/Scene.h"
#include "Kross/Renderer/Cameras/EditorCamera.h"

namespace Kross {
	class DockSpace;
	class RendererStats;
	class SceneHierarchy;
	class EntityProperties;
	class EditorLayer final : public Layer
	{
		Ref<Texture::T2D> m_Placeholder;
		Ref<FrameBuffer> m_Frame;
		Scene m_Scene;
		Camera::Editor m_Camera;
		glm::vec2 m_ViewportSize;
		glm::vec2 m_ViewportMinBound;
		glm::vec2 m_ViewportMaxBound;
		int m_GuizmoType = -1;

		RendererStats *rendererStats = nullptr;
		SceneHierarchy *sceneHierarchy = nullptr;
		EntityProperties *entityProperties = nullptr;
	public:
		EditorLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(double) override;
		virtual void OnGuiRender(double) override;
		virtual void OnEvent(Event& e) override;
		bool OnMouseClick(MouseButtonPressedEvent & e);
		void Dockspace();
		void Viewport();
		void ShowGuizmo();
		void Actions();
	};
}
