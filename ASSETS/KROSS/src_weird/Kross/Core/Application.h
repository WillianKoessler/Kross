#pragma once

#include "Core.h"
#include "Window.h"
#include "LayerStack.h"
#include "Kross/Events/Event.h"
#include "Kross/Events/ApplicationEvent.h"

#include "Kross/Core/Timestep.h"

#include "Kross/Renderer/Renderer.h"

#include "Kross/imgui/ImGuiLayer.h"

namespace Kross {
	class KAPI Application
	{
		struct settings
		{
			std::string title = "Kross Engine";
			unsigned short width = 640, height = 320;
			bool fullscreen = false;
			Renderer::Dimentions dims;
		};
		void Construct(settings&&);
	public:
		Application() { Construct({}); }
		Application(const std::string& title, unsigned short width, unsigned short height, Renderer::Dimentions dimentions)
		{
			Construct({ title, width, height, false, dimentions });
		}
		Application(const std::string& title, unsigned short width, unsigned short height, bool fullscreen = false)
		{
			Construct({ title, width, height, fullscreen });
		}
		Application(const std::string& title, bool fullscreen = true)
		{
			Construct({title, 0, 0, fullscreen});
		}

		~Application();

		void Run();

		void OnEvent(Event& e);
		void PushLayer(const Ref<Layer>& layer);
		void PushLayer(const std::initializer_list<Ref<Layer>>& list);
		void PushOverlay(const Ref<Layer>& layer);
		inline Window& GetWindow() { return *m_uptrWindow; }
		inline static Application& Get() { return *s_Instance; }
		inline double GetTime() const;

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_uptrWindow;
		bool m_bRunning = true;
		bool m_bMinimized = false;
		LayerStack m_LayerStack;
		Ref<ImGuiLayer> m_ptrImGuiLayer;

	private:
		static Application* s_Instance;
	};


	// Client-Side Definition
	Application* CreateApp();
}
