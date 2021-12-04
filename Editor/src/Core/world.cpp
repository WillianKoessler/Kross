#include "world.h"

#include <glm/gtc/type_ptr.hpp>

static bool window = false;

namespace Kross {

	world::world()
		: Layer("world")
	{
	}

	void world::OnAttach()
	{
		if (true) m_Camera = makeRef<Camera3D::FirstPerson>("main_Camera", ar, 90.0f);
		else      m_Camera = makeRef<Camera2D::OrthographicCtrl>("main_Camera", ar, true);

		m_Scene = makeRef<Scene>();
		m_SquareEntity = m_Scene->CreateEntity("Square");
		KROSS_TRACE("{0}", m_SquareEntity.HasComponent<TagComponent>());
		//m_SquareEntity.AddComponent<Kross::SpriteComponent>(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		//m_SquareEntity.GetComponent<Kross::TagComponent>()->print();

		//m_CameraEntity = m_Scene->CreateEntity("Basic Camera");
		//m_CameraEntity.AddComponent<Kross::CameraComponent>(m_Camera->GetCamera());
		//m_Scene->SetPrimaryCamera(m_CameraEntity);

		RenderCommand::SetClear({ 0.1f, 0.1f, 0.1f, .1f });
		m_Camera->Zoom(5.0f);
		Stack<Texture::T2D>::instance().Get("cherno", "assets/textures/ChernoLogo.png");
		Stack<Texture::T2D>::instance().Get("ck", "assets/textures/CheckerBoard.png");
		params.texture = Stack<Texture::T2D>::instance().Get("cage", "assets/textures/cage.png");
		Stack<Texture::T2D>::instance().Get("mamma", "assets/textures/cage_mamma.png");
		m_Camera->GetCamera()->SetPosition({ 0.0f, 0.0f, -1.0f });
		params.texture = nullptr;
	}

	void world::OnDetach()
	{
		Stack<Texture::T2D>::instance().Del("cage");
		Stack<Texture::T2D>::instance().Del("ck");
	}

	void world::OnUpdate(double ts)
	{
		m_Camera->OnUpdate(ts);

		RenderCommand::Clear();
		if (Input::IsKeyReleased(Key::Enter)) window = !window;

		static bool debug = false;
		if (Input::IsKeyReleased(Key::F1)) debug = !debug;
		RenderCommand::SetMode((debug ? RendererAPI::Mode::Wireframe : RendererAPI::Mode::Fill));


		Renderer2D::ResetStats();





		//Renderer2D::Begin(m_Camera->GetCamera());
		//Renderer2D::BatchBegin();
		//Renderer2D::BatchQuad(params);
		//Renderer2D::BatchEnd();
		//Renderer2D::End();
	}

	void world::OnEvent(Kross::Event& event)
	{
		m_Camera->OnEvent(event);
	}
}
