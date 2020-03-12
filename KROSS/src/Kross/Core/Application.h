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
		void Construct(const std::string& title = "Kross Engine", unsigned short width = 640, unsigned short height = 320);
	public:
		Application()
			: m_LayerStack() {
			Construct();
		}
		Application(const std::string& title, unsigned short width, unsigned short height)
			: m_LayerStack() {
			Construct(title, width, height);
		}
		~Application();

		void Run();

		void OnEvent(Event& e);
		void PushLayer(const Ref<Layer>& layer);
		void PushOverlay(const Ref<Layer>& layer);
		inline Window& GetWindow() { return *m_uptrWindow; }
		inline static Application& Get() { return *s_Instance; }

	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);

	private:
		Scope<Window> m_uptrWindow;
		bool m_bRunning = true;
		bool m_bMinimized = false;
		LayerStack m_LayerStack;
		Ref<ImGuiLayer> m_ptrImGuiLayer;
		float previous_time = 0.0f;

	private:
		static Application* s_Instance;
	};


	// Client-Side Definition
	Application* CreateApp();
}
