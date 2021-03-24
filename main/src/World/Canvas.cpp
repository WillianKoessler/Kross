#include "pch.h"
#include "Canvas.h"

Canvas::Canvas()
	: Layer("Canvas"),
	camera(Kross::makeRef<Kross::Camera::Ortho2DCtrl>(ar, true))
{
}

void Canvas::OnAttach()
{
	Kross::Renderer::Command::SetClear(0x0f0f0fff);
	entities.emplace_back(Entity::Props({ 0, 0 }, Entity::EF::Alive | Entity::EF::Friendly | Entity::EF::Solid, "Bob", "assets/textures/character.png"));
	const char* cherno = "assets/textures/ChernoLogo.png";
	const char* checker = "assets/textures/CheckerBoard.png";
	const char* cage = "assets/textures/cage.png";
	const char* cage_mamma = "assets/textures/cage_mamma.png";
	Kross::Stack<Kross::Texture::T2D>::instance().Get("cage", cage);
}
void Canvas::OnDetach()
{
}


void Canvas::OnUpdate(Kross::Timestep ts)
{
	Kross::Renderer2D::ResetStats();
	camera->OnUpdate(ts);

	acc.y = (Kross::Input::IsKeyPressed(KROSS_KEY_UP) - Kross::Input::IsKeyPressed(KROSS_KEY_DOWN)) / 100.0f;
	acc.x = (Kross::Input::IsKeyPressed(KROSS_KEY_RIGHT) - Kross::Input::IsKeyPressed(KROSS_KEY_LEFT)) / 100.0f;

	entities[0].SetAcc(acc);
	entities[0].OnUpdate(ts);

	Kross::Renderer::Command::Clear();
	Kross::Renderer2D::Begin(*camera->GetCamera());
	Kross::Renderer2D::BatchBegin();
	entities[0].DrawSelf();
	static float r = 0.0f;
	r += 0.6283f * ts;
	params.rotation = r;
	params.texture = Kross::Stack<Kross::Texture::T2D>::instance().Get("cage");
	for (int i = 1; i <= size; i++)
		for (int j = 1; j < size; j++)
		{
			params.position = { i-5, j-5 };
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

		Begin("Main Window");
		Text("FPS: %.1f", FrameRate);
		Checkbox("Demo Window", &show_demo_window);
		Checkbox("Show Renderer Stats", &show_rendererStats);
		Text("Entities: ");
		for (auto& e : entities)
		{
			SameLine(); Checkbox((e.GetName() + " Window").c_str(), &e.debugWindow);
			if (e.debugWindow) {
				Begin(e.GetName().c_str(), &e.debugWindow);
				Text("Position: X=%.1f, Y=%.1f", e.GetX(), e.GetY());
				Text("Velocity: X=%.3f, Y=%.3f", e.GetVel().x, e.GetVel().y);
				Text("HP %d/%d", e.hp, e.mhp);
				Text("State: %d", e.myState);
				Text("Direction: %d", e.myDirection);
				Text("sprite_speed: "); SameLine(); SliderFloat("###1", &e.sprite_speed, 0.0f, 1.0f);
				Text("dump: "); SameLine(); SliderFloat("###2", &e.dump, 0.0f, 1.0f);
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