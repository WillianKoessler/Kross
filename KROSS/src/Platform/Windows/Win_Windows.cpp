#include "Kross_pch.h"
#include "Win_Windows.h"

#include "Kross/Events/ApplicationEvent.h"
#include "Kross/Events/MouseEvent.h"
#include "Kross/Events/KeyEvent.h"

#include "GFXAPI/OpenGL/Context.h"
#include "GFXAPI/DirectX/Context.h"

namespace Kross {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char* description)
	{
		KROSS_CORE_ERROR("GLFW Error ({0}): {1}\nFILE: {2}\nLINE: {3}", error, description, __FILE__, __LINE__);
	}

	Window* Window::Create(const WindowProps& props)
	{
		KROSS_CORE_INFO("[Kross::Window] Windows window Created.");
		return new Win_Windows(props);
	}

	Win_Windows::Win_Windows(const WindowProps& props)
	{
		KROSS_PROFILE_FUNC();
		Init(props);
	}

	Win_Windows::~Win_Windows()
	{
		KROSS_PROFILE_FUNC();
		Shutdown();
		glfwTerminate();
	}

	void Win_Windows::Init(const WindowProps& props)
	{
		KROSS_PROFILE_FUNC();
		m_Data = props;

		KROSS_CORE_INFO("[Kross::Win_Windows] Creating window {0} ({1}, {2})", props.strTitle, props.nWidth, props.nHeight);

		if (!s_GLFWInitialized)
		{
			{
				KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwInit");
				// TODO: glfwTerminate on system shutdown
				int success = glfwInit();
				if (!success)
				{
					KROSS_MSGBOX("Could not intialize GLFW!", "[Kross::Win_Windows]", _FATAL_);
				}
			}
			{
				KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwSetErrorCallback");
				glfwSetErrorCallback(GLFWErrorCallback);
				s_GLFWInitialized = true;
			}
		}
		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwCreateWindow");
			m_Window = glfwCreateWindow((int)props.nWidth, (int)props.nHeight, m_Data.strTitle.c_str(), nullptr, nullptr);
			m_WindowHWND = glfwGetWin32Window(m_Window);
		}
		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init makeScope<OpenGL::Context>");
			m_Context = makeRef<GraphicsAPI::Context>(this);
		}

		m_Context->Init();


		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwSetWindowUserPointer");
			glfwSetWindowUserPointer(m_Window, &m_Data);
		}
		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init SetVSync");
			SetVSync(true);
		}

		// Set GLFW callbacks
		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwSetWindowSizeCallback");
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				data.nWidth = width;
				data.nHeight = height;

				WindowResizeEvent event(width, height);

				data.EventCallback(event);
			});
		}
		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwSetWindowCloseCallback");
			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});
		}
		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwSetKeyCallback");
			glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);
					break;
				}
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);
					break;
				}
				}
			});
		}
		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwSetCharCallback");
			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});
		}
		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwSetMouseButtonCallback");
			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);
					break;
				}
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);
					break;
				}
				}
			});
		}
		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwSetScrollCallback");
			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});
		}
		{
			KROSS_PROFILE_SCOPE("Kross::Win_Windows::Init glfwSetCursorPosCallback");
			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((int)xPos, (int)yPos);
				data.EventCallback(event);
			});
		}
	}

	void Win_Windows::Shutdown() const
	{
		glfwDestroyWindow(m_Window);
		KROSS_CORE_INFO("[Kross::Win_Windows] Windows window Destructed.");
	}

	void Win_Windows::OnUpdate()
	{
		glfwPollEvents();
		if (GetWidth() != 0 && GetHeight() != 0) m_Context->SwapBuffers();
	}

	void Win_Windows::SetVSync(bool enable)
	{
		KROSS_PROFILE_FUNC();
		glfwSwapInterval(enable);
		m_Data.bVSync = enable;
	}

	bool Win_Windows::IsVSync() const
	{
		return m_Data.bVSync;
	}

}