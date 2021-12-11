#pragma once

#include "Kross/Core/Core.h"

#include "Kross/Core/Window.h"

#include "Kross/Core/LayerStack.h"
#include "Kross/Core/Layer.h"
#include "Kross/imgui/ImGuiLayer.h"

#include "Kross/Events/Event.h"
#include "Kross/Events/ApplicationEvent.h"

int main(int argc, char **args);

namespace Kross {
	class Application
	{
		struct settings
		{
			const char* title = "Kross Engine";
			uint32_t width = 640, height = 320;
			bool fullscreen = false;
			uint8_t dims;
		};
		void Construct(settings&&);
	public:
		Application(const char* title, uint32_t width, uint32_t height, uint8_t dimentions) { Construct({ title, width, height, false, dimentions }); }

		~Application();

		void Run();

		void OnEvent(Event& e);
		void SetDarkTheme() const { m_ImGuiLayer->SetDarkTheme(); }
		const Ref<Layer> &GetGUILayer() { return m_ImGuiLayer; }
		void PushLayer(const Ref<Layer>& layer);
		void PushLayer(const std::initializer_list<Ref<Layer>>& list);
		void PushOverlay(const Ref<Layer>& layer);
		inline Window& GetWindow() { return *m_pWindow; }
		inline static Application& Get() { return *s_Instance; }
		inline double GetTime() const;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		bool m_bRunning = true;
		bool m_bMinimized = false;
		Window* m_pWindow;
		LayerStack m_pLayerStack;
		Ref<ImGuiLayer> m_ImGuiLayer;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char **args);
	};
	
	//Client-side Definition
	Application* CreateApp();
}
