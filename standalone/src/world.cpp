#include "world.h"

world::world()
	: Layer("world"),
	m_Camera(Kross::makeRef<Kross::Camera3D::FirstPerson>("main_Camera", ar, 90.0f))
	//m_Camera(Kross::makeRef<Kross::Camera2D::OrthographicCtrl>("main_Camera", ar, true))
{
}

void world::OnAttach()
{
	Kross::RenderCommand::SetClear(0x01010100);
	m_Camera->Zoom(5.0f);
	const char* cherno = "assets/textures/ChernoLogo.png";
	const char* checker = "assets/textures/CheckerBoard.png";
	const char* cage = "assets/textures/cage.png";
	const char* cage_mamma = "assets/textures/cage_mamma.png";
	Kross::Stack<Kross::Texture::T2D>::instance().Get("C", cherno);
	Kross::Stack<Kross::Texture::T2D>::instance().Get("ck", checker);
	Kross::Stack<Kross::Texture::T2D>::instance().Get("cage", cage);
	Kross::Stack<Kross::Texture::T2D>::instance().Get("mm", cage_mamma);
	m_Camera->GetCamera()->SetPosition({ 0.0f, 0.0f, 2.0f });
}

void world::OnDetach()
{
}

void world::OnUpdate(double ts)
{
	m_Camera->OnUpdate(ts);
	Kross::RenderCommand::Clear();
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

	Kross::Renderer2D::Begin(m_Camera->GetCamera());

	Kross::QuadParams params;
	params.texture = Kross::Stack<Kross::Texture::T2D>::instance().Get("mm");

	auto face = [&params](const glm::vec3& pos, int i, const glm::vec3& rot = glm::vec3(0.0f)) {
		params.position = pos;
		if (i < 0) params.rotation = rot;
		else if (i == 4) params.rotation = { glm::pi<float>() * 1.5f, 0.0f, 0.0f };
		else if (i == 5) params.rotation = { glm::pi<float>() * 0.5f, 0.0f, 0.0f };
		else params.rotation = { 0.0f, i * glm::pi<float>() / 2.0f, 0.0f };
		Kross::Renderer2D::BatchQuad(params);
	};

	face(cubePos + glm::vec3(-.5f, -.5f, .5f), 0);
	face(cubePos + glm::vec3(0.5f, .0f, .0f), 1);
	face(cubePos + glm::vec3(0.0f, .0f, -.5f), 2);
	face(cubePos + glm::vec3(-.5f, .0f, .0f), 3);
	face(cubePos + glm::vec3(0.0f, .5f, .0f), 4);
	face(cubePos + glm::vec3(0.0f, -.5f, .0f), 5);

	params.texture = Kross::Stack<Kross::Texture::T2D>::instance().Get("ck");
	params.color = { 1.0f, 0.0f, 0.0f, 1.0f };
	face({ 1.0f, 0.0f, 0.0f }, -1);
	params.color = { 0.0f, 1.0f, 0.0f, 1.0f };
	face({ 0.0f, 1.0f, 0.0f }, -1);
	params.color = { 0.0f, 0.0f, 1.0f, 1.0f };
	face({ 0.0f, 0.0f, 1.0f }, -1);

	Kross::Renderer2D::End();
}

void world::OnImGuiRender(double ts)
{
}

void world::OnEvent(Kross::Event& event)
{
	m_Camera->OnEvent(event);
}
