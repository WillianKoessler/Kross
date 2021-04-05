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

		virtual void Init(unsigned int, unsigned int) override;
		virtual void SwapBuffers() override;
		virtual void UpdateViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;

		static const float GetVersion();
	private:
		Window* m_Window;
		static bool initialized;
	};
}
