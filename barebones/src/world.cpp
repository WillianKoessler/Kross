#include "world.h"

world::world()
	: Layer("world"),
	m_Camera(Kross::makeRef<Kross::Camera3D::FirstPerson>("main_Camera", ar, 90.0f))
	//m_Camera(Kross::makeRef<Kross::Camera2D::OrthographicCtrl>("main_Camera", ar, true))
{
}

void world::OnAttach()
{
	Kross::RenderCommand::SetClear(0x0f0ff0ff);
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

void world::OnUpdate(double ts)
{
	m_Camera->OnUpdate(ts);
	Kross::RenderCommand::Clear();
	static bool debug = false;
	if (Kross::Input::IsKeyReleased(KROSS_KEY_F1)) debug = !debug;
	Kross::RenderCommand::SetMode((debug ? Kross::RendererAPI::Mode::Wireframe : Kross::RendererAPI::Mode::Fill));
}

void world::OnImGuiRender(double ts)
{
}

void world::OnEvent(Kross::Event& event)
{
	m_Camera->OnEvent(event);
}
