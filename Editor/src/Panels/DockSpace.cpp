#include "Editor_pch.h"
#include "EditorLayer.h"
#include "Panels.h"
#include "Kross/Util/PlatformUtils.h"

namespace Kross {
	
	void EditorLayer::Dockspace()
	{
		static struct DockspaceSettings
		{
			bool m_bShowed = false;
			bool m_bMessageBox = false;
			bool m_bStyleEditor = false;
			bool m_bFullscreen = true;
			uint32_t m_DockFlags = 0;
			uint32_t m_Flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		} data;
		ImGuiIO &io = ImGui::GetIO();
		static const char *id = "Kross_DockSpace";
		static std::string pop = "none\\none";

		if (data.m_bFullscreen)
		{
			ImGuiViewport *viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			data.m_Flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			data.m_Flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		if (data.m_DockFlags & ImGuiDockNodeFlags_PassthruCentralNode)
			data.m_Flags |= ImGuiWindowFlags_NoBackground;


		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin(id, &Panel::Manager().s_bDockspace, data.m_Flags);
		ImGui::PopStyleVar(data.m_bFullscreen ? 3 : 1);

		ImGui::GetStyle().WindowMinSize.x = 300.0f;

		// DockSpace
		ImGui::DockSpace(ImGui::GetID(id), ImVec2(0.0f, 0.0f), data.m_DockFlags);
		auto newScene = [&]() {
			m_Scene = Scene("main");//makeRef<Scene>();
			sceneHierarchy->SetContext(m_Scene);
			entityProperties->SetContext(m_Scene);
		};

		// MenuBar
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("New", NULL)) newScene();
				if (ImGui::MenuItem("Open...", NULL)) { m_Scene.Clear(); m_Scene.LoadScene(); }
				if (ImGui::MenuItem("Save", NULL)) m_Scene.SaveScene();
				if (ImGui::MenuItem("Save As...", NULL)) m_Scene.SaveScene(FileDialog::SaveFile("Kross Scene (*.kross)\0*.kross\0\0"));
				
				ImGui::Separator();
				if (ImGui::MenuItem("Close", "Alt+F4", false))
					Panel::Manager().s_bDockspace = false;
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("View"))
			{
				ImGui::MenuItem("Viewport", NULL, &Panel::Manager().s_bViewport);
				ImGui::MenuItem("GUI Demo", NULL, &Panel::Manager().s_bDemoWindow);
				ImGui::MenuItem("Status", NULL, &Panel::Manager().s_bRendererStats);
				ImGui::MenuItem("Scene Hierarchy", NULL, &Panel::Manager().s_bSceneHierarchy);
				ImGui::MenuItem("Entity Inspector", NULL, &Panel::Manager().s_bPropertiesInspector);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Settings"))
			{
				if (ImGui::MenuItem("Style", NULL)) pop = "Style Editor\\none";
				ImGui::Separator();
				Panel::ShowHelperMarker("Enables or Disable FullScreen mode."); ImGui::SameLine();
				ImGui::MenuItem("Fullscreen", NULL, &Panel::Manager().s_bFullscreen);
				Panel::ShowHelperMarker("Enable or Disable Keyboard navigation through the Graphical Interfaces"); ImGui::SameLine();
				ImGui::MenuItem("Keyboard", NULL, &Panel::AppManager().s_bKeyboardEnabled);
				Panel::ShowHelperMarker("Enable or Disable Gamepad navigation through the Graphical Interfaces"); ImGui::SameLine();
				ImGui::MenuItem("Gamepad", NULL, &Panel::AppManager().s_bGamepadEnabled);
				Panel::ShowHelperMarker("Enable or Disable Viewports to create another window on the operting system"); ImGui::SameLine();
				ImGui::MenuItem("Viewports", NULL, &Panel::AppManager().s_bViewportEnabled);
				ImGui::MenuItem("EditorCamera", NULL, &Panel::AppManager().s_bEditorCamera);
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				if (ImGui::MenuItem("Show Help", NULL)) pop = "Show Help\\none";
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// ImGui Demo Window
		if (Panel::Manager().s_bDemoWindow) ImGui::ShowDemoWindow(&Panel::Manager().s_bDemoWindow);
		std::string title = "PopUp";
		std::string message = "";
		if (pop != "none\\none") {
			std::string buff = pop.substr(0, pop.find_last_of('\\'));
			if (!buff.empty() && buff != "none") title = buff;
			buff = pop.substr(pop.find_first_of('\\') + 1);
			if (!buff.empty() && buff != "none") message = buff;
			ImGui::OpenPopup(title.c_str());
			glm::vec2 center = glm::vec2((float)Application::Get().GetWindow().GetWidth(), (float)Application::Get().GetWindow().GetHeight()) * 0.5f;
			ImGui::SetNextWindowPos({ center.x, center.y }, 0, { 0.5f, 0.5f });
		}
		if (ImGui::BeginPopupModal(title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
			ImGui::Text(message.c_str());
			ImGui::PushItemWidth(-ImGui::GetWindowSize().x / 2);
			if (ImGui::Button("OK", ImVec2(50, 0))) {
				ImGui::CloseCurrentPopup();
				pop = "none\\none";
			}
			ImGui::PopItemWidth();
			ImGui::EndPopup();
		}
		ImGui::End();

	}
}
//
//static struct DockSpaceData
//{
//	bool showed = false;
//	bool messageBox = false;
//	bool styleEditor = false;
//	bool fullscreen = true;
//	ImGuiDockNodeFlags dockFlags = 0;
//	ImGuiWindowFlags flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
//} data;
//
//namespace Kross {
//	void EditorLayer::DockSpace()
//	{
//		//static Panel::MessageBoxSpecs PopUpMessage;
//		ImGuiIO &io = ImGui::GetIO();
//		static std::string pop = "none\\none";
//
//		if (data.fullscreen)
//		{
//			ImGuiViewport *viewport = ImGui::GetMainViewport();
//			ImGui::SetNextWindowPos(viewport->Pos);
//			ImGui::SetNextWindowSize(viewport->Size);
//			ImGui::SetNextWindowViewport(viewport->ID);
//			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
//			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
//			data.flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
//			data.flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
//		}
//
//		if (data.dockFlags & ImGuiDockNodeFlags_PassthruCentralNode)
//			data.flags |= ImGuiWindowFlags_NoBackground;
//
//
//		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
//		ImGui::Begin(m_strName, &Panel::Manager().s_bDockspace, data.flags);
//		ImGui::PopStyleVar(data.fullscreen ? 3 : 1);
//
//		ImGui::GetStyle().WindowMinSize.x = 300.0f;
//
//		// DockSpace
//		ImGui::DockSpace(ImGui::GetID(m_strName), ImVec2(0.0f, 0.0f), data.dockFlags);
//
//		// MenuBar
//		if (ImGui::BeginMenuBar())
//		{
//			if (ImGui::BeginMenu("File"))
//			{
//				if (ImGui::MenuItem("New", NULL)) {
//					m_Scene = makeRef<Scene>("main");
//					//sceneHierarchyPanel->SetContext(m_Scene);
//					//entityPropertiesPanel->SetContext(m_Scene);
//				}
//				if (ImGui::MenuItem("Open...", NULL)) pop = "Open File Dialog\\";
//				if (ImGui::MenuItem("Save", NULL)) pop = "Save File\\";
//				if (ImGui::MenuItem("Save As...", NULL)) pop = "Save File As Dialog\\";
//				ImGui::Separator();
//				if (ImGui::MenuItem("Close", "Alt+F4", false))
//					Panel::Manager().s_bDockspace = false;
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("View"))
//			{
//				ImGui::MenuItem("Viewport", NULL, &Panel::Manager().s_bViewport);
//				ImGui::MenuItem("GUI Demo", NULL, &Panel::Manager().s_bDemoWindow);
//				ImGui::MenuItem("Status", NULL, &Panel::Manager().s_bRendererStats);
//				ImGui::MenuItem("Scene Hierarchy", NULL, &Panel::Manager().s_bSceneHierarchy);
//				ImGui::MenuItem("Entity Inspector", NULL, &Panel::Manager().s_bPropertiesInspector);
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("Settings"))
//			{
//				if (ImGui::MenuItem("Style", NULL)) pop = "Style Editor\\The Style Editor will be implemented soon";
//				ImGui::Separator();
//				Panel::ShowHelperMarker("Enables or Disable FullScreen mode."); ImGui::SameLine();
//				ImGui::MenuItem("Fullscreen", NULL, &Panel::Manager().s_bFullscreen);
//				Panel::ShowHelperMarker("Enable or Disable Keyboard navigation through the Graphical Interfaces"); ImGui::SameLine();
//				ImGui::MenuItem("Keyboard", NULL, &Panel::AppManager().s_bKeyboardEnabled);
//				Panel::ShowHelperMarker("Enable or Disable Gamepad navigation through the Graphical Interfaces"); ImGui::SameLine();
//				ImGui::MenuItem("Gamepad", NULL, &Panel::AppManager().s_bGamepadEnabled);
//				Panel::ShowHelperMarker("Enable or Disable Viewports to create another window on the operting system"); ImGui::SameLine();
//				ImGui::MenuItem("Viewports", NULL, &Panel::AppManager().s_bViewportEnabled);
//				ImGui::EndMenu();
//			}
//			if (ImGui::BeginMenu("Help"))
//			{
//				if (ImGui::MenuItem("Show Help", NULL)) pop = "Show Help\\Kross Engine\nCreated by Willian Koessler (2021)";
//				ImGui::EndMenu();
//			}
//			ImGui::EndMenuBar();
//		}
//		// ImGui Demo Window
//		if (Panel::Manager().s_bDemoWindow) ImGui::ShowDemoWindow(&Panel::Manager().s_bDemoWindow);
//
//
//		std::string title = pop.substr(0, pop.find_last_of('\\'));
//		std::string msg = pop.substr(pop.find_first_of('\\') + 1);
//
//		if (pop != "none\\none") {
//			ImGui::OpenPopup(title.c_str());
//			glm::vec2 center = glm::vec2((float)Application::Get().GetWindow().GetWidth(), (float)Application::Get().GetWindow().GetHeight()) * 0.5f;
//			ImGui::SetNextWindowPos({ center.x, center.y }, 0, { 0.5f, 0.5f });
//		}
//		if (ImGui::BeginPopupModal(title.c_str(), NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove))
//		{
//			ImGui::Text(msg.c_str());
//			auto w = ImGui::GetWindowSize().x;
//			ImGui::PushItemWidth(-w / 2);
//			if (ImGui::Button("OK", ImVec2(50, 0))) {
//				pop = "none\\none";
//				ImGui::CloseCurrentPopup();
//			}
//			ImGui::PopItemWidth();
//			ImGui::EndPopup();
//		}
//
//		ImGui::End();
//	}
//}
