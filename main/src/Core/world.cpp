#include "world.h"

static glm::vec2 uv0 = { 256, 256 };
static glm::vec2 uv1 = { 0, 0 };

world::world()
	: Layer("world"),
	m_Camera(Kross::makeRef<Kross::Camera3D::FirstPerson>("main_Camera", ar, 90.0f))
	//m_Camera(Kross::makeRef<Kross::Camera2D::OrthographicCtrl>("main_Camera", ar, true))
{

}

void world::OnAttach()
{
	Kross::Renderer::Command::SetClear({ 0.1f, 0.0f, 0.1f, .1f });
	m_Camera->Zoom(5.0f);
	const char* cherno = "assets/textures/ChernoLogo.png";
	const char* checker = "assets/textures/CheckerBoard.png";
	const char* cage = "assets/textures/cage.png";
	const char* cage_mamma = "assets/textures/cage_mamma.png";
	Kross::Stack<Kross::Texture::T2D>::instance().Get("cage", cage);
	Kross::Stack<Kross::Texture::T2D>::instance().Get("ck", checker);
	m_Camera->GetCamera()->SetPosition({ 0.0f, 0.0f, 5.0f });
}

void world::OnDetach()
{
}

void world::OnUpdate(double ts)
{
	m_Camera->OnUpdate(ts);
	m_Camera->GetCamera()->LookAt({ 3.0f, 0.0f, 0.0f });
	Kross::Renderer::Command::Clear();
	static bool debug = false;
	if (Kross::Input::IsKeyReleased(KROSS_KEY_F1)) debug = !debug;
	if (Kross::Input::IsKeyReleased(KROSS_KEY_ENTER)) {
		const glm::vec3& rotation = m_Camera->GetCamera()->GetRotation();
		KROSS_TRACE("{0}, {1}, {2}", rotation.x, rotation.y, rotation.z);
	}
	Kross::Renderer::Command::SetMode((debug ? Kross::RendererAPI::Mode::Wireframe : Kross::RendererAPI::Mode::Fill));


	Kross::Renderer3D::ResetStats();
	Kross::Renderer3D::Begin(m_Camera->GetCamera());
	//static Kross::QuadParams params;
	static Kross::Params3D params;

	if (Kross::Input::IsKeyHeld(KROSS_KEY_RIGHT))	params.position.x -= (float)ts * 50.0f;
	if (Kross::Input::IsKeyHeld(KROSS_KEY_LEFT))	params.position.x += (float)ts * 50.0f;
	if (Kross::Input::IsKeyHeld(KROSS_KEY_UP))		params.position.y += (float)ts * 50.0f;
	if (Kross::Input::IsKeyHeld(KROSS_KEY_DOWN))	params.position.y -= (float)ts * 50.0f;

	Kross::Renderer3D::BatchBegin();
	//Kross::Renderer3D::BatchQuad(params);
	Kross::Renderer3D::DrawCube(params);
	Kross::Renderer3D::BatchEnd();
	Kross::Renderer3D::End();
}

void world::OnImGuiRender(double)
{
}
/*
{
	bool window = true;
	ImGui::ShowDemoWindow(&window);

	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGui::Begin("main_dockspace");
	ImGui::DockSpace(ImGui::GetID("main_dockspace"));
	ImGui::Begin("window", &window);
//	ImGui::Image((void*)(uintptr_t)framebuffer->GetColorAttachmentRendererID(), ImVec2(0,0), ImVec2(-200,-200));
	ImGui::Image((void*)(uintptr_t)Kross::Stack<Kross::Texture::T2D>::instance().Get("cage")->GetID(),
		ImGui::GetContentRegionAvail(), { 0, 2 });
	ImGui::End();
	ImGui::End();
}
//*/
void world::OnEvent(Kross::Event& event)
{
	m_Camera->OnEvent(event);
}
