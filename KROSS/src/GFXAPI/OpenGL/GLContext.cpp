#include "Kross_pch.h"
#include "GLContext.h"
#include <GLFW/glfw3.h>

namespace Kross::OpenGL {
	bool Context::initialized = false;

	Context::Context(Window* window)
		: m_Window(window)
	{
		KROSS_PROFILE_FUNC();
		if (!m_Window)
		{
			KROSS_FATAL("Window pointer is NULL!");
		}
		KROSS_INFO("OpenGL Context Created");
	}
	Context::~Context()
	{
		glError::flushCache();
		KROSS_INFO("OpenGL Context Destructed");
	}
	void Context::Init(unsigned int x, unsigned int y)
	{
		KROSS_PROFILE_FUNC();
		unsigned int err = false;

		{
			KROSS_PROFILE_SCOPE("Kross::OpenGL::Context::Init glfwMakeContextCurrent");
			glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));
		}
		// Initialize OpenGL loader
		{
			KROSS_PROFILE_SCOPE("Kross::OpenGL::Context::Init OpenGLLoader");
#if defined(KROSS_GLAD)
			err = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
#elif defined(KROSS_GLEW)
			err = glewInit() != GLEW_OK;
#elif defined(KROSS_GL3W)
			err = gl3wInit() != 0;
#endif
		}
		if (err > 1 || err == 0) { KROSS_FATAL("Failed to Load OpenGL!!"); }

		glError::Begin();

		initialized = true;

		KROSS_TRACE("[OPENGL]");

		float version = GetVersion();

		const GLubyte* buffer;
		glCall(buffer = glGetString(GL_VERSION));
		KROSS_TRACE("    Version: {0}", buffer);
		glCall(buffer = glGetString(GL_VENDOR));
		KROSS_TRACE("    Vendor: {0}", buffer);
		glCall(buffer = glGetString(GL_RENDERER));
		KROSS_TRACE("    Renderer: {0}", buffer);

		if (version < 4.4f)
		{
			KROSS_FATAL("KROSS requires OpenGL 4.4 or superior.");
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
			KROSS_TRACE("OpenGL Context Initialized");
	}
	void Context::SwapBuffers()
	{
		KROSS_PROFILE_FUNC();
		glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));
	}
	void Context::UpdateViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height)
	{
		glCall(glViewport(x, y, width, height));
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
			KROSS_FATAL("Failed to get OpenGL Version.\nReason: OpenGL was not initialized.");
			return 0.0f;
		}
	}
}
