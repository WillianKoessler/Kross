#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Window.h"
#include "Kross/Renderer/Context.h"
#include "GLErrors.h"

struct GLFWwindow;

namespace Kross::OpenGL {
	class KAPI Context final : public Kross::Context
	{
	public:
		Context(Window* window);
		~Context();

		virtual void Init() override;
		virtual void SwapBuffers() override;

		static const float GetVersion();
	private:
		Window* m_Window;
		static bool initialized;
	};
}
