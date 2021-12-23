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

		ActionManager::RegisterAction("NewScene", Key::Control, Key::N, "Resets the Scene to a new one.");
		ActionManager::RegisterAction("OpenScene", Key::Control, Key::O, "Load a Scene from a file.");
		ActionManager::RegisterAction("SaveScene", Key::Control, Key::S, "Save the Scene to the file that is already open.");
		ActionManager::RegisterAction("SaveSceneAs", Key::Shift, Key::Control, Key::S, "Save the Scene to a file.");
		ActionManager::RegisterKeyAction("SelectionTool", Key::Q);
		ActionManager::RegisterKeyAction("TranslationTool", Key::W);
		ActionManager::RegisterKeyAction("RotationTool", Key::E);
		ActionManager::RegisterKeyAction("ScaleTool", Key::R);
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

		if (ActionManager::IsActionPerformed("NewScene")) {
			m_Scene = Scene("main");
			sceneHierarchy->SetContext(m_Scene);
			entityProperties->SetContext(m_Scene);
		}
		if (ActionManager::IsActionPerformed("OpenScene")) {
			m_Scene = Scene();
			sceneHierarchy->SetContext(m_Scene);
			entityProperties->SetContext(m_Scene);
			m_Scene.LoadScene();
		}
		if (ActionManager::IsActionPerformed("SaveScene")) {
			m_Scene.SaveScene();
		}
		if (ActionManager::IsActionPerformed("SaveSceneAs")) {
			m_Scene.SaveScene(FileDialog::SaveFile("Kross Scene (.kross)\0*.kross\0\0"));
		}
		if (ActionManager::IsActionPerformed("SelectionTool")) {
			m_GuizmoType = -1;
		}
		if (ActionManager::IsActionPerformed("TranslationTool")) {
			m_GuizmoType = ImGuizmo::TRANSLATE;
		}
		if (ActionManager::IsActionPerformed("RotationTool")) {
			m_GuizmoType = ImGuizmo::ROTATE;
		}
		if (ActionManager::IsActionPerformed("ScaleTool")) {
			m_GuizmoType = ImGuizmo::SCALE;
		}

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
	}
}
