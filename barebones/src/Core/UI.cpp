#include "pch.h"
#include "UI.h"

#include <glm/gtc/type_ptr.hpp>
static struct window_manager {
	bool main = true;
	bool status = false;
	bool viewport = true;
} windows;
static struct windows_
{
	bool opt_fullscreen_persistant = true;
	bool fullscreen = false;

	// window

	bool run_applicacion = true;
	bool show_rendererStats = false;
	bool show_camera = true;

	bool show_demo_window = false;
	bool show_style_editor = false;
	bool popup_message_box = false;

	bool viewport = true;

	// flags

	bool keyboardEnabled = true; bool keyboardState = true;
	bool gamepadEnabled = true; bool gamepadState = true;
	bool viewportsEnabled = false; bool viewportsState = false;

} manager;

void ShowHelpMarker(const char* desc)
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

void MessageBoxDialog(bool* show, const char* id, void* data = nullptr)
{
	if (*show) ImGui::OpenPopup(id);
	if (ImGui::BeginPopupModal(id, NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (id == "Style Open File")
		{
			//char buff[256] = { 0 };
			//ImGui::SetWindowFocus();
			//////if (!ImGui::IsAnyItemActive())
			//////	ImGui::SetKeyboardFocusHere(0);
			//////if (ImGui::InputText("file", buff, 256, ImGuiInputTextFlags_EnterReturnsTrue)) {///*Disabled till i know how to use InputText() to write into a buffer*/ || ImGui::Button("OK", ImVec2(50, 0))) {
			////	//ImGui::GetStyle() = LoadStyle(buff[0] ? buff : "imguiStyle.ini");
			//ImGui::GetStyle() = LoadStyle("imguiStyle.ini");
			//ImGui::CloseCurrentPopup();
			//*show = false;
			////}
			////ImGui::SetItemDefaultFocus();
			////ImGui::SameLine();
			////if (ImGui::Button("Cancel", ImVec2(50, 0))) {
			////	ImGui::CloseCurrentPopup();
			////	*show = false;
			////}
		}
		else if (id == "Error: 20")
		{
			ImGui::Text("This feature has not been implemented yet.");
			auto w = ImGui::GetWindowSize().x;
			ImGui::PushItemWidth(-w / 2);
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
				*show = false;
			}
			ImGui::PopItemWidth();
		}
		else
		{
			ImGui::Text("An Error has ocurred!\nThe ID for this PopUp Window was not setted.");
			if (ImGui::Button("Cancel"))
			{
				ImGui::CloseCurrentPopup();
				*show = false;
			}
		}
		ImGui::EndPopup();
	}
}

UI::UI()
	: Layer("UI")
{
	unsigned char* col = new unsigned char[4];
	col[0] = 255;
	col[1] = 0;
	col[2] = 0;
	col[3] = 255;

	canvas = Kross::Texture::T2D::CreateRef(50, 50, "blank", col);
}

void UI::OnAttach()
{
	Kross::Renderer::Command::SetClear(0x0f0f0fff);
}
void UI::OnDetach()
{
}


void UI::OnUpdate(Kross::Timestep ts)
{
	Kross::Renderer2D::ResetStats();
	Kross::Renderer::Command::Clear();
}

void UI::OnImGuiRender(Kross::Timestep ts)
{
	using namespace ImGui;
	{
		auto& io = ImGui::GetIO();
		auto& app = Kross::Application::Get();
		int32_t main = MainWindow(&windows.main, app.GetWindow().GetName().c_str());
		Status(&windows.status, ts);
		viewport(&windows.viewport);
		//auto main = MainWindow(&manager.run_applicacion, app.GetWindow().GetName().c_str());
		//Status(&manager.show_rendererStats, ts);
		//viewport(&manager.viewport);
		if (!windows.main) app.OnEvent(Kross::WindowCloseEvent());
		if (Kross::Input::IsKeyPressed(KROSS_KEY_F11)) manager.fullscreen = !manager.fullscreen;
		if (app.GetWindow().FullScreen(manager.fullscreen))
		{
			manager.viewportsState = manager.viewportsEnabled = false;
			io.ConfigFlags &= ~ImGuiConfigFlags_ViewportsEnable;
		}
		else manager.viewportsState = true;
	}
}

void UI::OnEvent(Kross::Event& event)
{
}

void UI::setCanvas(const Kross::Ref<Kross::Texture::T2D>& texture)
{
	canvas.reset(texture.get());
}

int UI::MainWindow(bool* p_open, const char* id)
{
	static const char* pop_msg = "";

	ImGuiIO& io = ImGui::GetIO();

	// ImGui Demo Window
	if(manager.show_demo_window) ImGui::ShowDemoWindow(&manager.show_demo_window);
	if (manager.show_style_editor)
	{
		pop_msg = "Error: 20";
		manager.popup_message_box = true;
	}
	//if(manager & MGR_DEMO_WINDOW) ImGui::ShowDemoWindow((bool*)1);
	//if (manager & MGR_STYLE_EDITOR)
	//{
	//
	//}

	manager.keyboardEnabled = io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard;
	manager.gamepadEnabled = io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad;
	manager.viewportsEnabled = io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable;

	static ImGuiDockNodeFlags dock_flags = 0;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoBringToFrontOnFocus;
	if (manager.opt_fullscreen_persistant)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		//window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		//window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

	
	//When using ImGuiDockNodeFlags_PassthruDockspace,
	//DockSpace() will render our background and handle the pass-thru hole,
	//so we ask Begin() to not render a background.
	//
	//if (dock_flags & ImGuiDockNodeFlags_PassthruDockspace) window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin(id, p_open, window_flags);
	ImGui::PopStyleVar();
	ImGuiID dockspace_id = ImGui::GetID(id);

	if (manager.opt_fullscreen_persistant) ImGui::PopStyleVar(2);
	if (Kross::Input::IsKeyPressed(KROSS_KEY_ESCAPE)) ImGui::SetWindowFocus();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dock_flags);
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Menu"))
		{
			ImGui::MenuItem("GUI Demo", NULL, &manager.show_demo_window);
			ImGui::MenuItem("Status", NULL, &manager.show_rendererStats);
			ImGui::MenuItem("Fullscreen", NULL, &manager.fullscreen);
			if (ImGui::MenuItem("Open", NULL, &manager.popup_message_box)) pop_msg = "Error: 20";
			ImGui::Separator();
			if (ImGui::MenuItem("Close", "Alt+F4", false, p_open != NULL))
				*p_open = false;
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Config"))
		{
			if (ImGui::BeginMenu("Settings"))
			{
				ImGui::MenuItem("ImGui", NULL, &manager.show_style_editor);

				ImGui::Separator();

				ImGui::EndMenu();
			}
			ImGui::Separator();
			ShowHelpMarker("Enable or Disable Keyboard navigation through the Graphical Interfaces"); ImGui::SameLine();
			ImGui::MenuItem("Keyboard", NULL, &manager.keyboardEnabled, manager.keyboardState);
			ShowHelpMarker("Enable or Disable Gamepad navigation through the Graphical Interfaces"); ImGui::SameLine();
			ImGui::MenuItem("Gamepad", NULL, &manager.gamepadEnabled, manager.keyboardState);
			ShowHelpMarker("Enable or Disable Viewports to create another window on the operting system"); ImGui::SameLine();
			ImGui::MenuItem("Viewports", NULL, &manager.viewportsEnabled, manager.viewportsState);
			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Camera"))
		{
			ImGui::MenuItem("Window", NULL, &manager.show_camera);

			ImGui::Separator();

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("Show Help", "F1", &manager.popup_message_box)) pop_msg = "Error: 20";
			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}

	MessageBoxDialog(&manager.popup_message_box, pop_msg);

	ImGui::End();
	return dockspace_id;
}

void UI::viewport(bool* p_open)
{
	if (p_open == nullptr || !(*p_open)) return;
	if (!canvas) {
		KROSS_WARN_ONCE("canvas is empty");
		return;
	}
	KROSS_TRACE_ONCE(canvas->GetName());
	using namespace ImGui;
	{
		ImGuiIO& io = GetIO();
		ImGuiWindowFlags flags = ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoBackground;
		if (Begin("viewport", p_open, flags)) {
			Image((void*)(intptr_t)canvas->GetID(),
				GetContentRegionAvail(),
				ImVec2(0, 1),
				ImVec2(1, 0));
			ImVec2 size = GetWindowSize();
			End();
		}
	}
}

void UI::Status(bool* show_rendererStats, float ts)
{
	if (show_rendererStats == nullptr || !(*show_rendererStats)) return;
	static bool pause_frame_plot_animation = false;

	static const char* frame_plot_options[2] = { "Frames per Second", "Elapsed Time" };
	static int plot_type = 0;

	float f = plot_type ? ts : 1 / ts;
	float fps = ts;
	static size_t frame_count = 0;
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
	if (frame_count++ % rate_tick == 0) { FrameRate = framerate_buffer / rate_tick; framerate_buffer = 0; }

	if (show_rendererStats)
	{
		char buf[128];
		sprintf_s(buf, "Status Monitor | FPS: %.1f###StatusTitle", FrameRate);
		if (ImGui::Begin(buf, show_rendererStats, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoCollapse))
		{
			if (ImGui::Combo("mode", &plot_type, frame_plot_options, 2)) once = false;
			ImGui::SameLine(); ShowHelpMarker(
				R"(Select what plot type you want.
Frames per Second mode, or FPS, is the number of how many frames the application can push out in a single second.
Elapsed Time mode, is the plot of how many milisseconds has passed since the last frame was drawn.
)");
			ImGui::Checkbox("Pause Animation", &pause_frame_plot_animation);
			float max = 0;
			for (size_t i = 0; i < plotsize; i++)
				max = std::max(frames[i], max);
			ImGui::PlotLines("", frames, plotsize - 1, 0, plot_type ? "Timestamp" : "Frames", 0, std::min(120.0f, max));
			auto& mouse = Kross::Input::GetMousePosition();
			ImGui::Text("Mouse: %.1f, %.1f", mouse.first, mouse.second);

			//ImGui::Text("MaxQuadCount: %d", Kross::Renderer2D::getStats().maxQuads);
			//ImGui::Text("Quad Count: %d", Kross::Renderer2D::getStats().QuadCount);
			//ImGui::Text("Draw Calls: %d", Kross::Renderer2D::getStats().DrawCount);
		}
		ImGui::End();
	}
}