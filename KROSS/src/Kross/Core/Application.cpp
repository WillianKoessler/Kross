#include "Kross_pch.h"
#include "Application.h"

#include "Stack.h"
#include "Kross/Renderer/Textures/Textures.h"

namespace Kross {

	Application* Application::s_Instance = nullptr;

	void Application::Construct(settings&& s)
	{
		KROSS_PROFILE_FUNC();
		if (s_Instance) KROSS_MSGBOX("Application already exists!", __FUNCTION__, _FATAL_);
		s_Instance = this;

		m_uptrWindow = Scope<Window>(Window::Create(WindowProps(s.title, s.width, s.height, s.fullscreen)));
		m_uptrWindow->SetVSync(true);
		m_uptrWindow->SetEventCallback(KROSS_BIND_EVENT_FN(Application::OnEvent));
		Renderer::Init(s.dims);

		m_ptrImGuiLayer = makeRef<ImGuiLayer>();
		PushOverlay(m_ptrImGuiLayer);

		KROSS_CORE_INFO("[{0}] Application Contructed", __FUNCTION__);
	}
	Application::~Application()
	{
		KROSS_PROFILE_FUNC();
		m_LayerStack.~LayerStack();
		Stack<Shader>::instance().clear();
		Stack<Texture::T2D>::instance().clear();
		Renderer::Shutdown();
		m_uptrWindow->Shutdown();
		m_uptrWindow.release();
		KROSS_CORE_INFO("[{0}] Application Destructed", __FUNCTION__);
	}

	void Application::PushLayer(const std::initializer_list<Ref<Layer>>& list)
	{
		KROSS_PROFILE_FUNC();
		for (const Ref<Layer>& l : list) {
			m_LayerStack.PushLayer(l);
			l->OnAttach();
			KROSS_CORE_TRACE("[{1}] Application '{0}' Pushed", l->GetName(), __FUNCSIG__);
		}
	}

	void Application::PushLayer(const Ref<Layer>& layer)
	{
		KROSS_PROFILE_FUNC();
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();
		KROSS_CORE_TRACE("[{1}] Application '{0}' Pushed", layer->GetName(), __FUNCSIG__);
	}

	void Application::PushOverlay(const Ref<Layer>& layer)
	{
		KROSS_PROFILE_FUNC();
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
		KROSS_CORE_TRACE("[{1}] Application Overlay '{0}' Pushed", layer->GetName(), __FUNCTION__);
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
			static Timestep time;
			double ts = time.GetLap();
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

		KROSS_CORE_TRACE("[{0}] Application Finished", __FUNCTION__);
	}

	inline double Application::GetTime() const
	{
		return Timestep::now();
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
			KROSS_CORE_TRACE("[{0}] Application Minimized", __FUNCTION__);
			return false;
		}
		m_bMinimized = false;
		RenderCommand::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}
