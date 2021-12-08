#include "DockSpace.h"

namespace Kross {
	DockSpace::DockSpace()
	{
		m_strName = "Kross DockSpace";
		KROSS_INFO("DockSpace Constructed");
		m_Flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	}

	void DockSpace::Menu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Open", NULL)) PopUpMessage = { true, "Open File Dialog", OK, DefaultFunc };
				ImGui::Separator();
				if (ImGui::MenuItem("Close", "Alt+F4", false))
					Manager().s_bDockspace = false;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				ImGui::MenuItem("Viewport", NULL, &Manager().s_bViewport);
				ImGui::MenuItem("GUI Demo", NULL, &Manager().s_bDemoWindow);
				ImGui::MenuItem("Status", NULL, &Manager().s_bRendererStats);
				ImGui::MenuItem("Entity Inspector", NULL, &Manager().s_bEntityInspector);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::MenuItem("ImGui", NULL)) PopUpMessage = { true, "Style Editor", OK, DefaultFunc };
				ImGui::Separator();
				ShowHelperMarker("Enables or Disable FullScreen mode."); ImGui::SameLine();
				ImGui::MenuItem("Fullscreen", NULL, &Manager().s_bFullscreen);
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
				ImGui::MenuItem("Editor Camera", NULL, &AppManager().s_bEditorCamera);
				ImGui::Separator();
				if (ImGui::MenuItem("Window", NULL)) PopUpMessage = { true, "Camera Window", OK, []() { ImGui::Text("Camera Inspector is yet to be implemented."); } };
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show Help", NULL)) PopUpMessage = { true, "Show Help", OK, DefaultFunc };
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void DockSpace::Show(double ts)
	{
		ImGuiIO& io = ImGui::GetIO();

		if (m_bFullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			m_Flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			m_Flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (m_DockFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			m_Flags |= ImGuiWindowFlags_NoBackground;


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(m_strName, &Manager().s_bDockspace, m_Flags);
		ImGui::PopStyleVar(m_bFullscreen ? 3 : 1);

		// DockSpace
		ImGui::DockSpace(ImGui::GetID(m_strName), ImVec2(0.0f, 0.0f), m_DockFlags);

		// MenuBar
		Menu();

		// ImGui Demo Window
		if (Manager().s_bDemoWindow) ImGui::ShowDemoWindow(&Manager().s_bDemoWindow);
		MessageBoxDialog(PopUpMessage);
		ImGui::End();
	}
}