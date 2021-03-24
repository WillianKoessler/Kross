#include "pch.h"
#include "Canvas.h"

static struct windows
{
	bool opt_fullscreen_persistant = true;
	bool fullscreen = false;

	// window

	bool run_application = true;
	bool show_rendererStats = false;
	bool show_camera = true;

	bool show_demo_window = false;
	bool show_style_editor = false;
	bool popup_message_box = false;

	// flags

	bool keyboard = true; int keyboardState = 1;
	bool gamepad = true; int gamepadState = 1;
	bool viewports = false; int viewportsState = false;

}app;

void Canvas::ShowHelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered())
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

Canvas::Canvas()
	: Layer("Canvas"),
	camera(Kross::makeRef<Kross::Camera::Persp3DCtrl>(ar, 90.0f))
{
}

void Canvas::OnAttach()
{
	Kross::Renderer2D::Init();
	Kross::Renderer::Command::SetClear({ 0.2f,0.4f,0.6f,1.0f });
}
void Canvas::OnDetach()
{}


void Canvas::OnUpdate(Kross::Timestep ts)
{
	camera->OnUpdate(ts);
	
	constexpr glm::mat4 mat4i = glm::mat4(1.0f);
	static glm::mat4 rotation = mat4i;
	
	static float theta = 0.0f;
	
	p.x += (Kross::Input::IsKeyPressed(KROSS_KEY_RIGHT) - Kross::Input::IsKeyPressed(KROSS_KEY_LEFT)) * ts * 1;
	p.y += (Kross::Input::IsKeyPressed(KROSS_KEY_UP) - Kross::Input::IsKeyPressed(KROSS_KEY_DOWN)) * ts * 1;
	p.z += (Kross::Input::IsKeyPressed(KROSS_KEY_COMMA) - Kross::Input::IsKeyPressed(KROSS_KEY_PERIOD)) * ts * 1;

	if(Kross::Renderer3D::Is_Initiated())
	{
	
		rotation = glm::rotate(mat4i, glm::radians(theta * 0.5f), { 0.0f, 0.0f, 1.0f });
		rotation = glm::rotate(rotation, glm::radians(theta), { 1.0f, 0.0f, 0.0f });
		
		Kross::Renderer3D::Begin(*camera->GetCamera());
		Kross::Renderer::Command::Clear();
		Kross::Renderer3D::DrawCube(
			glm::vec3(0.0f),
			{ 0.3f, 0.1f, 0.1f },
			0,
			{ 0.0f, 0.0f, 1.0f },
			{ 0.8f, 0.4f, 0.4f, 0.2f }
		);
		Kross::Renderer3D::DrawCube(
			glm::vec3(0.0f),
			{ 0.1f, 0.3f, 0.1f },
			0,
			{ 0.0f, 0.0f, 1.0f },
			{ 0.8f, 0.4f, 0.4f, 0.2f }
		);
		Kross::Renderer3D::DrawCube(
			glm::vec3(0.0f),
			{ 0.1f, 0.1f, 0.3f },
			0,
			{ 0.0f, 0.0f, 1.0f },
			{ 0.8f, 0.4f, 0.4f, 0.2f }
		);
		Kross::Renderer3D::DrawCube(
			glm::vec3(0.0f),
			glm::vec3(0.1f),
			0,
			{ 0.0f, 0.0f, 1.0f },
			{ 0.8f, 0.4f, 0.4f, 0.2f }
		);
		Kross::Renderer3D::DrawCube(
			p,
			glm::vec3(0.1f),
			rotation,
			{ 0.8f, 0.3f, 0.4f, 1.0f }
		);
		Kross::Renderer3D::End();
	}
	else if (Kross::Renderer2D::Is_Initiated())
	{
		Kross::Renderer2D::Begin(*camera->GetCamera());
		Kross::Renderer::Command::Clear();
		Kross::Renderer2D::BatchQuad(Kross::QuadParams());
		Kross::Renderer2D::End();
	}
	else
	{
		KROSS_CORE_WARN("{0} No Renderer Selected", __FUNCTION__);
	}
}

void Canvas::OnDetach()
{
	Kross::Renderer2D::Shutdown();
}

void Canvas::OnImGuiRender(Kross::Timestep ts)
{
	using namespace ImGui;
	auto& io = GetIO();

	ImGui::Begin("cube", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	ImGui::Text("Pos: X %.1f, Y %.1f, Z %.1f", p.x, p.y, p.z);
	ImGui::End();

	uint32_t main_screen = MainScreen(&app.run_application, "Main Window Dockspace");
	if (app.show_rendererStats) Status(&app.show_rendererStats, ts);
	auto& App = Kross::Application::Get();
	if (!app.run_application) App.OnEvent(Kross::WindowCloseEvent());
	if (App.GetWindow().FullScreen(app.fullscreen))
	{
		ToggleFlag(false, io.ConfigFlags, ImGuiConfigFlags_ViewportsEnable);
		app.viewportsState = -1;
		app.viewports = false;
	}
	else app.viewportsState = 0;
	if (app.show_camera)
		camera->DebugWindow();


	static ImGuiWindowFlags viewport_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_NoMove;

	ImGui::SetNextWindowDockID(main_screen, ImGuiCond_Once);
	if (!Begin("Viewport", nullptr, viewport_flags))
		ImGui::End();
	else
	{

		static void* framebuffer = nullptr;
		if (Kross::Renderer3D::Is_Initiated()) framebuffer = (void*)(intptr_t)Kross::Renderer3D::GetFrameBuffer()->GetTexture()->GetID();
		else if (Kross::Renderer2D::Is_Initiated()) framebuffer = (void*)(intptr_t)Kross::Renderer2D::GetFrameBuffer()->GetTexture()->GetID();
		Image(framebuffer,
			GetContentRegionAvail(),
			ImVec2(0, 0),
			ImVec2(1, 1));
		ImVec2 size = GetWindowSize();
		camera->OnEvent(Kross::WindowResizeEvent((uint32_t)size.x, (uint32_t)size.y));
		if (ImGui::IsWindowFocused()) {
			camera->Input(ImGui::IsKeyDown);
		}
		if (ImGui::IsWindowHovered())
			camera->OnEvent(Kross::MouseScrolledEvent(io.MouseWheelH, io.MouseWheel));
		ImGui::End();
	}

}

uint32_t Canvas::MainScreen(bool* p_open, const char* ID)
{
	static const char* id = "";
	ImGuiIO& io = ImGui::GetIO();

	// ImGui Demo Window
	if (app.show_demo_window) ImGui::ShowDemoWindow(&app.show_demo_window);
	// ImGui Style Editor
	if (app.show_style_editor)
	{
		static ImGuiStyle& style = ImGui::GetStyle();

		ImGui::Begin("Style", &app.show_style_editor, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Save", "Ctrl+S")) { SaveStyle(&style); }
				if (ImGui::MenuItem("Save As", NULL, &app.popup_message_box, false)) { id = "Yet Not Implemented"; }
				if (ImGui::MenuItem("Open", "Ctrl+O", &app.popup_message_box)) { id = "Style Open File"; }

				ImGui::Separator();
				if (ImGui::BeginMenu("Options"))
				{
					static bool enabled = true;
					ImGui::MenuItem("Enabled", "", &enabled);
					ImGui::BeginChild("child", ImVec2(0, 60), true);
					for (int i = 0; i < 10; i++)
						ImGui::Text("Scrolling Text %d", i);
					ImGui::EndChild();
					static float f = 0.5f;
					static int n = 0;
					static bool b = true;
					ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
					ImGui::InputFloat("Input", &f, 0.1f);
					ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
					ImGui::Checkbox("Check", &b);
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Quit", "Esc")) { app.show_style_editor = false; }
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z", &app.popup_message_box)) id = "Yet Not Implemented";
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X", &app.popup_message_box)) id = "Yet Not Implemented";
				if (ImGui::MenuItem("Copy", "CTRL+C", &app.popup_message_box)) id = "Yet Not Implemented";
				if (ImGui::MenuItem("Paste", "CTRL+V", &app.popup_message_box)) id = "Yet Not Implemented";
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
		ImGui::ShowStyleEditor(&style);
		ImGui::End();
	}

	ToggleFlag(app.keyboard, io.ConfigFlags, ImGuiConfigFlags_NavEnableKeyboard);
	ToggleFlag(app.gamepad, io.ConfigFlags, ImGuiConfigFlags_NavEnableGamepad);
	ToggleFlag(app.viewports, io.ConfigFlags, ImGuiConfigFlags_ViewportsEnable);



	static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
	bool opt_fullscreen = app.opt_fullscreen_persistant;

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

	// When using ImGuiDockNodeFlags_PassthruDockspace,
	// DockSpace() will render our background and handle the pass-thru hole,
	// so we ask Begin() to not render a background.
	if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(ID, p_open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);


	// Dockspace
	ImGuiID dockspace_id = ImGui::GetID(ID);
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::MenuItem("GUI Demo", NULL, &app.show_demo_window);
			ImGui::MenuItem("Status", NULL, &app.show_rendererStats);
			ImGui::MenuItem("Fullscreen", NULL, &app.fullscreen);
			if (ImGui::MenuItem("Open", NULL, &app.popup_message_box)) id = "Yet Not Implemented";
			ImGui::Separator();
			if (ImGui::MenuItem("Close", "Alt+F4", false, p_open != NULL))
				app.run_application = false;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Config"))
		{
			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::MenuItem("Style", NULL, &app.show_style_editor);

				ImGui::Separator();

				static const char* renderDs[2] = { "2D Render", "3D Render" };
				static int current = 1;
				ImGui::PushItemWidth(90);
				if (ImGui::Combo("###Renderer", &current, renderDs, sizeof(renderDs) / sizeof(size_t)))
				//bool open = ImGui::BeginCombo("##v", NULL, ImGuiComboFlags_NoPreview | ImGuiComboFlags_PopupAlignLeft);
				//if(open)
				{
					//Kross::Renderer::SetDims(current == 0 ? Kross::Renderer::D2 : Kross::Renderer::D3);
					Kross::Application::Get().renderDims = current == 0 ? Kross::Renderer::D2 : Kross::Renderer::D3;
				}
				//ImGui::EndCombo();
				ImGui::EndMenu();
			}
			ImGui::Separator();
			ShowHelpMarker("Enable or Disable Keyboard navigation through the Graphical Interfaces"); ImGui::SameLine();
			ImGui::MenuItem("Keyboard", NULL, &app.keyboard, app.keyboardState != -1);
			ShowHelpMarker("Enable or Disable Gamepad navigation through the Graphical Interfaces"); ImGui::SameLine();
			ImGui::MenuItem("Gamepad", NULL, &app.gamepad, app.keyboardState != -1);
			ShowHelpMarker("Enable or Disable Viewports to create another window on the operting system"); ImGui::SameLine();
			ImGui::MenuItem("Viewports", NULL, &app.viewports, app.viewportsState != -1);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Camera"))
		{
			ImGui::MenuItem("Window", NULL, &app.show_camera);

			ImGui::Separator();

			static const char* renderDs[2] = { "2D Orthographic", "3D Perspective" };
			static int current = 1;
			ImGui::PushItemWidth(17 * 8);
			if (camera->GetCamera()->GetCameraType() == "Orthographic_2D")
				current = 0;
			if (camera->GetCamera()->GetCameraType() == "Perspective_3D")
				current = 1;
			if (ImGui::Combo("", &current, renderDs, sizeof(renderDs) / sizeof(size_t)))
			{
				if (current == 0) camera = Kross::makeRef<Kross::Camera::Ortho2DCtrl>(renderDs[0], ar);
				if (current == 1) camera = Kross::makeRef<Kross::Camera::Persp3DCtrl>(ar, 120.0f);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Show Help", "F1", &app.popup_message_box)) id = "Yet Not Implemented";
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	MessageBoxDialog(&app.popup_message_box, id);

	ImGui::End();
	return dockspace_id;
}

void Canvas::Status(bool* show_rendererStats, float ts)
{
	static bool pause_frame_plot_animation = false;

	static const char* frame_plot_options[2] = { "Frames per Second", "Elapsed Time" };
	static int item_current = 0;

	float f = item_current ? ts : 1 / ts;
	static size_t timer = 0;
	static float FrameRate = 0;
	static float framerate_buffer = 0;
	static int rate_tick = 10;
	constexpr int plotsize = 128;
	static float* frames = new float[plotsize];
	static bool once = false;
	if (!once) { for (size_t i = 0; i < plotsize; i++)  frames[i] = f; once = true; }

	static float temp;
	if (!pause_frame_plot_animation)
	{
		frames[plotsize - 1] = f;
		for (size_t i = 0; i < plotsize; i++)
		{
			temp = frames[i + 1];
			frames[i] = temp;
		}
	}

	framerate_buffer += 1 / ts;
	if (timer++ % rate_tick == 0) { FrameRate = framerate_buffer / rate_tick; framerate_buffer = 0; }

		if(show_demo_window) ShowDemoWindow(&show_demo_window);
		if(show_rendererStats)
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
			if(e.debugWindow){
				Begin(e.GetName().c_str(), &e.debugWindow);
				Text("Position: X=%.1f, Y=%.1f", e.GetX(), e.GetY());
				Text("Velocity: X=%.3f, Y=%.3f", e.GetVel().x, e.GetVel().y);
				Text("HP %d/%d", e.hp, e.mhp);
				Text("State: %d", e.myState);
				Text("Direction: %d", e.myDirection);
				Text("sprite_speed: "); SameLine(); SliderFloat("", &e.sprite_speed, 0.0f, 1.0f);
				Text("dump: "); SameLine(); SliderFloat("", &e.dump, 0.0f, 1.0f);
				End();
			}
		}
		End();
	}
}

void Canvas::OnEvent(Kross::Event& event)
{
	camera.OnEvent(event);
}
