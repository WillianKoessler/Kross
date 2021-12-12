#pragma once

namespace Kross {
	class Panel
	{
	protected:
		static struct PanelManager
		{
			bool s_bDockspace = true;
			bool s_bDemoWindow = false;
			bool s_bRendererStats = false;
			bool s_bViewport = true;
			bool s_bSceneHierarchy = true;
			bool s_bPropertiesInspector = true;

			bool s_bFullscreen;
		} s_Manager;
		static struct AppSettings
		{
			bool s_bEditorCamera = true;
			bool s_bKeyboardEnabled = false;
			bool s_bGamepadEnabled = false;
			bool s_bViewportEnabled = false;
		} s_AppManager;

		enum class ButtonType { OK, OK_CANCEL, OK_RETRY_CANCEL, YES_NO, YES_NO_CANCEL };
		
		struct MessageBoxSpecs
		{
			bool show = false;
			char* id = nullptr;
			ButtonType type = ButtonType::OK;
			void(*func)(void) = DefaultFunc;
		};
	
	protected:
		virtual void Menu() {}
		static void ShowHelperMarker(const std::string& msg, float size = 35.0f);
		static void MessageBoxDialog(MessageBoxSpecs& specs);
		static void ButtonOK(MessageBoxSpecs &specs);
		static void ButtonOK_CANCEL(MessageBoxSpecs &specs);
		static void ButtonOK_RETRY_CANCEL(MessageBoxSpecs &specs);
		static void ButtonYES_NO(MessageBoxSpecs &specs);
		static void ButtonYES_NO_CANCEL(MessageBoxSpecs &specs);
	
	protected:
		static void(*DefaultFunc)(void);
		static Entity s_Selection;
		MessageBoxSpecs PopUpMessage;
		uint32_t m_Flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_Popup;
		const char *m_strName;

	public:
		Panel();
		virtual ~Panel() {};

		virtual void Show(double ts) = 0;

		static void setFlag(ImGuiConfigFlags flag, bool value) { value ? ImGui::GetIO().ConfigFlags |= flag : ImGui::GetIO().ConfigFlags &= ~flag; }

		static PanelManager &Manager() { return s_Manager; }
		static AppSettings &AppManager() { return s_AppManager; }
	};
}
