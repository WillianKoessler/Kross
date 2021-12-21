#include "Editor_pch.h"
#include "EditorLayer.h"

#include "Panels.h"
#include <glm/gtc/type_ptr.hpp>
#include "SceneCameraController.h"

namespace Kross {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_ViewportSize(400.0f, 400.0f), m_Scene("main")
	{
	}

	void EditorLayer::OnAttach()
	{
		RenderCommand::SetClear(*(glm::vec4 *)&ImGui::GetStyle().Colors[ImGuiCol_WindowBg]);

		m_Frame = FrameBuffer::CreateRef("EditorLayer_Framebuffer", { 800, 600, 1, false });

		Stack<Texture::T2D>::Get("cherno", "assets/textures/ChernoLogo.png");
		Stack<Texture::T2D>::Get("ck", "assets/textures/CheckerBoard.png");
		Stack<Texture::T2D>::Get("cage", "assets/textures/cage.png");
		m_Placeholder = Stack<Texture::T2D>::Get("mamma", "assets/textures/cage_mamma.png");

		rendererStats = new RendererStats();
		sceneHierarchy = new SceneHierarchy(m_Scene);
		entityProperties = new EntityProperties(m_Scene);
	}
	void EditorLayer::OnDetach()
	{
		delete rendererStats;
		delete sceneHierarchy;
		delete entityProperties;
		m_Frame->unBind();
	}

	void EditorLayer::OnUpdate(double ts)
	{
		if (!Panel::Manager().s_bDockspace) Application::Get().OnEvent(WindowCloseEvent());
		m_Frame->Resize(m_ViewportSize);
		m_Camera.SetViewportSize(m_ViewportSize);

		m_Frame->Bind();
		if (Panel::AppManager().s_bEditorCamera) {
			m_Camera.OnUpdate(ts);
			m_Scene.OnUpdateEditor(ts, m_Camera);
		} else {
			m_Scene.OnUpdateRuntime(ts);
		}
		m_Frame->unBind();
	}

	void EditorLayer::OnGuiRender(double ts)
	{
		Dockspace();
		rendererStats->Show(ts);
		sceneHierarchy->Show();
		entityProperties->Show();
		Panel::setConfigFlag(ImGuiConfigFlags_ViewportsEnable, Panel::AppManager().s_bViewportEnabled);
		Panel::setConfigFlag(ImGuiConfigFlags_NavEnableKeyboard, Panel::AppManager().s_bKeyboardEnabled);
		Panel::setConfigFlag(ImGuiConfigFlags_NavEnableGamepad, Panel::AppManager().s_bGamepadEnabled);
		Viewport();
	}

	void EditorLayer::OnEvent(Event &e)
	{
		if (Panel::AppManager().s_bEditorCamera) m_Camera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<KeyPressedEvent>(KROSS_BIND_EVENT_FN(EditorLayer::OnKeyPressed));
	}

	bool Kross::EditorLayer::OnKeyPressed(KeyPressedEvent &e)
	{
		auto newScene = [&]() { m_Scene = Scene("main"); sceneHierarchy->SetContext(m_Scene); entityProperties->SetContext(m_Scene); };
		bool ctrl = Input::IsKeyHeld(Key::LeftControl) || Input::IsKeyHeld(Key::RightControl);
		bool shift = Input::IsKeyHeld(Key::LeftShift) || Input::IsKeyHeld(Key::RightShift);
		bool alt = Input::IsKeyHeld(Key::LeftAlt) || Input::IsKeyHeld(Key::RightAlt);
		switch (e.GetKey()) {
			default: break;
			case Key::O: { if (ctrl) { newScene(); m_Scene.LoadScene(); } KROSS_TRACE("Ctrl+O"); break; }
			case Key::N: { if (ctrl) newScene(); KROSS_TRACE("CTRL+N"); break; }
			case Key::S: { 
					if (ctrl && shift) { m_Scene.SaveScene(FileDialog::SaveFile("Kross Scene (.kross)\0*.kross\0\0")); KROSS_TRACE("CTRL+SHIFT+S"); }
					else if (ctrl) { m_Scene.SaveScene(); KROSS_TRACE("CTRL+S"); }
					break;
				}
			case Key::Q: { m_GuizmoType = -1; break; }
			case Key::W: { m_GuizmoType = ImGuizmo::TRANSLATE; break; }
			case Key::E: { m_GuizmoType = ImGuizmo::ROTATE; break; }
			case Key::R: { m_GuizmoType = ImGuizmo::SCALE; break; }
		}
		return false;
	}
}
