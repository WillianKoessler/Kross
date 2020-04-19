#include "pch.h"
#include "Canvas.h"
#include <fstream>

static struct windows
{
	bool opt_fullscreen_persistant = true;
	bool fullscreen = false;

	// window

	bool run_applicacion = true;
	bool show_rendererStats = false;
	bool show_camera = false;

	bool show_demo_window = false;
	bool show_style_editor = false;
	bool popup_message_box = false;

	// flags

	bool keyboard = true; int keyboardState = 1;
	bool gamepad = true; int gamepadState = 1;
	bool viewports = false; int viewportsState = false;

}app;


static uint32_t MainScreen(bool* p_open, const char* ID);
static void Status(bool* show_rendererStats, float ts);
void SaveStyle(ImGuiStyle* style, const std::string& path = "imguiStyle.ini");
ImGuiStyle LoadStyle(const std::string& path);
inline bool ToggleFlag(bool enable, int& flags, int mask)
{
	if (enable)
		flags |= mask;
	else
		flags &= ~(flags & mask);
	return enable;
}
// Helper to display a little (?) mark which shows a tooltip when hovered.
static void ShowHelpMarker(const char* desc)
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

	for (auto& e : entities)
	{
		//if (Kross::Input::IsMouseButtonPressed(KROSS_MOUSE_BUTTON_1)) params.position = camera.GetCamera()->GetPosition();
		//Kross::Renderer2D::BatchQuad(params);
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

void Canvas::OnImGuiRender(Kross::Timestep ts)
{
	using namespace ImGui;
	auto& io = GetIO();

	auto main_screen = MainScreen(&app.run_applicacion, "Main Window Dockspace");
	if (app.show_rendererStats) Status(&app.show_rendererStats, ts);
	auto& App = Kross::Application::Get();
	if (!app.run_applicacion) App.OnEvent(Kross::WindowCloseEvent());
	if (App.GetWindow().FullScreen(app.fullscreen))
	{
		ToggleFlag(false, io.ConfigFlags, ImGuiConfigFlags_ViewportsEnable);
		app.viewportsState = -1;
		app.viewports = false;
	}
	else app.viewportsState = 0;

	if (app.show_camera) camera.DebugWindow();

	if (!entities.empty())
	{
		if (Begin("Entities", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
		{
			if (ImGui::TreeNode("Entities"))
			{
				ImGui::SameLine(); ShowHelpMarker("Click on Entity Name to show more info.");
				ImGui::PushStyleVar(ImGuiStyleVar_IndentSpacing, ImGui::GetFontSize() * 3); // Increase spacing to differentiate leaves from expanded contents.
				for (auto& e : entities)
				{
					ImGui::TreeNodeEx(e.GetName().c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
					if (ImGui::IsItemClicked())
						e.debugWindow = !e.debugWindow;
				}
				ImGui::PopStyleVar();
				ImGui::TreePop();
			}
			for (auto& e : entities) e.ShowDebugWindow();
			ImGui::End();
		}
		else ImGui::End();
	}
	static ImGuiWindowFlags viewport_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoNavInputs | ImGuiWindowFlags_MenuBar;
	ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowDockID(main_screen, ImGuiCond_FirstUseEver);
	if (!Begin("Viewport", nullptr, viewport_flags))
		ImGui::End();
	else
	{
		Image((void*)(intptr_t)Kross::Renderer2D::GetFrameBuffer()->GetTexture()->GetID(),
			GetContentRegionAvail(),
			ImVec2(0, 1),
			ImVec2(1, 0));
		ImVec2 size = GetWindowSize();
		camera.OnEvent(Kross::WindowResizeEvent((uint32_t)size.x, (uint32_t)size.y));
		if (ImGui::IsWindowFocused()) {
			camera.Input(ImGui::IsKeyDown);
			for (auto& e : entities) e.Input();
		}
		if (ImGui::IsWindowHovered())
			camera.OnEvent(Kross::MouseScrolledEvent(io.MouseWheelH, io.MouseWheel));
		ImGui::End();
	}
}

void Canvas::OnEvent(Kross::Event& event)
{
	//camera.OnEvent(event);
}

inline void CancelButton(bool* show, ImVec2 size = { 0, 0 })
{
	if (size.x > 0 && size.y > 0)
	{
		if (ImGui::Button("Cancel", size))
		{
			ImGui::CloseCurrentPopup();
			*show = false;
		}
	}
	else
	{
		if (ImGui::Button("Cancel"))
		{
			ImGui::CloseCurrentPopup();
			*show = false;
		}
	}
}
void MessageBoxDialog(bool* show, const char* id, void* data = nullptr)
{
	if (*show) ImGui::OpenPopup(id);
	if (ImGui::BeginPopupModal(id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (id == "Style Open File")
		{
			char buff[256] = { 0 };
			ImGui::SetWindowFocus();
			if (!ImGui::IsAnyItemActive())
				ImGui::SetKeyboardFocusHere(0);
			if (ImGui::InputText("file", buff, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {///*Disabled till i know how to use InputText() to write into a buffer*/ || ImGui::Button("OK", ImVec2(50, 0))) {
				ImGui::GetStyle() = LoadStyle(buff[0] ? buff : "imguiStyle.ini");
				ImGui::CloseCurrentPopup();
				*show = false;
			}
			ImGui::SetItemDefaultFocus();
			ImGui::SameLine();
			if (ImGui::Button("Cancel", ImVec2(50, 0))) {
				ImGui::CloseCurrentPopup();
				*show = false;
			}
		}
		else if (id == "Yet Not Implemented")
		{
			ImGui::Text("This feature has yet not been implemented.");
			CancelButton(show);
		}
		else
		{
			ImGui::Text("An Error has ocurred!\nThe ID for this Modal Window was not setted.");
			CancelButton(show);
		}
		ImGui::EndPopup();
	}
}


uint32_t MainScreen(bool* p_open, const char* ID)
{
	static const char* id = "";
	ImGuiIO& io = ImGui::GetIO();

	if (app.show_demo_window) ImGui::ShowDemoWindow(&app.show_demo_window);
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
				*p_open = false;
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Config"))
		{
			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::MenuItem("ImGui", NULL, &app.show_style_editor);
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

void Status(bool* show_rendererStats, float ts)
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

	if (show_rendererStats)
	{
		char buf[128];
		sprintf_s(buf, "Application Status | FPS: %.1f###StatusTitle", FrameRate);
		if (ImGui::Begin(buf, show_rendererStats, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::Combo("mode", &item_current, frame_plot_options, 2)) once = false;
			ImGui::SameLine(); ShowHelpMarker(
				R"(Select what plot type you want.
Frames per Second mode, or FPS, is the number of how many frames the application can push out in a single second.
Elapsed Time mode, is the plot of how many milisseconds has passed since the last frame was drawn.
)");
			ImGui::Checkbox("Pause Animation", &pause_frame_plot_animation);
			float max = 0;
			for (size_t i = 0; i < plotsize; i++)
				max = std::max(frames[i], max);
			ImGui::PlotLines("", frames, plotsize - 1, 0, "Frames", 0, std::min(120.0f, max));
			auto& mouse = Kross::Input::GetMousePos();
			ImGui::Text("Mouse: %.1f, %.1f", mouse.x, mouse.y);

			ImGui::Text("MaxQuadCount: %d", Kross::Renderer2D::getStats().maxQuads);
			ImGui::Text("Quad Count: %d", Kross::Renderer2D::getStats().QuadCount);
			ImGui::Text("Draw Calls: %d", Kross::Renderer2D::getStats().DrawCount);
			ImGui::End();
		}
		else
			ImGui::End();
	}
	if (!(*show_rendererStats)) once = false;
}

void SaveStyle(ImGuiStyle* style, const std::string& path)
{
	auto aFloat = [](const char* label, float value)
	{
		return std::string(label) + "					= " + std::to_string(value) + '\n';
	};
	auto aVec2 = [](const char* label, ImVec2 value)
	{
		return std::string(label) + "					= { " + std::to_string(value.x) + ", " + std::to_string(value.y) + " }\n";
	};
	auto aVec4 = [](const char* label, ImVec4 value)
	{
		return std::string(label) + "	= { " + std::to_string(value.x) + ", " + std::to_string(value.y) + ", " + std::to_string(value.z) + ", " + std::to_string(value.w) + " }\n";
	};
	std::string buffer;

	buffer += aFloat("Alpha				", style->Alpha);
	buffer += aVec2("WindowPadding		", style->WindowPadding);
	buffer += aFloat("WindowRounding		", style->WindowRounding);
	buffer += aFloat("WindowBorderSize	", style->WindowBorderSize);
	buffer += aVec2("WindowMinSize		", style->WindowMinSize);
	buffer += aVec2("WindowTitleAlign	", style->WindowTitleAlign);
	buffer += aFloat("ChildRounding		", style->ChildRounding);
	buffer += aFloat("ChildBorderSize		", style->ChildBorderSize);
	buffer += aFloat("PopupRounding		", style->PopupRounding);
	buffer += aFloat("PopupBorderSize		", style->PopupBorderSize);
	buffer += aVec2("FramePadding		", style->FramePadding);
	buffer += aFloat("FrameRounding		", style->FrameRounding);
	buffer += aFloat("FrameBorderSize		", style->FrameBorderSize);
	buffer += aVec2("ItemSpacing			", style->ItemSpacing);
	buffer += aVec2("ItemInnerSpacing	", style->ItemInnerSpacing);
	buffer += aVec2("TouchExtraPadding	", style->TouchExtraPadding);
	buffer += aFloat("IndentSpacing		", style->IndentSpacing);
	buffer += aFloat("ColumnsMinSpacing	", style->ColumnsMinSpacing);
	buffer += aFloat("ScrollbarSize		", style->ScrollbarSize);
	buffer += aFloat("ScrollbarRounding	", style->ScrollbarRounding);
	buffer += aFloat("GrabMinSize			", style->GrabMinSize);
	buffer += aFloat("GrabRounding		", style->GrabRounding);
	buffer += aFloat("TabRounding			", style->TabRounding);
	buffer += aFloat("TabBorderSize		", style->TabBorderSize);
	buffer += aVec2("ButtonTextAlign		", style->ButtonTextAlign);
	buffer += aVec2("DisplayWindowPadding", style->DisplayWindowPadding);
	buffer += aVec2("DisplaySafeAreaPadding", style->DisplaySafeAreaPadding);
	buffer += aFloat("MouseCursorScale	", style->MouseCursorScale);
	buffer += aFloat("AntiAliasedLines	", style->AntiAliasedLines);
	buffer += aFloat("AntiAliasedFill		", style->AntiAliasedFill);
	buffer += aFloat("CurveTessellationTol", style->CurveTessellationTol);
	buffer += aVec4("Colors[ImGuiCol_Text]				", style->Colors[ImGuiCol_Text]);
	buffer += aVec4("Colors[ImGuiCol_TextDisabled]		", style->Colors[ImGuiCol_TextDisabled]);
	buffer += aVec4("Colors[ImGuiCol_WindowBg]			", style->Colors[ImGuiCol_WindowBg]);
	buffer += aVec4("Colors[ImGuiCol_ChildBg]			", style->Colors[ImGuiCol_ChildBg]);
	buffer += aVec4("Colors[ImGuiCol_PopupBg]			", style->Colors[ImGuiCol_PopupBg]);
	buffer += aVec4("Colors[ImGuiCol_Border]				", style->Colors[ImGuiCol_Border]);
	buffer += aVec4("Colors[ImGuiCol_BorderShadow]		", style->Colors[ImGuiCol_BorderShadow]);
	buffer += aVec4("Colors[ImGuiCol_FrameBg]			", style->Colors[ImGuiCol_FrameBg]);
	buffer += aVec4("Colors[ImGuiCol_FrameBgHovered]		", style->Colors[ImGuiCol_FrameBgHovered]);
	buffer += aVec4("Colors[ImGuiCol_FrameBgActive]		", style->Colors[ImGuiCol_FrameBgActive]);
	buffer += aVec4("Colors[ImGuiCol_TitleBg]			", style->Colors[ImGuiCol_TitleBg]);
	buffer += aVec4("Colors[ImGuiCol_TitleBgActive]		", style->Colors[ImGuiCol_TitleBgActive]);
	buffer += aVec4("Colors[ImGuiCol_TitleBgCollapsed]	", style->Colors[ImGuiCol_TitleBgCollapsed]);
	buffer += aVec4("Colors[ImGuiCol_MenuBarBg]			", style->Colors[ImGuiCol_MenuBarBg]);
	buffer += aVec4("Colors[ImGuiCol_ScrollbarBg]		", style->Colors[ImGuiCol_ScrollbarBg]);
	buffer += aVec4("Colors[ImGuiCol_ScrollbarGrab]		", style->Colors[ImGuiCol_ScrollbarGrab]);
	buffer += aVec4("Colors[ImGuiCol_ScrollbarGrabHovered]", style->Colors[ImGuiCol_ScrollbarGrabHovered]);
	buffer += aVec4("Colors[ImGuiCol_ScrollbarGrabActive]", style->Colors[ImGuiCol_ScrollbarGrabActive]);
	buffer += aVec4("Colors[ImGuiCol_CheckMark]			", style->Colors[ImGuiCol_CheckMark]);
	buffer += aVec4("Colors[ImGuiCol_SliderGrab]			", style->Colors[ImGuiCol_SliderGrab]);
	buffer += aVec4("Colors[ImGuiCol_SliderGrabActive]	", style->Colors[ImGuiCol_SliderGrabActive]);
	buffer += aVec4("Colors[ImGuiCol_Button]				", style->Colors[ImGuiCol_Button]);
	buffer += aVec4("Colors[ImGuiCol_ButtonHovered]		", style->Colors[ImGuiCol_ButtonHovered]);
	buffer += aVec4("Colors[ImGuiCol_ButtonActive]		", style->Colors[ImGuiCol_ButtonActive]);
	buffer += aVec4("Colors[ImGuiCol_Header]				", style->Colors[ImGuiCol_Header]);
	buffer += aVec4("Colors[ImGuiCol_HeaderHovered]		", style->Colors[ImGuiCol_HeaderHovered]);
	buffer += aVec4("Colors[ImGuiCol_HeaderActive]		", style->Colors[ImGuiCol_HeaderActive]);
	buffer += aVec4("Colors[ImGuiCol_Separator]			", style->Colors[ImGuiCol_Separator]);
	buffer += aVec4("Colors[ImGuiCol_SeparatorHovered]	", style->Colors[ImGuiCol_SeparatorHovered]);
	buffer += aVec4("Colors[ImGuiCol_SeparatorActive]	", style->Colors[ImGuiCol_SeparatorActive]);
	buffer += aVec4("Colors[ImGuiCol_ResizeGrip]			", style->Colors[ImGuiCol_ResizeGrip]);
	buffer += aVec4("Colors[ImGuiCol_ResizeGripHovered]	", style->Colors[ImGuiCol_ResizeGripHovered]);
	buffer += aVec4("Colors[ImGuiCol_ResizeGripActive]	", style->Colors[ImGuiCol_ResizeGripActive]);
	buffer += aVec4("Colors[ImGuiCol_Tab]				", style->Colors[ImGuiCol_Tab]);
	buffer += aVec4("Colors[ImGuiCol_TabHovered]			", style->Colors[ImGuiCol_TabHovered]);
	buffer += aVec4("Colors[ImGuiCol_TabActive]			", style->Colors[ImGuiCol_TabActive]);
	buffer += aVec4("Colors[ImGuiCol_TabUnfocused]		", style->Colors[ImGuiCol_TabUnfocused]);
	buffer += aVec4("Colors[ImGuiCol_TabUnfocusedActive]	", style->Colors[ImGuiCol_TabUnfocusedActive]);
	buffer += aVec4("Colors[ImGuiCol_DockingPreview]		", style->Colors[ImGuiCol_DockingPreview]);
	buffer += aVec4("Colors[ImGuiCol_DockingEmptyBg]		", style->Colors[ImGuiCol_DockingEmptyBg]);
	buffer += aVec4("Colors[ImGuiCol_PlotLines]			", style->Colors[ImGuiCol_PlotLines]);
	buffer += aVec4("Colors[ImGuiCol_PlotLinesHovered]	", style->Colors[ImGuiCol_PlotLinesHovered]);
	buffer += aVec4("Colors[ImGuiCol_PlotHistogram]		", style->Colors[ImGuiCol_PlotHistogram]);
	buffer += aVec4("Colors[ImGuiCol_PlotHistogramHovered]", style->Colors[ImGuiCol_PlotHistogramHovered]);
	buffer += aVec4("Colors[ImGuiCol_TextSelectedBg]		", style->Colors[ImGuiCol_TextSelectedBg]);
	buffer += aVec4("Colors[ImGuiCol_DragDropTarget]		", style->Colors[ImGuiCol_DragDropTarget]);
	buffer += aVec4("Colors[ImGuiCol_NavHighlight]		", style->Colors[ImGuiCol_NavHighlight]);
	buffer += aVec4("Colors[ImGuiCol_NavWindowingHighlight]", style->Colors[ImGuiCol_NavWindowingHighlight]);
	buffer += aVec4("Colors[ImGuiCol_NavWindowingDimBg]	", style->Colors[ImGuiCol_NavWindowingDimBg]);
	buffer += aVec4("Colors[ImGuiCol_ModalWindowDimBg]	", style->Colors[ImGuiCol_ModalWindowDimBg]);
	std::ofstream file;
	file.open((path).c_str(), std::ios::out | std::ios::trunc);
	KROSS_CORE_TRACE("Writing to File: {0}\n{1}", path, buffer);
	file.write(buffer.c_str(), buffer.size());
	file.close();
}

ImGuiStyle LoadStyle(const std::string& path)
{
	KROSS_PROFILE_FUNC();
	KROSS_CORE_TRACE("Requested LoadStyle from file: {0}", path);
	constexpr int size = 41 + ImGuiCol_COUNT;
	float fFile[size] = { 0.0f };
	std::string result;
	std::ifstream in(path, std::ios::in);
	if (in)
	{
		ImGuiStyle style;
		std::string buffer;
		std::string name;
		std::string value;
		size_t line = 0, eq;
		while (std::getline(in, buffer))
		{
			line++;
			std::istringstream a(buffer);
			a >> name;

			eq = buffer.find_first_of("=") + 1;
			if (eq == std::string::npos)
			{
				KROSS_CORE_ERROR("{0} Invalid Syntax. Line: {1}. Expected '='", __FUNCTION__, line);
				KROSS_MSGBOX("Invalid Syntax.", __FUNCTION__, _ERROR_);
				return ImGuiStyle();
			}
			size_t fbrq = buffer.find('{', eq), sbrq = buffer.find('}', eq);
			if (fbrq > sbrq)
			{
				KROSS_CORE_ERROR("{0} Invalid Syntax. Line: {1}. Expected '}'", __FUNCTION__, line);
				KROSS_MSGBOX("Invalid Syntax.", __FUNCTION__, _ERROR_);
				return ImGuiStyle();
			}
			if (fbrq != std::string::npos) // All Vectors
			{
				if (sbrq == std::string::npos)
				{
					KROSS_CORE_ERROR("{0} Invalid Syntax. Line: {1}. Expected '}'", __FUNCTION__, line);
					KROSS_MSGBOX("Invalid Syntax.", __FUNCTION__, _ERROR_);
					return ImGuiStyle();
				}

				try
				{
					std::vector<float> arr;
					value = buffer.substr(fbrq+1, sbrq-fbrq-1);
					size_t last_comma = 0, comma = value.find(',');
					for(int i = 0; i < value.size(); i++)
					{
						if (value[i] == ',' || i == value.size()-1)
						{
							auto sub = value.substr(last_comma, i);
							arr.push_back(std::stof(sub));
							last_comma = i + 1;
						}
					}
					if (arr.size() == 2)
					{
						// vec2
						if (!name.compare("WindowPadding"))	style.WindowPadding = ImVec2(arr[0], arr[1]);
						else if (!name.compare("WindowMinSize"))	style.WindowMinSize = ImVec2(arr[0], arr[1]);
						else if (!name.compare("WindowTitleAlign"))	style.WindowTitleAlign = ImVec2(arr[0], arr[1]);
						else if (!name.compare("FramePadding"))	style.FramePadding = ImVec2(arr[0], arr[1]);
						else if (!name.compare("ItemSpacing"))	style.ItemSpacing = ImVec2(arr[0], arr[1]);
						else if (!name.compare("ItemInnerSpacing"))	style.ItemInnerSpacing = ImVec2(arr[0], arr[1]);
						else if (!name.compare("TouchExtraPadding"))	style.TouchExtraPadding = ImVec2(arr[0], arr[1]);
						else if (!name.compare("ButtonTextAlign"))	style.ButtonTextAlign = ImVec2(arr[0], arr[1]);
						else if (!name.compare("DisplayWindowPadding"))	style.DisplayWindowPadding = ImVec2(arr[0], arr[1]);
						else if (!name.compare("DisplaySafeAreaPadding"))	style.DisplaySafeAreaPadding = ImVec2(arr[0], arr[1]);
					}
					else if (arr.size() == 4)
					{
						// vec4
						if (!name.compare("Colors[ImGuiCol_Text]")) style.Colors[ImGuiCol_Text] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_TextDisabled]")) style.Colors[ImGuiCol_TextDisabled] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_WindowBg]")) style.Colors[ImGuiCol_WindowBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ChildBg]")) style.Colors[ImGuiCol_ChildBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_PopupBg]")) style.Colors[ImGuiCol_PopupBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_Border]")) style.Colors[ImGuiCol_Border] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_BorderShadow]")) style.Colors[ImGuiCol_BorderShadow] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_FrameBg]")) style.Colors[ImGuiCol_FrameBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_FrameBgHovered]")) style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_FrameBgActive]")) style.Colors[ImGuiCol_FrameBgActive] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_TitleBg]")) style.Colors[ImGuiCol_TitleBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_TitleBgActive]")) style.Colors[ImGuiCol_TitleBgActive] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_TitleBgCollapsed]")) style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_MenuBarBg]")) style.Colors[ImGuiCol_MenuBarBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ScrollbarBg]")) style.Colors[ImGuiCol_ScrollbarBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ScrollbarGrab]")) style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ScrollbarGrabHovered]")) style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ScrollbarGrabActive]")) style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_CheckMark]")) style.Colors[ImGuiCol_CheckMark] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_SliderGrab]")) style.Colors[ImGuiCol_SliderGrab] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_SliderGrabActive]")) style.Colors[ImGuiCol_SliderGrabActive] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_Button]")) style.Colors[ImGuiCol_Button] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ButtonHovered]")) style.Colors[ImGuiCol_ButtonHovered] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ButtonActive]")) style.Colors[ImGuiCol_ButtonActive] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_Header]")) style.Colors[ImGuiCol_Header] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_HeaderHovered]")) style.Colors[ImGuiCol_HeaderHovered] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_HeaderActive]")) style.Colors[ImGuiCol_HeaderActive] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_Separator]")) style.Colors[ImGuiCol_Separator] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_SeparatorHovered]")) style.Colors[ImGuiCol_SeparatorHovered] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_SeparatorActive]")) style.Colors[ImGuiCol_SeparatorActive] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ResizeGrip]")) style.Colors[ImGuiCol_ResizeGrip] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ResizeGripHovered]")) style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ResizeGripActive]")) style.Colors[ImGuiCol_ResizeGripActive] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_Tab]")) style.Colors[ImGuiCol_Tab] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_TabHovered]")) style.Colors[ImGuiCol_TabHovered] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_TabActive]")) style.Colors[ImGuiCol_TabActive] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_TabUnfocused]")) style.Colors[ImGuiCol_TabUnfocused] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_TabUnfocusedActive]")) style.Colors[ImGuiCol_TabUnfocusedActive] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_DockingPreview]")) style.Colors[ImGuiCol_DockingPreview] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_DockingEmptyBg]")) style.Colors[ImGuiCol_DockingEmptyBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_PlotLines]")) style.Colors[ImGuiCol_PlotLines] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_PlotLinesHovered]")) style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_PlotHistogram]")) style.Colors[ImGuiCol_PlotHistogram] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_PlotHistogramHovered]")) style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_TextSelectedBg]")) style.Colors[ImGuiCol_TextSelectedBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_DragDropTarget]")) style.Colors[ImGuiCol_DragDropTarget] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_NavHighlight]")) style.Colors[ImGuiCol_NavHighlight] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_NavWindowingHighlight]")) style.Colors[ImGuiCol_NavWindowingHighlight] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_NavWindowingDimBg]")) style.Colors[ImGuiCol_NavWindowingDimBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
						else if (!name.compare("Colors[ImGuiCol_ModalWindowDimBg]")) style.Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(arr[0], arr[1], arr[2], arr[3]);
					}
					else
					{
						// invalid
						KROSS_CORE_ERROR("{0} Invalid Syntax. Line: {1}. Only vec2 and vec4 are supported", __FUNCTION__, line);
						KROSS_MSGBOX("Invalid Syntax.", __FUNCTION__, _ERROR_);
						return ImGuiStyle();
					}
				}
				catch (std::exception& e)
				{
					if (!std::string("invalid stof argument").compare(e.what()))
						KROSS_MSGBOX("Line: " + std::to_string(line) + "\nError: Invalid Value for\nVarible Named: " + name + "\nFile: " + path, __FUNCTION__, _ERROR_);
					else
						KROSS_MSGBOX(e.what(), "Exception Catch", _ERROR_);
					return ImGuiStyle();
				}
			}
			else // All single values
			{
				if (buffer.find('}', eq) != std::string::npos)
				{
					KROSS_CORE_ERROR("{0} Invalid Syntax. Line: {1}. Expected '{' for Vectors, or invalid '}' for Floats", __FUNCTION__, line);
					KROSS_MSGBOX("Invalid Syntax.", __FUNCTION__, _ERROR_);
					return ImGuiStyle();
				}
				try {
					value = buffer.substr(eq);
					if (!name.compare("Alpha")) style.Alpha = std::stof(value);
					else if (!name.compare("WindowRounding")) style.WindowRounding = std::stof(value);
					else if (!name.compare("WindowBorderSize")) style.WindowBorderSize = std::stof(value);
					else if (!name.compare("ChildRounding")) style.ChildRounding = std::stof(value);
					else if (!name.compare("ChildBorderSize")) style.ChildBorderSize = std::stof(value);
					else if (!name.compare("PopupRounding")) style.PopupRounding = std::stof(value);
					else if (!name.compare("PopupBorderSize")) style.PopupBorderSize = std::stof(value);
					else if (!name.compare("FrameRounding")) style.FrameRounding = std::stof(value);
					else if (!name.compare("FrameBorderSize")) style.FrameBorderSize = std::stof(value);
					else if (!name.compare("IndentSpacing")) style.IndentSpacing = std::stof(value);
					else if (!name.compare("ColumnsMinSpacing")) style.ColumnsMinSpacing = std::stof(value);
					else if (!name.compare("ScrollbarSize")) style.ScrollbarSize = std::stof(value);
					else if (!name.compare("ScrollbarRounding")) style.ScrollbarRounding = std::stof(value);
					else if (!name.compare("GrabMinSize")) style.GrabMinSize = std::stof(value);
					else if (!name.compare("GrabRounding")) style.GrabRounding = std::stof(value);
					else if (!name.compare("TabRounding")) style.TabRounding = std::stof(value);
					else if (!name.compare("TabBorderSize")) style.TabBorderSize = std::stof(value);
					else if (!name.compare("MouseCursorScale")) style.MouseCursorScale = std::stof(value);
					else if (!name.compare("AntiAliasedLines")) style.AntiAliasedLines = std::stof(value);
					else if (!name.compare("AntiAliasedFill")) style.AntiAliasedFill = std::stof(value);
					else if (!name.compare("CurveTessellationTol")) style.CurveTessellationTol = std::stof(value);
				}
				catch (std::exception& e)
				{
					if (!std::string("invalid stof argument").compare(e.what()))
						KROSS_MSGBOX("Line: " + std::to_string(line) + "\nError: Invalid Value for\nVarible Named: " + name + "\nFile: " + path, __FUNCTION__, _ERROR_);
					else
						KROSS_MSGBOX(e.what(), "Exception Catch", _ERROR_);
					return ImGuiStyle();
				}
			}
		}
		in.close();
		return style;
	}
	else
	{
		in.close();
		KROSS_MSGBOX("Could not open file " + path, __FUNCTION__, _WARN_);
		return ImGuiStyle();
	}

}


/*
Alpha									= 1.000000
WindowPadding							= [ 8.000000, 8.000000 ]
WindowRounding							= 0.000000
WindowBorderSize						= 1.000000
WindowMinSize							= [ 32.000000, 32.000000 ]
WindowTitleAlign						= [ 0.000000, 0.500000 ]
ChildRounding							= 0.000000
ChildBorderSize							= 1.000000
PopupRounding							= 0.000000
PopupBorderSize							= 1.000000
FramePadding							= [ 4.000000, 3.000000 ]
FrameRounding							= 0.000000
FrameBorderSize							= 0.000000
ItemSpacing								= [ 8.000000, 4.000000 ]
ItemInnerSpacing						= [ 4.000000, 4.000000 ]
TouchExtraPadding						= [ 0.000000, 0.000000 ]
IndentSpacing							= 21.000000
ColumnsMinSpacing						= 6.000000
ScrollbarSize							= 16.000000
ScrollbarRounding						= 9.000000
GrabMinSize								= 10.000000
GrabRounding							= 0.000000
TabRounding								= 4.000000
TabBorderSize							= 0.000000
ButtonTextAlign							= [ 0.500000, 0.500000 ]
DisplayWindowPadding					= [ 19.000000, 19.000000 ]
DisplaySafeAreaPadding					= [ 3.000000, 3.000000 ]
MouseCursorScale						= 1.000000
AntiAliasedLines						= 1.000000
AntiAliasedFill							= 1.000000
CurveTessellationTol					= 1.250000
Colors[ImGuiCol_Text]					= [ 1.000000, 1.000000, 1.000000, 1.000000 ]
Colors[ImGuiCol_TextDisabled]			= [ 0.500000, 0.500000, 0.500000, 1.000000 ]
Colors[ImGuiCol_WindowBg]				= [ 0.060000, 0.060000, 0.060000, 1.000000 ]
Colors[ImGuiCol_ChildBg]				= [ 0.000000, 0.000000, 0.000000, 0.000000 ]
Colors[ImGuiCol_PopupBg]				= [ 0.080000, 0.080000, 0.080000, 0.940000 ]
Colors[ImGuiCol_Border]					= [ 0.430000, 0.430000, 0.500000, 0.500000 ]
Colors[ImGuiCol_BorderShadow]			= [ 0.000000, 0.000000, 0.000000, 0.000000 ]
Colors[ImGuiCol_FrameBg]				= [ 0.160000, 0.290000, 0.480000, 0.540000 ]
Colors[ImGuiCol_FrameBgHovered]			= [ 0.260000, 0.590000, 0.980000, 0.400000 ]
Colors[ImGuiCol_FrameBgActive]			= [ 0.260000, 0.590000, 0.980000, 0.670000 ]
Colors[ImGuiCol_TitleBg]				= [ 0.040000, 0.040000, 0.040000, 1.000000 ]
Colors[ImGuiCol_TitleBgActive]			= [ 0.160000, 0.290000, 0.480000, 1.000000 ]
Colors[ImGuiCol_TitleBgCollapsed]		= [ 0.000000, 0.000000, 0.000000, 0.510000 ]
Colors[ImGuiCol_MenuBarBg]				= [ 0.140000, 0.140000, 0.140000, 1.000000 ]
Colors[ImGuiCol_ScrollbarBg]			= [ 0.020000, 0.020000, 0.020000, 0.530000 ]
Colors[ImGuiCol_ScrollbarGrab]			= [ 0.310000, 0.310000, 0.310000, 1.000000 ]
Colors[ImGuiCol_ScrollbarGrabHovered]	= [ 0.410000, 0.410000, 0.410000, 1.000000 ]
Colors[ImGuiCol_ScrollbarGrabActive]	= [ 0.510000, 0.510000, 0.510000, 1.000000 ]
Colors[ImGuiCol_CheckMark]				= [ 0.260000, 0.590000, 0.980000, 1.000000 ]
Colors[ImGuiCol_SliderGrab]				= [ 0.240000, 0.520000, 0.880000, 1.000000 ]
Colors[ImGuiCol_SliderGrabActive]		= [ 0.260000, 0.590000, 0.980000, 1.000000 ]
Colors[ImGuiCol_Button]					= [ 0.260000, 0.590000, 0.980000, 0.400000 ]
Colors[ImGuiCol_ButtonHovered]			= [ 0.260000, 0.590000, 0.980000, 1.000000 ]
Colors[ImGuiCol_ButtonActive]			= [ 0.060000, 0.530000, 0.980000, 1.000000 ]
Colors[ImGuiCol_Header]					= [ 0.260000, 0.590000, 0.980000, 0.310000 ]
Colors[ImGuiCol_HeaderHovered]			= [ 0.260000, 0.590000, 0.980000, 0.800000 ]
Colors[ImGuiCol_HeaderActive]			= [ 0.260000, 0.590000, 0.980000, 1.000000 ]
Colors[ImGuiCol_Separator]				= [ 0.430000, 0.430000, 0.500000, 0.500000 ]
Colors[ImGuiCol_SeparatorHovered]		= [ 0.100000, 0.400000, 0.750000, 0.780000 ]
Colors[ImGuiCol_SeparatorActive]		= [ 0.100000, 0.400000, 0.750000, 1.000000 ]
Colors[ImGuiCol_ResizeGrip]				= [ 0.260000, 0.590000, 0.980000, 0.250000 ]
Colors[ImGuiCol_ResizeGripHovered]		= [ 0.260000, 0.590000, 0.980000, 0.670000 ]
Colors[ImGuiCol_ResizeGripActive]		= [ 0.260000, 0.590000, 0.980000, 0.950000 ]
Colors[ImGuiCol_Tab]					= [ 0.180000, 0.350000, 0.580000, 0.862000 ]
Colors[ImGuiCol_TabHovered]				= [ 0.260000, 0.590000, 0.980000, 0.800000 ]
Colors[ImGuiCol_TabActive]				= [ 0.200000, 0.410000, 0.680000, 1.000000 ]
Colors[ImGuiCol_TabUnfocused]			= [ 0.068000, 0.102000, 0.148000, 0.972400 ]
Colors[ImGuiCol_TabUnfocusedActive]		= [ 0.136000, 0.262000, 0.424000, 1.000000 ]
Colors[ImGuiCol_DockingPreview]			= [ 0.260000, 0.590000, 0.980000, 0.700000 ]
Colors[ImGuiCol_DockingEmptyBg]			= [ 0.200000, 0.200000, 0.200000, 1.000000 ]
Colors[ImGuiCol_PlotLines]				= [ 0.610000, 0.610000, 0.610000, 1.000000 ]
Colors[ImGuiCol_PlotLinesHovered]		= [ 1.000000, 0.430000, 0.350000, 1.000000 ]
Colors[ImGuiCol_PlotHistogram]			= [ 0.900000, 0.700000, 0.000000, 1.000000 ]
Colors[ImGuiCol_PlotHistogramHovered]	= [ 1.000000, 0.600000, 0.000000, 1.000000 ]
Colors[ImGuiCol_TextSelectedBg]			= [ 0.260000, 0.590000, 0.980000, 0.350000 ]
Colors[ImGuiCol_DragDropTarget]			= [ 1.000000, 1.000000, 0.000000, 0.900000 ]
Colors[ImGuiCol_NavHighlight]			= [ 0.260000, 0.590000, 0.980000, 1.000000 ]
Colors[ImGuiCol_NavWindowingHighlight]	= [ 1.000000, 1.000000, 1.000000, 0.700000 ]
Colors[ImGuiCol_NavWindowingDimBg]		= [ 0.800000, 0.800000, 0.800000, 0.200000 ]
Colors[ImGuiCol_ModalWindowDimBg]		= [ 0.800000, 0.800000, 0.800000, 0.350000 ]

*/
