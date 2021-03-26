#include "pch.h"
#include "Canvas.h"

#include <glm/gtc/type_ptr.hpp>

Canvas::Canvas()
	: Layer("Canvas"),
	camera(Kross::makeRef<Kross::Camera::Ortho2DCtrl>(ar, true))
{
}

void Canvas::OnAttach()
{
	const char* cherno = "assets/textures/ChernoLogo.png";
	const char* checker = "assets/textures/CheckerBoard.png";
	const char* cage = "assets/textures/cage.png";
	const char* cage_mamma = "assets/textures/cage_mamma.png";
	Kross::Stack<Kross::Texture::T2D>::instance().Get("cage", cage);
	
	Kross::Renderer::Command::SetClear(0x0f0f0fff);
	entities.emplace_back(Entity::Props(glm::vec3(0.0f), Entity::EF::Alive | Entity::EF::Solid | Entity::EF::Friendly, "Bob", "assets/textures/character.png"));
	entities.emplace_back(Entity::Props({ 2.0f, 0.0f, 0.0f }, Entity::EF::Alive | Entity::EF::Solid, "Skelly", "assets/textures/skelly.png"));
	entities[0].active = true;

	Kross::Stack<Kross::Shader>::instance().Get()
}
void Canvas::OnDetach()
{
}


void Canvas::OnUpdate(Kross::Timestep ts)
{
	Kross::Renderer2D::ResetStats();
	camera->OnUpdate(ts);

	acc.y = (Kross::Input::IsKeyPressed(KROSS_KEY_UP) - Kross::Input::IsKeyPressed(KROSS_KEY_DOWN)) * ts;
	acc.x = (Kross::Input::IsKeyPressed(KROSS_KEY_RIGHT) - Kross::Input::IsKeyPressed(KROSS_KEY_LEFT)) * ts;

	Kross::Renderer::Command::Clear();
	Kross::Renderer2D::Begin(*camera->GetCamera());
	Kross::Renderer2D::BatchBegin();
	for (auto& e : entities)
	{
		if(e.active) e.SetAcc(acc);
		e.Input(ts);
		e.OnUpdate(ts);
		e.DrawSelf();
	}
	static float r = 0.0f;
	r += 0.6283f * ts;
	params.rotation = r;
	params.texture = Kross::Stack<Kross::Texture::T2D>::instance().Get("cage");
	for (int i = 1; i <= size; i++)
		for (int j = 1; j < size; j++)
		{
			params.position = { i-5, j-5, 0.0f };
			params.color = { i / size, j / size, i / size, j / size };
			Kross::Renderer2D::BatchQuad(params);
		}
	Kross::Renderer2D::BatchEnd();
	Kross::Renderer2D::End();
}

void Canvas::OnImGuiRender(Kross::Timestep ts)
{
	using namespace ImGui;
	{
		static bool show_demo_window = false;
		static bool show_rendererStats = false;
		static bool fullscreen = false;

		static size_t timer = 0;
		static float FrameRate = 0;
		static float framerate_buffer = 0;

		framerate_buffer += 1 / ts;
		if (timer++ % 10 == 0) { FrameRate = framerate_buffer / 10; framerate_buffer = 0; }

		if (show_demo_window) ShowDemoWindow(&show_demo_window);
		if (show_rendererStats)
		{
			Begin("Renderer Stats", &show_rendererStats);
			Text("Quad Count: %d", Kross::Renderer2D::getStats().QuadCount);
			Text("Draw Calls: %d", Kross::Renderer2D::getStats().DrawCount);
			End();
		}
		Kross::Application::Get().GetWindow().FullScreen(fullscreen);

		Begin("Main Window");
		Text("FPS: %.1f", FrameRate); SameLine(); Checkbox("FullScreen: ", &fullscreen);
		Checkbox("Demo Window", &show_demo_window);
		Checkbox("Show Renderer Stats", &show_rendererStats);
		Text("Entities: ");
		ImGui::Indent(30);
		for (auto& e : entities)
		{
			Checkbox((e.GetName() + " Window").c_str(), &e.debugWindow);
			if (e.debugWindow) {
				Begin(e.GetName().c_str(), &e.debugWindow);
				Text("Position: X=%.1f, Y=%.1f", e.GetX(), e.GetY());
				Text("Velocity: X=%.3f, Y=%.3f", e.GetVel().x, e.GetVel().y);
				Text("HP %d/%d", e.hp, e.mhp);
				Text("State: %d", e.myState);
				Text("Direction: %d", e.myDirection);
				Checkbox("Inputs", &e.active);
				Text("sprite_speed: "); SameLine(); SliderFloat("###1", &e.sprite_speed, 0.0f, 1.0f);
				Text("dump: "); SameLine(); SliderFloat("###2", &e.dump, 0.0f, 1.0f);
				SliderFloat2("vel", glm::value_ptr(acc), -0.01f, 0.01f);
				End();
			}
		}
		End();
	}
}

void Canvas::OnEvent(Kross::Event& event)
{
	camera->OnEvent(event);
}