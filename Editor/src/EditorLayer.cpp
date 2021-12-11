#include "Editor_pch.h"
#include "EditorLayer.h"

#include "Panels.h"
#include <glm/gtc/type_ptr.hpp>
#include "SceneCameraController.h"

namespace Kross {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_ViewportSize(400.0f, 400.0f), m_Scene(makeRef<Scene>())
		//m_Camera(makeRef<Camera3D::FirstPerson>("main_Camera", ((float)Application::Get().GetWindow().GetWidth()) / ((float)Application::Get().GetWindow().GetHeight()), 90.0f))
	{
	}

	void EditorLayer::OnAttach()
	{
		RenderCommand::SetClear(0x0a0a32FF);
		//RenderCommand::SetClear(0xFF00FFFF);

		m_Frame = FrameBuffer::CreateRef("EditorLayer_Framebuffer", { 800, 600, 1, false });

		Stack<Texture::T2D>::Get("cherno", "assets/textures/ChernoLogo.png");
		Stack<Texture::T2D>::Get("ck", "assets/textures/CheckerBoard.png");
		Stack<Texture::T2D>::Get("cage", "assets/textures/cage.png");
		m_Placeholder = Stack<Texture::T2D>::Get("mamma", "assets/textures/cage_mamma.png");

		Panel::AppManager().s_bKeyboardEnabled = true;

		m_pPanels.push_back(makeScope<DockSpace>());
		m_pPanels.push_back(makeScope<RendererStats>());
		m_pPanels.push_back(makeScope<SceneHierarchy>(m_Scene));
		m_pPanels.push_back(makeScope<EntityProperties>(m_Scene));

		Entity sceneCamera = m_Scene->CreateEntity("SceneCamera");
		sceneCamera.Add<CameraComponent>();
		sceneCamera.Add<NativeScriptComponent>()->Bind<CameraController>();

		m_Scene->CreateEntity("mySquare").Add<SpriteComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		m_Scene->CreateEntity("otherSquare").Add<SpriteComponent>();

		m_Scene->CreateEntity("Second Camera").Add<CameraComponent>();
		auto camera = m_Scene->CreateEntity("Third Camera");
		camera.Add<CameraComponent>();
		camera.Remove<TransformComponent>();

		RenderCommand::BackCull(true);
	}
	void EditorLayer::OnDetach()
	{
		m_Frame->unBind();
	}

	void EditorLayer::OnUpdate(double ts)
	{
		if (!Panel::Manager().s_bDockspace) Application::Get().OnEvent(WindowCloseEvent());
		m_Frame->Resize(m_ViewportSize);
		m_Camera.SetViewportSize(m_ViewportSize);
		m_Scene->OnViewportResize(m_ViewportSize);

		m_Frame->Bind();
		RenderCommand::Clear();
		if (Panel::AppManager().s_bEditorCamera) {
			m_Camera.OnUpdate(ts);
			m_Scene->OnUpdateEditor(ts, m_Camera);
		} else m_Scene->OnUpdateRuntime(ts);
		m_Frame->unBind();

		m_bMouseClick = false;
		m_bMouseMoved = false;
	}

	void EditorLayer::OnImGuiRender(double ts)
	{
		for (auto &p : m_pPanels) p->Show(ts);
		Panel::setFlag(ImGuiConfigFlags_ViewportsEnable, Panel::AppManager().s_bViewportEnabled);
		Panel::setFlag(ImGuiConfigFlags_NavEnableKeyboard, Panel::AppManager().s_bKeyboardEnabled);
		Panel::setFlag(ImGuiConfigFlags_NavEnableGamepad, Panel::AppManager().s_bGamepadEnabled);

		if (Panel::Manager().s_bViewport) {
			static bool m_bBackFaceCull = false;
			static bool m_bWireFrame = false;
			static auto m_Flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

			RenderCommand::BackCull(m_bBackFaceCull);
			RenderCommand::SetMode(m_bWireFrame ? RendererAPI::Mode::Wireframe : RendererAPI::Mode::Fill);

			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			ImGui::Begin("Viewport", &Panel::Manager().s_bViewport, m_Flags);

			PassEvents(ImGui::IsWindowFocused() && ImGui::IsWindowHovered());

			if (ImGui::BeginMenuBar())
			{
				if (ImGui::BeginMenu("Settings")) {
					ImGui::MenuItem("BackFaceCulling", NULL, &m_bBackFaceCull);
					ImGui::MenuItem("WireFrame", NULL, &m_bWireFrame);
					ImGui::EndMenu();
				}
				ImGui::EndMenuBar();
			}

			ImVec2 ImGuiViewportPanelSize = ImGui::GetContentRegionAvail();
			glm::vec2 glmViewportSize = { ImGuiViewportPanelSize.x, ImGuiViewportPanelSize.y };
			
			if (m_ViewportSize != glmViewportSize && ImGuiViewportPanelSize.x > 0.0f && ImGuiViewportPanelSize.y > 0.0f)
					m_ViewportSize = glmViewportSize;

			ImGui::Image(
				(void *)(uintptr_t)m_Frame->GetColorAttachmentID(),
				ImGuiViewportPanelSize,
				ImVec2(0.0f, 1.0f),
				ImVec2(1.0f, 0.0f)
			);
			ImGui::PopStyleVar(3);
			ImGui::End();
		}
	}

	void EditorLayer::OnEvent(Event &e)
	{
		if (Panel::AppManager().s_bEditorCamera) m_Camera.OnEvent(e);
		if (e.GetEventType() == MouseButtonPressedEvent::GetStaticType()) m_bMouseClick = true;
		if (e.GetEventType() == MouseMovedEvent::GetStaticType()) m_bMouseMoved = true;
	}

}
