#include "Kross_pch.h"
#include "Context.h"
#include <GLFW/glfw3.h>

namespace Kross::OpenGL {
	bool Context::initialized = false;

	Context::Context(Window* window)
		: m_Window(window)
	{
		KROSS_PROFILE_FUNC();
		if (!m_Window)
		{
			KROSS_MSGBOX("Window pointer is NULL!", __FUNCTION__, _FATAL_);
		}
		KROSS_CORE_INFO("[ {0} ] |||| OpenGL Context Created", __FUNCTION__);
	}
	Context::~Context()
	{
		glError::flushCache();
		KROSS_CORE_INFO("[ {0} ] |||| OpenGL Context Destructed", __FUNCTION__);
	}
	void Context::Init(unsigned int x, unsigned int y)
	{
		KROSS_PROFILE_FUNC();
		unsigned int err = false;

		{
			KROSS_PROFILE_SCOPE("glfwMakeContextCurrent");
			glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));
		}
		// Initialize OpenGL loader
		{
			KROSS_PROFILE_SCOPE("OpenGLLoader");
#if defined(KROSS_GLAD)
			err = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#elif defined(KROSS_GLEW)
			err = glewInit() != GLEW_OK;
#elif defined(KROSS_GL3W)
			err = gl3wInit() != 0;
#endif
		}
		if (err > 1 || err == 0) { KROSS_MSGBOX("Failed to Load OpenGL!!", __FUNCTION__, _FATAL_); }

		glError::Begin();

		initialized = true;

		KROSS_CORE_TRACE("[OPENGL]");

		float version = GetVersion();

		const GLubyte* buffer;
		glCall(buffer = glGetString(GL_VERSION));
		KROSS_CORE_TRACE("    Version: {0}", buffer);
		glCall(buffer = glGetString(GL_VENDOR));
		KROSS_CORE_TRACE("    Vendor: {0}", buffer);
		glCall(buffer = glGetString(GL_RENDERER));
		KROSS_CORE_TRACE("    Renderer: {0}", buffer);

		if (version < 4.4f)
		{
			KROSS_MSGBOX("KROSS requires OpenGL 4.4 or superior.", __FUNCTION__, _FATAL_);
		}

#if (KROSS_GL_DEBUG)
#if(KROSS_DEBUG)
			glEnable(GL_DEBUG_OUTPUT);
			glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
#else
#endif
			glDebugMessageCallback(glDebugMessage, nullptr);
#endif

			glViewport(0, 0, x, y);
			KROSS_CORE_TRACE("[ {0} ] |||| OpenGL Context Initialized", __FUNCTION__);
	}
	void Context::SwapBuffers()
	{
		KROSS_PROFILE_FUNC();
		glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));
	}
	const float Context::GetVersion()
	{
		if (initialized)
		{
			static float version = -1.0f;
			if (version == -1.0f)
			{
				int major, minor;
				glCall(glGetIntegerv(GL_MAJOR_VERSION, &major));
				glCall(glGetIntegerv(GL_MAJOR_VERSION, &minor));
				version = (float)major + minor / 10.0f;
			}
			return version;
		}
		else
		{
			KROSS_MSGBOX("Failed to get OpenGL Version.\nReason: OpenGL was not initialized.", __FUNCTION__, _ERROR_);
			return 0.0f;
		}
	}
}
