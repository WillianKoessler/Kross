#include "EditorLayer.h"

#include "Panels/DockSpace.h"
#include "Panels/RendererStats.h"
#include "Panels/Viewport.h"
#include <glm/gtc/type_ptr.hpp>
#include "../assets/scripts/native/SceneCameraController.h"

namespace Kross {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"), m_ViewportSize(0.0f, 0.0f)
		//m_Camera(makeRef<Camera3D::FirstPerson>("main_Camera", ((float)Application::Get().GetWindow().GetWidth()) / ((float)Application::Get().GetWindow().GetHeight()), 90.0f))
	{
	}

	void EditorLayer::OnAttach()
	{
		RenderCommand::SetClear(0x0a0a10FF);
		//RenderCommand::SetClear(0xFF00FFFF);

		m_Frame = FrameBuffer::CreateRef("EditorLayer_Framebuffer", { 800, 600, 1, false });

		Stack<Texture::T2D>::Get("cherno", "assets/textures/ChernoLogo.png");
		Stack<Texture::T2D>::Get("ck", "assets/textures/CheckerBoard.png");
		Stack<Texture::T2D>::Get("cage", "assets/textures/cage.png");
		m_Placeholder = Stack<Texture::T2D>::Get("mamma", "assets/textures/cage_mamma.png");

		Panel::AppManager().s_bKeyboardEnabled = true;

		m_pPanels.push_back(new DockSpace("Kross"));
		m_pPanels.push_back(new RendererStats("Renderer Status"));

		
		m_Scene = makeRef<Scene>();

		Entity sceneCamera = m_Scene->CreateEntity("SceneCamera");
		//SceneCamera cmpt;
		//cmpt.SetOrtho(10.0f, -1.0f, 1.0f);
		//cmpt.SetViewportSize({ m_Frame->GetSpecs().Width, m_Frame->GetSpecs().Height });
		sceneCamera.AddComponent<CameraComponent>();

		sceneCamera.AddComponent<NativeScriptComponent>().Bind<CameraController>();
		m_Scene->SetPrimaryCamera(sceneCamera);



		m_Scene->CreateEntity("mySquare");
		Entity square = m_Scene->GetEntity("mySquare");
		square.AddComponent<SpriteComponent>(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

		RenderCommand::BackCull(true);
	}
	void EditorLayer::OnDetach()
	{
		for (int i = (int)m_pPanels.size() - 1; i > -1; i--) delete m_pPanels[i];
		m_Frame->unBind();
	}

	void EditorLayer::OnUpdate(double ts)
	{
		if (!Panel::Manager().s_bDockspace) Application::Get().OnEvent(WindowCloseEvent());
		m_Camera.OnUpdate(ts);
		m_Frame->Bind();
		Renderer2D::ResetStats();
		RenderCommand::Clear();

		static bool debug = false;
		if (Input::IsKeyReleased(Key::F1)) debug = !debug;
		RenderCommand::SetMode((debug ? RendererAPI::Mode::Wireframe : RendererAPI::Mode::Fill));

		{
			//static glm::vec3 cubePos = { 2.0f, 0.0f, 0.0f };
			//static glm::vec3 cubeVel = { 0.0f, 0.0f, 0.0f };
			//glm::vec3 cubeAcc = { 0.0f, 0.0f, 0.0f };

			//float speed = (float)ts * 8.0f;
			//if (Kross::Input::IsKeyHeld(Kross::Key::Up)) cubeAcc.y += speed;
			//if (Kross::Input::IsKeyHeld(Kross::Key::Down)) cubeAcc.y -= speed;
			//if (Kross::Input::IsKeyHeld(Kross::Key::Right)) cubeAcc.x += speed;
			//if (Kross::Input::IsKeyHeld(Kross::Key::Left)) cubeAcc.x -= speed;

			//cubeVel = cubeVel * 0.9f + cubeAcc;
			//cubePos += cubeVel;
			//static float count = 0.0f;
			//count += speed;
			//cubePos.z = glm::sin(count)*2.0f-1.0f;
			//if (false)
			//{
			//	Renderer2D::Begin(m_camController->GetCamera());

			//	QuadParams params;
			//	auto face = [&params](const glm::vec3 &pos, int i, const glm::vec3 &rot = glm::vec3(0.0f)) {
			//		params.position = pos;
			//		if (i < 0) params.rotation = rot;
			//		else if (i == 4) params.rotation = { glm::half_pi<float>() + glm::pi<float>(), 0.0f, 0.0f };
			//		else if (i == 5) params.rotation = { glm::half_pi<float>(), 0.0f, 0.0f };
			//		else params.rotation = { 0.0f, i * glm::half_pi<float>(), 0.0f };
			//		Renderer2D::BatchQuad(params);
			//	};

			//	params.texture = Stack<Texture::T2D>::Get("mamma");
			//	face(cubePos + glm::vec3(0.0f, 0.0f, 0.5f), 0);
			//	face(cubePos + glm::vec3(0.5f, 0.0f, 0.0f), 1);
			//	face(cubePos + glm::vec3(0.0f, 0.0f, -.5f), 2);
			//	params.texture = Stack<Texture::T2D>::Get("cage");
			//	face(cubePos + glm::vec3(-.5f, 0.0f, 0.0f), 3);
			//	face(cubePos + glm::vec3(0.0f, 0.5f, 0.0f), 4);
			//	face(cubePos + glm::vec3(0.0f, -.5f, 0.0f), 5);

			//	Renderer2D::DebugAxis();
			//	//params.color = { 1.0f, 0.0f, 0.0f, 1.0f }; face({ 1.0f, 0.0f, 0.0f }, 1); // X
			//	//params.color = { 0.0f, 1.0f, 0.0f, 1.0f }; face({ 0.0f, 1.0f, 0.0f }, 4); // Y
			//	//params.color = { 0.0f, 0.0f, 1.0f, 1.0f }; face({ 0.0f, 0.0f, 1.0f }, 0); // Z


			//	Kross::Renderer2D::End();
			//}
		}

		//m_Scene->OnUpdateEditor(ts, m_Camera);
		m_Scene->OnUpdateRuntime(ts);

		m_Frame->unBind();
	}

	void EditorLayer::OnImGuiRender(double ts)
	{
		for (auto &p : m_pPanels) p->Show(ts);
		Panel::setFlag(ImGuiConfigFlags_ViewportsEnable, Panel::AppManager().s_bViewportEnabled);
		Panel::setFlag(ImGuiConfigFlags_NavEnableKeyboard, Panel::AppManager().s_bKeyboardEnabled);
		Panel::setFlag(ImGuiConfigFlags_NavEnableGamepad, Panel::AppManager().s_bGamepadEnabled);

		if (!Panel::Manager().s_bViewport) return;

		static bool m_bBackFaceCull = false;
		static auto m_Flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_MenuBar;

		RenderCommand::BackCull(m_bBackFaceCull);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		ImGui::Begin(GetName(), &Panel::Manager().s_bViewport, m_Flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Settings")) {
				ImGui::MenuItem("BackFaceCulling", NULL, &m_bBackFaceCull);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();
		if (m_ViewportSize != *(glm::vec2 *)&viewportPanelSize && viewportPanelSize.x > 0.0f && viewportPanelSize.y > 0.0f)
		{
			m_ViewportSize = *(glm::vec2*)&viewportPanelSize;
			m_Camera.SetViewportSize(m_ViewportSize);
			m_Frame->Resize(m_ViewportSize);
			m_Scene->GetEntity("SceneCamera").GetComponent<CameraComponent>().camera.SetViewportSize(m_ViewportSize);
		}

		ImGui::Image( (void *)(uintptr_t)m_Frame->GetColorAttachmentID(), viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::PopStyleVar(3);
		ImGui::End();

	}

	void EditorLayer::OnEvent(Event &e)
	{
		m_Camera.OnEvent(e);
	}
}
