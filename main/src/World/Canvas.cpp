#include "pch.h"
#include "Canvas.h"

Canvas::Canvas()
	: Layer("Canvas"),
	camera(ar, true)
{
}

void Canvas::OnAttach()
{
	Kross::Renderer::Command::SetClear({ 0.15f, 0.1f, 0.2f, 1.0f });
	vel = pos = { 0, 0 };
	//entities.emplace_back(Entity::Props({ 0, 0 }, Entity::EF::Alive | Entity::EF::Friendly | Entity::EF::Solid, "Bob", "assets/textures/cage.png"));
	//entities.emplace_back(Entity::Props({ 2, 0 }, Entity::EF::Alive | Entity::EF::Solid, "skelly", "assets/textures/Checkerboard.png"));
}
void Canvas::OnDetach()
{}


void Canvas::OnUpdate(Kross::Timestep ts)
{
	Kross::Renderer2D::ResetStats();
	camera.OnUpdate(ts);

	acc.y = (Kross::Input::IsKeyPressed(KROSS_KEY_UP) - Kross::Input::IsKeyPressed(KROSS_KEY_DOWN)) / 100.0f;
	acc.x = (Kross::Input::IsKeyPressed(KROSS_KEY_RIGHT) - Kross::Input::IsKeyPressed(KROSS_KEY_LEFT)) / 100.0f;
	vel += acc;
	pos += vel *= 0.9f;
	//entities[0].SetAcc({ acc, 0.0f });
	//entities[0].OnUpdate(ts);

	Kross::Renderer::Command::Clear();
	Kross::Renderer2D::Begin(*camera.GetCamera());
	Kross::Renderer2D::BatchBegin();
	//entities[0].DrawSelf();

	params.position = pos;
	params.texture = Kross::Stack<Kross::Texture::T2D>::get().Get("cage");
	params.color = glm::vec4(1.0f);
	Kross::Renderer2D::BatchQuad(params);
	//for (int i = 1; i <= size; i++)
	//	for (int j = 1; j < size; j++)
	//	{
	//		params.position = { i, j };
	//		params.texture = 0;
	//		params.color = { i / size, j / size, i / size, j / size };
	//		Kross::Renderer2D::BatchQuad(params);
	//	}
	Kross::Renderer2D::BatchEnd();
	Kross::Renderer2D::End();
}

void Canvas::OnImGuiRender(Kross::Timestep ts)
{
	using namespace ImGui;
	for (auto& e : entities)
	{
		Begin(e.GetName().c_str());
		Text("Position: X=%.1f, Y=%.1f", e.GetX(), e.GetY());
		Text("HP %d/%d", e.hp, e.mhp);
		End();
	}
	{
		Begin("Renderer Stats");
		Text("Quad Count: %d", Kross::Renderer2D::getStats().QuadCount);
		Text("Draw Calls: %d", Kross::Renderer2D::getStats().DrawCount);
		End();
	}
}

void Canvas::OnEvent(Kross::Event& event)
{
	camera.OnEvent(event);
}
