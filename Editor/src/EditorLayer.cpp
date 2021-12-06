#include "EditorLayer.h"

#include "Panels/DockSpace.h"
#include "Panels/RendererStats.h"
#include "Panels/Viewport.h"
#include <glm/gtc/type_ptr.hpp>


namespace Kross {
	EditorLayer::EditorLayer()
		: Layer("EditorLayer"),
		m_Camera(makeRef<Camera3D::FirstPerson>("main_Camera", ((float)Application::Get().GetWindow().GetWidth()) / ((float)Application::Get().GetWindow().GetHeight()), 90.0f))
	{
	}

	void EditorLayer::OnAttach()
	{
		RenderCommand::SetClear(0x0a0a0fFF);

		m_Frame = FrameBuffer::CreateRef("EditorLayer_Framebuffer", {800, 600, 1, false});

		Stack<Texture::T2D>::Get("cherno", "assets/textures/ChernoLogo.png");
		Stack<Texture::T2D>::Get("ck", "assets/textures/CheckerBoard.png");
		Stack<Texture::T2D>::Get("cage", "assets/textures/cage.png");
		Stack<Texture::T2D>::Get("mamma", "assets/textures/cage_mamma.png");

		Panel::AppManager().s_bKeyboardEnabled = true;

		m_pPanels.push_back(new DockSpace("Kross"));
		m_pPanels.push_back(new RendererStats("Renderer Status"));
		m_pPanels.push_back(new Viewport("Viewport", m_Frame));

		RenderCommand::BackCull(false);
	}
	void EditorLayer::OnDetach()
	{
		for (int i = (int)m_pPanels.size() - 1; i > -1; i--) delete m_pPanels[i];
		m_Frame->unBind();
	}

	void EditorLayer::OnUpdate(double ts)
	{
		m_Frame->Bind();
		m_Camera->OnUpdate(ts);
		Renderer2D::ResetStats();
		RenderCommand::Clear();

		static bool debug = false;
		if (Kross::Input::IsKeyReleased(Kross::Key::F1)) debug = !debug;
		Kross::RenderCommand::SetMode((debug ? Kross::RendererAPI::Mode::Wireframe : Kross::RendererAPI::Mode::Fill));

		static glm::vec3 cubePos = { 0.0f, 0.0f, 0.0f };
		static glm::vec3 cubeVel = { 0.0f, 0.0f, 0.0f };
		glm::vec3 cubeAcc = { 0.0f, 0.0f, 0.0f };

		float speed = (float)ts * 8.0f;
		if (Kross::Input::IsKeyHeld(Kross::Key::Up)) cubeAcc.y += speed;
		if (Kross::Input::IsKeyHeld(Kross::Key::Down)) cubeAcc.y -= speed;
		if (Kross::Input::IsKeyHeld(Kross::Key::Right)) cubeAcc.x += speed;
		if (Kross::Input::IsKeyHeld(Kross::Key::Left)) cubeAcc.x -= speed;
		
		cubeVel = cubeVel * 0.9f + cubeAcc;
		cubePos += cubeVel;
		static float count = 0.0f;
		count += speed;
		cubePos.z = glm::sin(count)*2.0f-1.0f;

		Renderer2D::Begin(m_Camera->GetCamera());

		QuadParams params;
		params.texture = Stack<Texture::T2D>::Get("mamma");
		auto face = [&params](const glm::vec3& pos, int i, const glm::vec3& rot = glm::vec3(0.0f)) {
			params.position = pos;
			if (i < 0) params.rotation = rot;
			else if (i == 4) params.rotation = { glm::pi<float>() * 1.5f, 0.0f, 0.0f };
			else if (i == 5) params.rotation = { glm::pi<float>() * 0.5f, 0.0f, 0.0f };
			else params.rotation = { 0.0f, i * glm::pi<float>() / 2.0f, 0.0f };
			Renderer2D::BatchQuad(params);
		};

		face(cubePos + glm::vec3(-.5f, -.5f, .5f), 0);
		face(cubePos + glm::vec3(0.5f, .0f, .0f), 1);
		face(cubePos + glm::vec3(0.0f, .0f, -.5f), 2);
		face(cubePos + glm::vec3(-.5f, .0f, .0f), 3);
		face(cubePos + glm::vec3(0.0f, .5f, .0f), 4);
		face(cubePos + glm::vec3(0.0f, -.5f, .0f), 5);

		params.texture = Stack<Kross::Texture::T2D>::Get("ck");
		params.color = { 1.0f, 0.0f, 0.0f, 1.0f }; face({ 0.0f, 0.5f, 0.5f }, 1);
		params.color = { 0.0f, 1.0f, 0.0f, 1.0f }; face({ 0.5f, 0.0f, 0.5f }, 4);
		params.color = { 0.0f, 0.0f, 1.0f, 1.0f }; face({ 0.0f, 0.0f, 0.0f }, 0);

		Kross::Renderer2D::End();

		m_Frame->unBind();
	}

	void EditorLayer::OnImGuiRender(double ts)
	{
		for(auto& p : m_pPanels) p->Show(ts);
		Panel::setFlag(ImGuiConfigFlags_ViewportsEnable, Panel::AppManager().s_bViewportEnabled);
	}

	void EditorLayer::OnEvent(Event& e)
	{
		m_Camera->OnEvent(e);
	}
}
