#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Window.h"
#include "Kross/Core/Timestep.h"
#include "Kross/Core/LayerStack.h"
#include "Kross/imgui/ImGuiLayer.h"
#include "Kross/Renderer/Renderer.h"
#include "Kross/Events/Event.h"
#include "Kross/Events/ApplicationEvent.h"
int main(int argc, char** args);

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
		//Application() { Construct({}); }
		Application(const char* title, uint32_t width, uint32_t height, uint8_t dimentions) { Construct({ title, width, height, false, dimentions }); }
		
		//Application(const std::string& title, uint8_t width, uint8_t height, bool fullscreen = false) { Construct({ title, width, height, fullscreen }); }
		//Application(const std::string& title, bool fullscreen = true) { Construct({title, 0, 0, fullscreen}); }

		~Application();

		void Run();

		void OnEvent(Event& e);
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
		ImGuiLayer* m_refImGuiLayer;

	private:
		static Application* s_Instance;
		friend int ::main(int argc, char** argv);
	};

	// Client-Side Definition
	Application* CreateApp();
}
//extern Kross::Application* Kross::CreateApp();
