#include "world.h"

#include <glm/gtc/type_ptr.hpp>

static bool window = false;

world::world()
	: Layer("world")
{
}

void world::OnAttach()
{
	if (true)
		m_Camera = Kross::makeRef<Kross::Camera3D::FirstPerson>("main_Camera", ar, 90.0f);
	else
		m_Camera = Kross::makeRef<Kross::Camera2D::OrthographicCtrl>("main_Camera", ar, true);

	m_Scene = Kross::makeRef<Kross::Scene>();
	square = m_Scene->CreateEntity("Square");
	square.AddComponent<Kross::SpriteComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

	Kross::RenderCommand::SetClear({ 0.1f, 0.0f, 0.1f, .1f });
	m_Camera->Zoom(5.0f);
	const char* cherno = "assets/textures/ChernoLogo.png";
	const char* checker = "assets/textures/CheckerBoard.png";
	const char* cage = "assets/textures/cage.png";
	const char* cage_mamma = "assets/textures/cage_mamma.png";
	params.texture = Kross::Stack<Kross::Texture::T2D>::instance().Get("cage", cage);
	Kross::Stack<Kross::Texture::T2D>::instance().Get("ck", checker);
	m_Camera->GetCamera()->SetPosition({ 0.0f, 0.0f, -1.0f });
	params.texture = nullptr;
}

void world::OnDetach()
{
	Kross::Stack<Kross::Texture::T2D>::instance().Del("cage");
	Kross::Stack<Kross::Texture::T2D>::instance().Del("ck");
}

void world::OnUpdate(double ts)
{
	m_Camera->OnUpdate(ts);

	Kross::RenderCommand::Clear();
	if (Kross::Input::IsKeyReleased(KROSS_KEY_ENTER)) window = !window;

	static bool debug = false;
	if (Kross::Input::IsKeyReleased(KROSS_KEY_F1)) debug = !debug;
	Kross::RenderCommand::SetMode((debug ? Kross::RendererAPI::Mode::Wireframe : Kross::RendererAPI::Mode::Fill));


	Kross::Renderer2D::ResetStats();
	Kross::Renderer2D::Begin(m_Camera->GetCamera());
	Kross::Renderer2D::BatchBegin();
	m_Scene->OnUpdate(ts);
	Kross::Renderer2D::BatchQuad(params);
	Kross::Renderer2D::BatchEnd();
	Kross::Renderer2D::End();
}

void world::OnImGuiRender(double ts)
{
	if (window)
	{
		using namespace ImGui;
		Begin("window", &window);
		Separator();
		if (square)
		{
			Text("%s", square.GetComponent<Kross::TagComponent>().tag);
			if (square.HasComponent<Kross::SpriteComponent>())
			{
				auto& squareColor = square.GetComponent<Kross::SpriteComponent>().tint;
				ColorEdit3("color", glm::value_ptr(squareColor));
			}
		}
		Separator();
		//Image((void*)(uintptr_t)Kross::Stack<Kross::Texture::T2D>::instance().Get("cage")->GetID(), ImGui::GetContentRegionAvail(), { 0, 2 });
		End();
	}
}

void world::OnEvent(Kross::Event& event)
{
	m_Camera->OnEvent(event);
}

//ImGui::ShowDemoWindow(&window);

//ImGuiViewport* viewport = ImGui::GetMainViewport();
//ImGui::SetNextWindowPos(viewport->Pos);
//ImGui::SetNextWindowSize(viewport->Size);
//ImGui::SetNextWindowViewport(viewport->ID);

//ImGui::Begin("main_dockspace",&window, ImGuiWindowFlags_NoCollapse);
//ImGui::DockSpace(ImGui::GetID("main_dockspace"));
//ImGui::End();
