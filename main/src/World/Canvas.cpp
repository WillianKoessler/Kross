#include "pch.h"
#include "Canvas.h"

#include <glm/gtc/type_ptr.hpp>

Canvas::Canvas()
	: Layer("Canvas"),
	camera(ar, true)
{
}

void Canvas::OnAttach()
{
	entities.emplace_back(Entity::Props({ 0, 0 }, Entity::EF::Alive | Entity::EF::Solid | Entity::EF::Friendly, "Bob", "assets/textures/Oboro.png"));
	//entities.emplace_back(Entity::Props({ 2, 0 }, Entity::EF::Alive | Entity::EF::Solid, "Skelly", "assets/textures/skelly.png"));
	entities[0].active = true;
}
void Canvas::OnDetach()
{}


void Canvas::OnUpdate(Kross::Timestep ts)
{
	Kross::Renderer2D::ResetStats();
	params.Reset();
	camera.OnUpdate(ts);

	Kross::Renderer2D::Begin(*camera.GetCamera());
	Kross::Renderer::Command::SetClear({ 0.15f, 0.1f, 0.2f, 1.0f });
	Kross::Renderer::Command::Clear();

	params.position = { -1.0f,-1.0f };
	params.texture = Kross::Stack<Kross::Texture::T2D>::get().Get("ChernoLogo", "assets/textures/ChernoLogo.png");
	Kross::Renderer2D::BatchQuad(params);

	params.position.x -= 0.5f;
	params.texture = Kross::Stack<Kross::Texture::T2D>::get().Get("cage", "assets/textures/cage.png");
	params.FlipY();
	Kross::Renderer2D::BatchQuad(params);
	params.FlipY();

	for(auto& e : entities)
	{
		//if (Kross::Input::IsMouseButtonPressed(KROSS_MOUSE_BUTTON_1)) params.position = camera.GetCamera()->GetPosition();
		//Kross::Renderer2D::BatchQuad(params);
		e.Input(camera, ts);
		e.OnUpdate(ts);
		e.DrawSelf();
	}

	for (int i = 1; i <= size; i++)
		for (int j = 1; j < size; j++)
		{
			params.position = { i, j };
			//params.texture = 0;
			params.color = { i / size, j / size, i / size, j / size };
			Kross::Renderer2D::BatchQuad(params);
		}
	Kross::Renderer2D::End();
}

void Dockspace(bool* p_open);
void Canvas::OnImGuiRender(Kross::Timestep ts)
{
	using namespace ImGui;
	static bool main_window = true;
	Dockspace(&main_window);
	{
		static bool show_demo_window = false;
		static bool show_rendererStats = false;
		static bool show_viewport = false;
		static bool fullscreen = false;
		static bool pause_frame_plot_animation = false;

		static size_t timer = 0;
		static float FrameRate = 0;
		static float framerate_buffer = 0;
		static int rate_tick = 10;
		static std::vector<float> frame_times;

		if(!pause_frame_plot_animation) frame_times.push_back(1 / ts);
		if (frame_times.size() > 120) frame_times.erase(frame_times.begin());

		framerate_buffer += 1 / ts;
		if (timer++ % rate_tick == 0) { FrameRate = framerate_buffer / rate_tick; framerate_buffer = 0; }

		if(show_demo_window) ShowDemoWindow(&show_demo_window);
		if(show_rendererStats)
		{
			Begin("Renderer Stats", &show_rendererStats);
			Text("MaxQuadCount: %d", Kross::Renderer2D::getStats().maxQuads);
			Text("Quad Count: %d", Kross::Renderer2D::getStats().QuadCount);
			Text("Draw Calls: %d", Kross::Renderer2D::getStats().DrawCount);
			End(); 
		}
		Kross::Application::Get().GetWindow().FullScreen(fullscreen);

		if (!Begin("Main Window", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
		{
			End();
		}
		else
		{
			Checkbox("Pause Animation", &pause_frame_plot_animation);
			ImGui::PlotLines("Frame Times", frame_times.data(), (int)frame_times.size());
			Text("FPS: %.1f", FrameRate); SameLine(); Checkbox("FullScreen", &fullscreen);
			auto& mouse = Kross::Input::GetMousePos();
			Text("Mouse: %.1f, %.1f", mouse.x, mouse.y);
			Checkbox("Demo Window", &show_demo_window);
			Checkbox("Show Renderer Stats", &show_rendererStats);
			Text("Entities: ");
			for (auto& e : entities)
			{
				Text("    "); SameLine(); Checkbox((e.GetName() + " Window").c_str(), &e.debugWindow);
				if (e.debugWindow) {
					auto& p = e.GetProps();
					Begin(e.GetName().c_str(), &e.debugWindow);
					Text("Position: X=%.1f, Y=%.1f", p.pos.x, p.pos.y);
					Text("Velocity: X=%.10f, Y=%.10f", p.vel.x, p.vel.y);
					Text("HP %d/%d", e.hp, e.mhp);
					Text("State: %d", e.myState);
					Text("Direction: %d", e.myDirection);
					Checkbox("Inputs", &e.active);
					SliderFloat("Speed", &e.speed, 0.0f, 1.0f);
					SliderFloat("sprite_speed", &e.sprite_speed, 0.0f, 1.0f);
					SliderFloat("dump", &e.dump, 0.0f, 1.0f);
					SliderFloat2("vel", glm::value_ptr(p.vel), -.1f, .1f);
					End();
				}
			}
			End();
		}
		static bool once = false;
		if (!once) { SetNextWindowSize(ImVec2(400, 200)); once = true; }
		if (!Begin("Viewport", &show_viewport))
			End();
		else
		{
			Image((void*)(intptr_t)Kross::Renderer2D::GetFrameBuffer()->GetKrossTexture()->GetID(),
				GetContentRegionAvail(),
				ImVec2(0,1),
				ImVec2(1,0));
			ImVec2 size = GetWindowSize();
			camera.OnEvent(Kross::WindowResizeEvent((uint32_t)size.x, (uint32_t)size.y));
			//camera.OnWindowResized(Kross::WindowResizeEvent(size.x, size.y));
			End();
		}
	}
}

void Canvas::OnEvent(Kross::Event& event)
{
	//camera.OnEvent(event);
}


void Dockspace(bool* p_open)
{
	static bool opt_fullscreen_persistant = true;
	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	bool opt_fullscreen = opt_fullscreen_persistant;

	// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
	// because it would be confusing to have two docking targets within each others.
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
	if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace", p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	// Dockspace
	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
	}

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::Separator();
			if (ImGui::MenuItem("Close", NULL, false, p_open != NULL))
				*p_open = false;
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}

	ImGui::End();
}
