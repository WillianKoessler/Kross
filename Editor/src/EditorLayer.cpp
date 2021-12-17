#include "Editor_pch.h"
#include "EditorLayer.h"

#include "Panels.h"
#include <glm/gtc/type_ptr.hpp>
#include "SceneCameraController.h"

namespace Kross {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_ViewportSize(400.0f, 400.0f), m_Scene(makeRef<Scene>("main"))
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

		Panel::AppManager().s_bKeyboardEnabled = true;

		rs = new RendererStats();
		sh = new SceneHierarchy(m_Scene);
		ep = new EntityProperties(m_Scene);
	}
	void EditorLayer::OnDetach()
	{
		delete rs;
		delete sh;
		delete ep;
		m_Frame->unBind();
	}

	void EditorLayer::OnUpdate(double ts)
	{
		if (Input::IsKeyPressed(Key::F1)) {
			SceneSerializer ss(m_Scene);
			ss.Serialize("assets/scenes/demo.kross");
		}
		if (Input::IsKeyPressed(Key::F2)) {
			m_Scene = makeRef<Scene>("main");
			sh->SetContext(m_Scene);
			SceneSerializer ss(m_Scene);
			ss.Deserialize("assets/scenes/demo.kross");
		}
		if (Input::IsKeyPressed(Key::Space)) TagComponent::ShowAll();
		if (Input::IsKeyPressed(Key::Enter)) KROSS_TRACE("----------------------------------------------------------------");
		if (!Panel::Manager().s_bDockspace) Application::Get().OnEvent(WindowCloseEvent());
		m_Frame->Resize(m_ViewportSize);
		m_Camera.SetViewportSize(m_ViewportSize);

		m_Frame->Bind();
		RenderCommand::Clear();
		if (Panel::AppManager().s_bEditorCamera) {
			m_Camera.OnUpdate(ts);
			m_Scene->OnUpdateEditor(ts, m_Camera);
		} else m_Scene->OnUpdateRuntime(ts);
		m_Frame->unBind();
	}

	void EditorLayer::OnImGuiRender(double ts)
	{
		Dockspace();
		rs->Show(ts);
		sh->Show(ts);
		ep->Show(ts);
		Panel::setFlag(ImGuiConfigFlags_ViewportsEnable, Panel::AppManager().s_bViewportEnabled);
		Panel::setFlag(ImGuiConfigFlags_NavEnableKeyboard, Panel::AppManager().s_bKeyboardEnabled);
		Panel::setFlag(ImGuiConfigFlags_NavEnableGamepad, Panel::AppManager().s_bGamepadEnabled);
		Viewport();
	}

	void EditorLayer::OnEvent(Event &e)
	{
		if (Panel::AppManager().s_bEditorCamera) m_Camera.OnEvent(e);
	}
}
