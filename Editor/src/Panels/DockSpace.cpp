#include "Editor_pch.h"
#include "DockSpace.h"

namespace Kross {
	DockSpace::DockSpace()
	{
		m_Flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		m_strName = "Kross DockSpace";
		KROSS_INFO("Panel '{0}' Constructed", m_strName);
	}

	void DockSpace::Menu()
	{
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				if (ImGui::MenuItem("Open", NULL)) PopUpMessage = { true, "Open File Dialog", ButtonType::OK, DefaultFunc };
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
				ImGui::MenuItem("Scene Hierarchy", NULL, &Manager().s_bSceneHierarchy);
				ImGui::MenuItem("Entity Inspector", NULL, &Manager().s_bPropertiesInspector);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::MenuItem("Style", NULL)) PopUpMessage = { true, "Style Editor", ButtonType::OK, DefaultFunc };
				ImGui::Separator();
				Panel::ShowHelperMarker("Enables or Disable FullScreen mode."); ImGui::SameLine();
				ImGui::MenuItem("Fullscreen", NULL, &Manager().s_bFullscreen);
				Panel::ShowHelperMarker("Enable or Disable Keyboard navigation through the Graphical Interfaces"); ImGui::SameLine();
				ImGui::MenuItem("Keyboard", NULL, &AppManager().s_bKeyboardEnabled);
				Panel::ShowHelperMarker("Enable or Disable Gamepad navigation through the Graphical Interfaces"); ImGui::SameLine();
				ImGui::MenuItem("Gamepad", NULL, &AppManager().s_bGamepadEnabled);
				Panel::ShowHelperMarker("Enable or Disable Viewports to create another window on the operting system"); ImGui::SameLine();
				ImGui::MenuItem("Viewports", NULL, &AppManager().s_bViewportEnabled);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show Help", NULL)) PopUpMessage = { true, "Show Help", ButtonType::OK, DefaultFunc };
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}
	}

	void DockSpace::Show(double ts)
	{
		ImGuiIO &io = ImGui::GetIO();

		if (m_bFullscreen)
		{
			ImGuiViewport *viewport = ImGui::GetMainViewport();
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

		ImGui::GetStyle().WindowMinSize.x = 300.0f;

		// DockSpace
		ImGui::DockSpace(ImGui::GetID(m_strName), ImVec2(0.0f, 0.0f), m_DockFlags);

		// MenuBar
		Menu();

		// ImGui Demo Window
		if (Manager().s_bDemoWindow) ImGui::ShowDemoWindow(&Manager().s_bDemoWindow);
		Panel::MessageBoxDialog(PopUpMessage);
		ImGui::End();
	}
}