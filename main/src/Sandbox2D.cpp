#include "Sandbox2D.h"

#include "glm/gtc/type_ptr.hpp"

Sandbox2D::Sandbox2D()
	: Layer("Sandbox2D"),
	w(Kross::Application::Get().GetWindow().GetWidth()),
	h(Kross::Application::Get().GetWindow().GetHeight()),
	demo(false),
	m_Camera(ar,true),
	rotquad1(0.0f),
	rotquad2(0.0f),
	rotbg(0.0f)
{}

void Sandbox2D::OnAttach()
{
	KROSS_PROFILE_FUNC();
	texture = Kross::Textures::T2D::CreateRef("assets/textures/Checkerboard.png");
	Kross::Renderer::Command::SetClear({ 0.2f, 0.1f, 0.2f, 1.0f });

	//TODO BELOW
	//batchlist(2);
}
void Sandbox2D::OnDetach()
{
	KROSS_PROFILE_FUNC();
}
void Sandbox2D::OnUpdate(Kross::Timestep ts)
{
	KROSS_RTP_SCOPE(Update);

	float buffer[8 * (3 + 4)] = {
	-0.5f + pos[0], -0.5f + pos[1], 0.0f,		0.8f, 0.1f, 0.1f, 1.0f,
	 0.5f + pos[0], -0.5f + pos[1], 0.0f,		0.8f, 0.1f, 0.1f, 1.0f,
	 0.5f + pos[0],  0.5f + pos[1], 0.0f,		0.8f, 0.1f, 0.1f, 1.0f,
	-0.5f + pos[0],  0.5f + pos[1], 0.0f,		0.8f, 0.1f, 0.1f, 1.0f,

	 0.5f, -0.5f, 0.0f,		0.1f, 0.1f, 0.8f, 1.0f,
	 1.5f, -0.5f, 0.0f,		0.1f, 0.1f, 0.8f, 1.0f,
	 1.5f,  0.5f, 0.0f,		0.1f, 0.1f, 0.8f, 1.0f,
	 0.5f,  0.5f, 0.0f,		0.1f, 0.1f, 0.8f, 1.0f,
	};

	{
		KROSS_RTP_SCOPE(Camera);
		m_Camera.OnUpdate(ts);
	}

	{
		KROSS_RTP_SCOPE(Clear);
		Kross::Renderer::Command::Clear();
	}

	{
		KROSS_RTP_SCOPE(Begin);
		Kross::Renderer2D::Begin(*m_Camera.GetCamera());

		// TODO: IMPLEMENT THIS

		//batchlist.index(0).position(pos[0], pos[1], 0.0f);
		//batchlist.index(0).color(0.8f, 0.1f, 0.1f, 1.0f);
	}

	{
		KROSS_RTP_SCOPE(Draws);

		// TODO: pass SIZE variable to be automaticaly measuring data pointer size inside the function itself
		Kross::Renderer2D::uploadVBO(buffer, sizeof(buffer));
		
		Kross::Renderer2D::DrawQuad({ 0, 0 }, 1);
		//Kross::Renderer2D::DrawRotatedQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, glm::radians(rotquad1), color1);
		//Kross::Renderer2D::DrawRotatedQuad({ 0.5f, -0.5f }, { 0.5f, 0.75f },glm::radians(rotquad2), color2);
		//Kross::Renderer2D::DrawRotatedQuad({ 0.0f, 0.0f, -0.9f }, 20.0f , texture, glm::radians(rotbg), bgcolor, 10);
	}

	Kross::Renderer2D::End();
}
void Sandbox2D::OnImGuiRender(Kross::Timestep ts)
{
	using namespace ImGui;
	{
		KROSS_RTP_SCOPE(ImGuiRender);
		char buf[128];
		sprintf_s(buf, "Sandbox2D | FPS: %.3f###MainWindow", 1/ts.GetSeconds());
		ImGuiBackendFlags flags = ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoOptions;
		Begin(buf);
		Checkbox("Demo Window", &demo);
		Text("quads"); ImGui::SameLine(); ImGui::ColorEdit4("red", glm::value_ptr(color1), flags); ImGui::SameLine(); ImGui::ColorEdit4("blue", glm::value_ptr(color2), flags);
		Text("background"); ImGui::SameLine(); ImGui::ColorEdit4("background", glm::value_ptr(bgcolor), flags);

		Kross::Timer::read_c([](Kross::Timer::step& t) {
			char label[100];
			strcpy_s(label, "%.3fms - ");
			strcat_s(label, t.cName);
			Text(label, t.fTime);
		});
		End();
	}
	{
		Begin("Rotation");
		SliderFloat("quad1", &rotquad1, 0.0f, 360.0f);
		SliderFloat("quad2", &rotquad2, 0.0f, 360.0f);
		SliderFloat("backg", &rotbg, 0.0f, 360.0f);
		End();
	}
	{
		Begin("Position");
		SliderFloat("x", &pos[0], -10, 10);
		SliderFloat("y", &pos[1], -10, 10);
		End();
	}
	if (demo) ShowDemoWindow(&demo);
}
void Sandbox2D::OnEvent(Kross::Event& e)
{
	m_Camera.OnEvent(e);
}

