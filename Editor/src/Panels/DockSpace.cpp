#include "DockSpace.h"

namespace Kross {
	DockSpace::DockSpace(const std::string& name)
		: m_strName(name)
	{
	}
	DockSpace::~DockSpace()
	{
	}
	void DockSpace::Show(double ts)
	{
		//if (m_bShowed) { KROSS_WARN("Calling Begin without End"); return; }
		//m_bShowed = true;

		ImGuiIO& io = ImGui::GetIO();
		static MessageBoxSpecs mbSpecs;

		AppManager().s_bKeyboardEnabled = io.ConfigFlags & ImGuiConfigFlags_NavEnableKeyboard;
		AppManager().s_bGamepadEnabled = io.ConfigFlags & ImGuiConfigFlags_NavEnableGamepad;
		AppManager().s_bViewportEnabled = io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable;

		static ImGuiDockNodeFlags dock_flags = ImGuiDockNodeFlags_None;

		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (m_bFullscreen)
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

		if (dock_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(m_strName.c_str(), &m_bOpen, window_flags);
		ImGui::PopStyleVar(m_bFullscreen ? 3 : 1);

		// DockSpace
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dock_flags);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Open", NULL)) mbSpecs = { true, "Open File Dialog", OK, DefaultFunc };
				ImGui::Separator();
				if (ImGui::MenuItem("Close", "Alt+F4", false))
					m_bOpen = false;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Config"))
			{
				if (ImGui::BeginMenu("Settings"))
				{
					if (ImGui::MenuItem("ImGui", NULL)) mbSpecs = { true, "Style Editor", OK, DefaultFunc };
					ImGui::Separator();
					ImGui::MenuItem("GUI Demo", NULL, &Manager().s_bDemoWindow);
					ImGui::MenuItem("Status", NULL, &Manager().s_bRendererStats);
					ImGui::MenuItem("Fullscreen", NULL, &Manager().s_bFullscreen);
					ImGui::EndMenu();
				}
				ImGui::Separator();
				ShowHelperMarker("Enable or Disable Keyboard navigation through the Graphical Interfaces"); ImGui::SameLine();
				ImGui::MenuItem("Keyboard", NULL, &AppManager().s_bKeyboardEnabled);
				ShowHelperMarker("Enable or Disable Gamepad navigation through the Graphical Interfaces"); ImGui::SameLine();
				ImGui::MenuItem("Gamepad", NULL, &AppManager().s_bGamepadEnabled);
				ShowHelperMarker("Enable or Disable Viewports to create another window on the operting system"); ImGui::SameLine();
				ImGui::MenuItem("Viewports", NULL, &AppManager().s_bViewportEnabled);
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Camera"))
			{
				if (ImGui::MenuItem("Window", NULL)) mbSpecs = { true, "Camera Window", OK, []() { ImGui::Text("Camera Inspector is yet to be implemented."); } };
				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show Help", "F1")) mbSpecs = { true, "Show Help", OK, DefaultFunc };
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// ImGui Demo Window
		if (Manager().s_bDemoWindow) ImGui::ShowDemoWindow(&Manager().s_bDemoWindow);
		MessageBoxDialog(mbSpecs);
		ImGui::End();
	}
}