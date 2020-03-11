#include "Kross_pch.h"
#include "Application.h"
#include "Input.h"

#include "GLFW/glfw3.h"

namespace Kross {

	Application* Application::s_Instance = nullptr;

	Application::Application()
		: m_LayerStack()
	{
		KROSS_PROFILE_FUNC();
		if (s_Instance) KROSS_MSGBOX("Application already exists!", "[Kross::Application]", _FATAL_);
		s_Instance = this;

		m_uptrWindow = Scope<Window>(Window::Create(WindowProps()));
		m_uptrWindow->SetVSync(true);
		m_uptrWindow->SetEventCallback(KROSS_BIND_EVENT_FN(Application::OnEvent));
		Renderer::Init();

		m_ptrImGuiLayer = makeRef<ImGuiLayer>();
		PushOverlay(m_ptrImGuiLayer);

		KROSS_CORE_INFO("[Kross::Application] Application Contructed");
	}

	Application::~Application()
	{
		KROSS_PROFILE_FUNC();
		m_LayerStack.~LayerStack();
		Renderer::Shutdown();
		m_uptrWindow->Shutdown();
		m_uptrWindow.release();
		KROSS_CORE_INFO("[Kross::Application] Application Destructed");
	}

	void Application::PushLayer(const Ref<Layer>& layer)
	{
		KROSS_PROFILE_FUNC();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
		KROSS_CORE_TRACE("[Kross::Application] Application '{0}' Pushed", layer->GetName());
	}

	void Application::PushOverlay(const Ref<Layer>& layer)
	{
		KROSS_PROFILE_FUNC();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
		KROSS_CORE_TRACE("[Kross::Application] Application Overlay '{0}' Pushed", layer->GetName());
	}

	void Application::OnEvent(Event& e)
	{
		KROSS_PROFILE_FUNC();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(KROSS_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(KROSS_BIND_EVENT_FN(Application::OnWindowResize));

		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.handled)
				break;
		}
	}

	void Application::Run()
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_FILE_TRACE("-----------------------RUNTIME STARTED-----------------------");
		while (m_bRunning)
		{
			KROSS_PROFILE_SCOPE("Run Loop");
			float time = (float)glfwGetTime();
			Timestep ts = time - previous_time;
			previous_time = time;

			if (!m_bMinimized)
			{
				KROSS_PROFILE_SCOPE("LayerStack OnUpdate");
				for (Ref<Layer>& layer : m_LayerStack)
					layer->OnUpdate(ts);
			}

			m_ptrImGuiLayer->Begin();
			if(!m_bMinimized)
			{
				KROSS_PROFILE_SCOPE("LayerStack ImGuiRender");
				for (Ref<Layer>& layer : m_LayerStack)
					layer->OnImGuiRender(ts);
			}
			m_ptrImGuiLayer->End();

			m_uptrWindow->OnUpdate();
		}
		KROSS_CORE_FILE_TRACE("-----------------------RUNTIME ENDED-----------------------");

		KROSS_CORE_TRACE("[Kross::Application] Application Finished");
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		KROSS_PROFILE_FUNC();
		m_bRunning = false;
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		KROSS_PROFILE_FUNC();
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_bMinimized = true;
			KROSS_CORE_TRACE("[Kross::Application] Application Minimized");
			return false;
		}
		m_bMinimized = false;
		Renderer::Command::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}
