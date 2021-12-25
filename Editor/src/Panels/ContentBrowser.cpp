#include "Editor_pch.h"
#include "ContentBrowser.h"
#include "Panel.h"

#include <filesystem>

constexpr char *s_AssetsDirectory = "assets";

namespace Kross {
	ContentBrowser::ContentBrowser()
	{
		SetName("Content Browser");
		m_Flags = ImGuiWindowFlags_MenuBar;
	}
	void ContentBrowser::Show(double ts)
	{
		if (!Panel::Manager().s_bContentBrowser) return;
		ImGui::Begin(GetName());
		// TODO: Create content browser imgui code
		ImGui::End();
	}
}
