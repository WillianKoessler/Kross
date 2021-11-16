#include "world.h"

world::world()
	: Layer("world"),
	m_Camera(Kross::makeRef<Kross::Camera3D::FirstPerson>("main_Camera", ar, 90.0f))
	//m_Camera(Kross::makeRef<Kross::Camera2D::OrthographicCtrl>("main_Camera", ar, true))
{
}

void world::OnAttach()
{
	Kross::Renderer::Command::SetClear(0x0f0ff0ff);
	m_Camera->Zoom(5.0f);
	const char* cherno = "assets/textures/ChernoLogo.png";
	const char* checker = "assets/textures/CheckerBoard.png";
	const char* cage = "assets/textures/cage.png";
	const char* cage_mamma = "assets/textures/cage_mamma.png";
	//Kross::Stack<Kross::Texture::T2D>::instance().Get("cage", cage);
	//Kross::Stack<Kross::Texture::T2D>::instance().Get("ck", checker);
	m_Camera->GetCamera()->SetPosition({0.0f, 0.0f, 2.0f});
}

void world::OnDetach()
{
}

void world::OnUpdate(Kross::Timestep ts)
{
	m_Camera->OnUpdate(ts);
	Kross::Renderer::Command::Clear();
	static bool debug = false;
	if (Kross::Input::IsKeyReleased(KROSS_KEY_F1)) debug = !debug;
	Kross::Renderer::Command::SetMode((debug ? Kross::RendererAPI::Mode::Wireframe : Kross::RendererAPI::Mode::Fill));

	if (true) {
		Kross::Renderer3D::ResetStats();
		Kross::Renderer3D::Begin(m_Camera->GetCamera());
		Kross::CubeParams params = { glm::vec3(1.0f), glm::vec3(1.0), glm::vec4(1.0) };
		Kross::Renderer3D::DrawCube(params);
		Kross::Renderer3D::End();
	} else {
		Kross::Renderer2D::ResetStats();
		Kross::Renderer2D::Begin(m_Camera->GetCamera());
		Kross::Renderer2D::BatchBegin();
		Kross::QuadParams params;
		params.position = glm::vec3(0.0f);
		params.texture = Kross::Stack<Kross::Texture::T2D>::instance().Get("ck");
		Kross::Renderer2D::BatchQuad(params);
		Kross::Renderer2D::BatchEnd();
		Kross::Renderer2D::End();
	}
}

void world::OnImGuiRender(Kross::Timestep)
{
}

void world::OnEvent(Kross::Event& event)
{
	m_Camera->OnEvent(event);
}
