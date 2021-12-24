#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Window.h"
#include "Kross/Renderer/Context.h"
#include "GLErrors.h"

struct GLFWwindow;

namespace Kross::OpenGL {
	class Context final : public Kross::Context
	{
	public:
		Context(Window* window);
		~Context();

		virtual void Init(uint32_t x, uint32_t y) override;
		virtual void SwapBuffers() override;
		virtual void UpdateViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

//		static const float GetVersion();
	private:
		Window* m_Window;
		static bool initialized;
	};
}
