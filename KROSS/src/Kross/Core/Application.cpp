#include <Kross_pch.h>
#include "Application.h"

#include "Kross/Core/Timestep.h"
#include "Kross/Renderer/Renderer.h"
#include "Kross/Renderer/Shaders.h"

#include "Stack.h"
#include "Kross/Renderer/Textures/Textures.h"
#include "Kross/Core/Resource.h"

namespace Kross {

	Application* Application::s_Instance = nullptr;

	void Application::Construct(settings&& s)
	{
		KROSS_PROFILE_FUNC();
		if (s_Instance) KROSS_FATAL("Application already exists!");
		s_Instance = this;

		m_pWindow = Window::Create({ s.title, s.width, s.height, s.fullscreen });
		m_pWindow->SetVSync(true);
		m_pWindow->SetEventCallback(KROSS_BIND_EVENT_FN(Application::OnEvent));
		Renderer::Init(s.dims);

		PushOverlay(m_ImGuiLayer = makeRef<ImGuiLayer>());

		KROSS_INFO("Application Contructed");
	}
	Application::~Application()
	{
		KROSS_PROFILE_FUNC();
		Stack<Shader>::clear();
		Stack<Texture::T2D>::clear();
		m_pLayerStack.clear();
		Renderer::Shutdown();
		delete m_pWindow;
		KROSS_INFO("Application Destructed");
	}

	void Application::PushLayer(const std::initializer_list<Ref<Layer>>& list)
	{
		KROSS_PROFILE_FUNC();
		for (const Ref<Layer>& l : list) {
			m_pLayerStack.PushLayer(l);
			l->OnAttach();
			KROSS_TRACE("Application '{0}' Pushed", l->GetName());
		}
	}

	void Application::PushLayer(const Ref<Layer>& layer)
	{
		KROSS_PROFILE_FUNC();
		m_pLayerStack.PushLayer(layer);
		layer->OnAttach();
		KROSS_TRACE("Application '{0}' Pushed", layer->GetName());
	}

	void Application::PushOverlay(const Ref<Layer>& layer)
	{
		KROSS_PROFILE_FUNC();
		m_pLayerStack.PushOverlay(layer);
		layer->OnAttach();
		KROSS_TRACE("Application Overlay '{0}' Pushed", layer->GetName());
	}

	void Application::OnEvent(Event& e)
	{
		KROSS_PROFILE_FUNC();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(KROSS_BIND_EVENT_FN(Application::OnWindowClose));
		dispatcher.Dispatch<WindowResizeEvent>(KROSS_BIND_EVENT_FN(Application::OnWindowResize));
		m_pLayerStack.PropagateEvent(e);
	}

	void Application::Run()
	{
		KROSS_PROFILE_FUNC();
		KROSS_TRACE("-----------------------RUNTIME STARTED-----------------------");
		while (m_bRunning)
		{
			KROSS_PROFILE_SCOPE("Run Loop");
			static Timestep time;
			static double ts = 0.0;
			ts = time.GetLap();
			if (!m_bMinimized) {
				KROSS_PROFILE_SCOPE("LayerStack OnUpdate");
				for(auto& layer : m_pLayerStack) layer->OnUpdate(ts);
			}

			m_ImGuiLayer->Begin();
			if(!m_bMinimized) {
				KROSS_PROFILE_SCOPE("LayerStack ImGuiRender");
				for (auto &layer : m_pLayerStack) {
					layer->OnImGuiRender(ts);
				}
			}
			m_ImGuiLayer->End();

			m_pWindow->OnUpdate();
		}
		KROSS_TRACE("-----------------------RUNTIME ENDED-----------------------");

		KROSS_TRACE("Application Finished");
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
			KROSS_TRACE("Application Minimized");
			return false;
		}
		m_bMinimized = false;
		RenderCommand::OnWindowResize(e.GetWidth(), e.GetHeight());
		return false;
	}
}
