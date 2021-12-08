#pragma once

#include <string>
#include <functional>
#include "Kross.h"

namespace Kross {
	class Panel : public Resource
	{
	protected:
		struct PanelManager
		{
			bool s_bDockspace;
			bool s_bDemoWindow;
			bool s_bRendererStats;
			bool s_bViewport;
			bool s_bEntityInspector;

			bool s_bFullscreen;
		};
		struct AppSettings
		{
			bool s_bKeyboardEnabled = false;
			bool s_bGamepadEnabled = false;
			bool s_bViewportEnabled = false;
		};

		enum MessageBoxButtons { OK, OK_CANCEL, OK_RETRY_CANCEL, YES_NO, YES_NO_CANCEL };
		
		std::function<void()> DefaultFunc = []() { ImGui::Text("This feature has not been yet implemented."); };
		
		struct MessageBoxSpecs
		{
			bool show = false;
			char* id = nullptr;
			MessageBoxButtons buttontype = OK;
			std::function<void()> func;
		};

		void ShowHelperMarker(const std::string& msg, float size = 35.0f);
		void MessageBoxDialog(MessageBoxSpecs& specs);
		virtual void Menu() {}

		MessageBoxSpecs m_PopUpSpecs;
		uint32_t m_Flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_Popup;

	public:
		Panel() { SetName("Unnamed_Panel"); }
		virtual ~Panel() {};

		virtual void Show(double ts) = 0;

		static void setFlag(ImGuiConfigFlags flag, bool value) { value ? ImGui::GetIO().ConfigFlags |= flag : ImGui::GetIO().ConfigFlags &= ~flag; }

		static PanelManager &Manager();
		static AppSettings &AppManager();
	};
}
