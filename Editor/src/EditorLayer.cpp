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
		auto &bg = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
		glm::vec4 color(bg.x, bg.y, bg.z, bg.w);
		RenderCommand::SetClear(color);

		FrameBuffer::Specification specs;
		specs.AttachmentsSpecs = { FrameBuffer::Texture::Format::RGBA8, FrameBuffer::Texture::Format::INT, FrameBuffer::Texture::Format::Depth };
		specs.Width = 800;
		specs.Height = 600;
		specs.Samples = 1;
		specs.SwapChainTarget = false;
		m_Frame = FrameBuffer::CreateRef("EditorLayer_Framebuffer", specs);

		Stack<Texture::T2D>::Get("cherno", "assets/textures/ChernoLogo.png");
		Stack<Texture::T2D>::Get("ck", "assets/textures/CheckerBoard.png");
		Stack<Texture::T2D>::Get("cage", "assets/textures/cage.png");
		m_Placeholder = Stack<Texture::T2D>::Get("mamma", "assets/textures/cage_mamma.png");

		rendererStats = new RendererStats();
		sceneHierarchy = new SceneHierarchy(m_Scene);
		entityProperties = new EntityProperties(m_Scene);
		contentBrowser = new ContentBrowser();

		ActionManager::RegisterAction("NewScene", Key::Control, Key::N, [&](const void* data, size_t size) { m_Scene = Scene("main"); sceneHierarchy->SetContext(m_Scene); entityProperties->SetContext(m_Scene); }, "Resets the Scene to a new one.");
		ActionManager::RegisterAction("OpenScene", Key::Control, Key::O, [&](const void* data, size_t size) {
			printf("OpenScene Action Performed\n");
			m_Scene = Scene();
			sceneHierarchy->SetContext(m_Scene);
			entityProperties->SetContext(m_Scene);
			m_Scene.LoadScene((const char*)data);
			}, "Load a Scene from a file.");
		ActionManager::RegisterAction("SaveScene", Key::Control, Key::S, [&](const void* data, size_t size) { m_Scene.SaveScene(); }, "Save the Scene to the file that is already open.");
		ActionManager::RegisterAction("SaveSceneAs", Key::Shift, Key::Control, Key::S, [&](const void* data, size_t size) { m_Scene.SaveScene(FileDialog::SaveFile("Kross Scene (.kross)\0*.kross\0\0")); }, "Save the Scene to a file.");
		ActionManager::RegisterKeyAction("SelectionTool", Key::Q, [&](const void* data, size_t size) { m_GuizmoType = -1; });
		ActionManager::RegisterKeyAction("TranslationTool", Key::W, [&](const void* data, size_t size) { m_GuizmoType = ImGuizmo::TRANSLATE; });
		ActionManager::RegisterKeyAction("RotationTool", Key::E, [&](const void* data, size_t size) { m_GuizmoType = ImGuizmo::ROTATE; });
		ActionManager::RegisterKeyAction("ScaleTool", Key::R, [&](const void* data, size_t size) { m_GuizmoType = ImGuizmo::SCALE; });
	}
	void EditorLayer::OnDetach()
	{
		delete rendererStats;
		delete sceneHierarchy;
		delete entityProperties;
		delete contentBrowser;
		m_Frame->unBind();
	}

	void EditorLayer::OnUpdate(double ts)
	{
		if (!Panel::Manager().s_bDockspace) Application::Get().OnEvent(WindowCloseEvent());

		m_Frame->Resize(m_ViewportSize);
		m_Camera.SetViewportSize(m_ViewportSize);

		m_Frame->Bind();
		RenderCommand::Clear();
		m_Frame->ClearColorAttachment(1, -1);

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
		contentBrowser->Show();
		Panel::setConfigFlag(ImGuiConfigFlags_ViewportsEnable, Panel::AppManager().s_bViewportEnabled);
		Panel::setConfigFlag(ImGuiConfigFlags_NavEnableKeyboard, Panel::AppManager().s_bKeyboardEnabled);
		Panel::setConfigFlag(ImGuiConfigFlags_NavEnableGamepad, Panel::AppManager().s_bGamepadEnabled);
		Viewport();
	}

	void EditorLayer::OnEvent(Event &e)
	{
		if (Panel::AppManager().s_bEditorCamera) m_Camera.OnEvent(e);
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseButtonPressedEvent>(KROSS_BIND_EVENT_FN(EditorLayer::OnMouseClick));
	}
	bool EditorLayer::OnMouseClick(MouseButtonPressedEvent &e)
	{
		if (!ImGuizmo::IsOver() && e.GetMouseButton() == MouseButton::Left) {
			auto [mx, my] = ImGui::GetMousePos();
			mx -= m_ViewportMinBound.x;
			my -= m_ViewportMinBound.y;
			glm::vec2 viewportSize = m_ViewportMaxBound - m_ViewportMinBound;
			my = viewportSize.y - my;
			int mouseX = (int)mx;
			int mouseY = (int)my;
			if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
				m_Frame->Bind();
				int id = m_Frame->ReadPixel(1, mouseX, mouseY);
				m_Frame->unBind();
				m_Scene.Select(id >= 0 ? Entity((entt::entity)id, &m_Scene) : Entity());
			}
		}
		return false;
	}
}
