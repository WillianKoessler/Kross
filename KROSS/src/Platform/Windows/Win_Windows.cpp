#include "Kross_pch.h"
#include "Win_Windows.h"

#include "Kross/Events/ApplicationEvent.h"
#include "Kross/Events/MouseEvent.h"
#include "Kross/Events/KeyEvent.h"

#include "Kross/Renderer/Context.h"
//#include "GFXAPI/DirectX/Context.h"

namespace Kross {

	static bool s_GLFWInitialized = false;

	static void GLFWErrorCallback(int error, const char *description)
	{
		KROSS_ERROR("GLFW Error ({0}): {1}", error, description);
	}

	Win_Windows::Win_Windows(WindowProps &&props)
	{
		KROSS_PROFILE_FUNC();
		Init(std::move(props));
	}

	Win_Windows::~Win_Windows()
	{
		KROSS_PROFILE_FUNC();
		Shutdown();
	}

	void Win_Windows::Init(WindowProps &&props)
	{
		KROSS_PROFILE_FUNC();
		m_Data = props;

		if (!s_GLFWInitialized)
		{
			{
				KROSS_PROFILE_SCOPE("glfwInit");
				int success = glfwInit();
				if (!success) KROSS_FATAL("Could not intialize GLFW!");
			}
			{
				KROSS_PROFILE_SCOPE("glfwSetErrorCallback");
				glfwSetErrorCallback(GLFWErrorCallback);
				s_GLFWInitialized = true;
			}
		}
		m_Monitor = glfwGetPrimaryMonitor();
		auto mode = glfwGetVideoMode(m_Monitor);
		{
			KROSS_PROFILE_SCOPE("glfwCreateWindow");
			if ((props.nWidth == 0 || props.nHeight == 0) && props.fullscreen)
			{
				m_Window = glfwCreateWindow(mode->width, mode->height, props.strTitle, m_Monitor, NULL);
			} else if ((props.nWidth > 0 && props.nHeight > 0) && props.fullscreen)
			{
				m_Window = glfwCreateWindow((int)props.nWidth, (int)props.nHeight, m_Data.strTitle, m_Monitor, NULL);
				glfwSetWindowMonitor(m_Window, m_Monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
			} else if ((props.nWidth > 0 && props.nHeight > 0) && !props.fullscreen)
			{
				m_Window = glfwCreateWindow((int)props.nWidth, (int)props.nHeight, m_Data.strTitle, nullptr, NULL);
			} else
			{
				KROSS_FATAL(
					"Invalid Window Attributes.\n\tTitle: {0}\n\tWidth: {1}\n\tHeight: {2}\n\tFullcreen Mode: {3}\n{4}",
					props.strTitle, props.nWidth, props.nHeight, props.fullscreen ? "ON" : "OFF",
					"(Prompt Log or File Log, for more information.)");
			}

			int x, y;
			glfwGetWindowPos(m_Window, &x, &y);
			m_Data.x = x; m_Data.y = y;
			glfwGetWindowSize(m_Window, &x, &y);
			m_Data.nWidth = x; m_Data.nHeight = y;

			m_WindowHWND = glfwGetWin32Window(m_Window);
		}
		{
			KROSS_PROFILE_SCOPE("makeScope<Kross::Context>");
			m_Context = Context::CreateScope(this);
			m_Context->Init(m_Data.nWidth, m_Data.nHeight);
		}

		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVSync(true);

		// Set GLFW callbacks
		{
			KROSS_PROFILE_SCOPE("EventsCallbacks");
			glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height) {
				WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
				data.nWidth = width;
				data.nHeight = height;
				data.EventCallback(WindowResizeEvent(width, height));
				});
			glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window) {
				WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
				data.EventCallback(WindowCloseEvent());
				});
			glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int keycode, int scancode, int action, int mods) {
				static uint32_t repeats = GLFW_RELEASE;
				WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
				if (action == GLFW_RELEASE) {
					data.EventCallback(KeyReleasedEvent(keycode)); repeats = GLFW_RELEASE;
				} else if (action == GLFW_PRESS) {
					data.EventCallback(KeyPressedEvent(keycode)); repeats = GLFW_PRESS;
				} else if (action == GLFW_REPEAT) data.EventCallback(KeyHeldEvent(keycode, repeats++));
				});
			glfwSetCharCallback(m_Window, [](GLFWwindow *window, unsigned int keycode) {
				WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
				data.EventCallback(KeyTypedEvent(keycode));
				});
			glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods) {
				static uint32_t repeats = GLFW_RELEASE;
				WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
				if (action == GLFW_RELEASE) {
					data.EventCallback(MouseButtonReleasedEvent(button)); repeats = GLFW_RELEASE;
				} else if (action == GLFW_PRESS) {
					data.EventCallback(MouseButtonPressedEvent(button)); repeats = GLFW_PRESS;
				} else if (action == GLFW_REPEAT) data.EventCallback(MouseButtonHeldEvent(button, repeats++));
				});
			glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xOffset, double yOffset) {
				WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
				data.EventCallback(MouseScrolledEvent((float)xOffset, (float)yOffset));
				});
			glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xPos, double yPos) {
				WindowData &data = *(WindowData *)glfwGetWindowUserPointer(window);
				data.EventCallback(MouseMovedEvent((int)xPos, (int)yPos));
				});
		}
		KROSS_INFO("Window Initialized {0} ({1}, {2})", m_Data.strTitle, m_Data.nWidth, m_Data.nHeight);
	}

	void Win_Windows::Shutdown() const
	{
		glfwDestroyWindow(m_Window);
		glfwTerminate();
		KROSS_INFO("Windows Window Destructed.");
	}

	void Win_Windows::OnUpdate()
	{
		glfwPollEvents();
		if (GetWidth() != 0 && GetHeight() != 0)
			m_Context->SwapBuffers();
	}

	bool Win_Windows::FullScreen(bool enable) const
	{
		static bool previous_state = false;
		if (enable == previous_state) return false;
		previous_state = enable;

		auto mode = glfwGetVideoMode(m_Monitor);
		if (enable) glfwSetWindowMonitor(m_Window, m_Monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
		else glfwSetWindowMonitor(m_Window, nullptr, 100, 100, m_Data.nWidth, m_Data.nHeight, mode->refreshRate);
		//glfwGetFramebufferSize(m_Window, nullptr, nullptr);
		//m_Context->UpdateViewport(0, 0, m_Data.nWidth, m_Data.nHeight);
		//KROSS_TRACE("Size = ({0}, {1})", m_Data.nWidth, m_Data.nHeight);
		return enable;
	}

	bool Win_Windows::CursorEnabled(bool active) const
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, (active ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN));
		return active;
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
