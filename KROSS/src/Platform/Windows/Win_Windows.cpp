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
		KROSS_CORE_ERROR_("GLFW Error ({0}): {1}", error, description);
	}

	Window* Window::Create(const WindowProps& props)
	{
		KROSS_CORE_INFO("[{0}] Windows window Created.", __FUNCTION__);
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

		if (!s_GLFWInitialized)
		{
			{
				KROSS_PROFILE_SCOPE("glfwInit");
				int success = glfwInit();
				if (!success)
				{
					KROSS_MSGBOX("Could not intialize GLFW!", __FUNCTION__, _FATAL_);
				}
			}
			{
				KROSS_PROFILE_SCOPE("glfwSetErrorCallback");
				glfwSetErrorCallback(GLFWErrorCallback);
				s_GLFWInitialized = true;
			}
		}
		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		{
			KROSS_PROFILE_SCOPE("glfwCreateWindow");
			if ((props.nWidth == 0 || props.nHeight == 0) && props.fullscreen)
			{
				m_Window = glfwCreateWindow(mode->width, mode->height, props.strTitle.c_str(), monitor, NULL);
			}
			else if ((props.nWidth > 0 && props.nHeight > 0) && props.fullscreen)
			{
				m_Window = glfwCreateWindow((int)props.nWidth, (int)props.nHeight, m_Data.strTitle.c_str(), monitor, NULL);
				glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			}
			else if ((props.nWidth > 0 && props.nHeight > 0) && !props.fullscreen)
			{
				m_Window = glfwCreateWindow((int)props.nWidth, (int)props.nHeight, m_Data.strTitle.c_str(), NULL, NULL);
			}
			else
			{
				KROSS_CORE_ERROR(
					"[{0}] Invalid Window Attributes.\nTitle: {1}\nWidth: {2}\nHeight: {3}\nFullcreen Mode: {4}",
					__FUNCTION__, props.strTitle, props.nWidth, props.nHeight, props.fullscreen ? "ON" : "OFF"
				);
				KROSS_MSGBOX("Invalid Window Attributes.\n(Prompt Log or File Log, for more information.", __FUNCTION__, _FATAL_);
			}

			int x, y;
			glfwGetWindowPos(m_Window, &x, &y);
			m_Data.x = x; m_Data.y = y;
			glfwGetWindowSize(m_Window, &x, &y);
			m_Data.nWidth = x; m_Data.nHeight = y;

			m_WindowHWND = glfwGetWin32Window(m_Window);
		}
		{
			KROSS_PROFILE_SCOPE("makeScope<OpenGL::Context>");
			//int x, y;
			//glfwGetWindowSize(m_Window, &x, &y);

			m_Context = makeRef<GraphicsAPI::Context>(this);
			m_Context->Init(m_Data.nWidth, m_Data.nHeight);
		}

		{
			KROSS_PROFILE_SCOPE("glfwSetWindowUserPointer");
			glfwSetWindowUserPointer(m_Window, &m_Data);
		}
		{
			KROSS_PROFILE_SCOPE("SetVSync");
			SetVSync(true);
		}

		// Set GLFW callbacks
		{
			KROSS_PROFILE_SCOPE("glfwSetWindowSizeCallback");
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
			KROSS_PROFILE_SCOPE("glfwSetWindowCloseCallback");
			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});
		}
		{
			KROSS_PROFILE_SCOPE("glfwSetKeyCallback");
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
			KROSS_PROFILE_SCOPE("glfwSetCharCallback");
			glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				KeyTypedEvent event(keycode);
				data.EventCallback(event);
			});
		}
		{
			KROSS_PROFILE_SCOPE("glfwSetMouseButtonCallback");
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
			KROSS_PROFILE_SCOPE("glfwSetScrollCallback");
			glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseScrolledEvent event((float)xOffset, (float)yOffset);
				data.EventCallback(event);
			});
		}
		{
			KROSS_PROFILE_SCOPE("glfwSetCursorPosCallback");
			glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

				MouseMovedEvent event((int)xPos, (int)yPos);
				data.EventCallback(event);
			});
		}

		KROSS_CORE_INFO("[{0}] Window Initialized {1} ({2}, {3})", __FUNCTION__, m_Data.strTitle, m_Data.nWidth, m_Data.nHeight);
	}

	void Win_Windows::Shutdown() const
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		KROSS_CORE_INFO("[{0}] Windows window Destructed.", __FUNCTION__);
	}

	void Win_Windows::OnUpdate()
	{
		glfwPollEvents();
		if (GetWidth() != 0 && GetHeight() != 0) m_Context->SwapBuffers();
	}

	void Win_Windows::FullScreen(bool enable) const
	{
		auto monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(monitor);
		if (enable);//glfwSetWindowMonitor(m_Window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		else glfwSetWindowMonitor(m_Window, monitor, m_Data.x, m_Data.y, (int)m_Data.nWidth, (int)m_Data.nHeight, mode->refreshRate);
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