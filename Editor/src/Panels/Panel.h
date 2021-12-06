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
			bool s_bDemoWindow = false;
			bool s_bRendererStats = false;
			bool s_bViewport = true;
			bool s_bFullscreen = false;
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
	public:
		Panel() { SetName("Unnamed_Panel"); }
		virtual ~Panel() {};
		void ShowHelperMarker(const std::string& msg, float size = 35.0f);
		void MessageBoxDialog(MessageBoxSpecs& specs);
		virtual void Show(double ts) = 0;
		inline static void setFlag(ImGuiConfigFlags flag, bool value) { value ? ImGui::GetIO().ConfigFlags |= flag : ImGui::GetIO().ConfigFlags &= ~flag; }
		static PanelManager& Manager() { return s_Manager; }
		static AppSettings& AppManager() { return s_AppManager; }
	protected:
		static PanelManager s_Manager;
		static AppSettings s_AppManager;
	};
}
