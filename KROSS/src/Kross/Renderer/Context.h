#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	class Window;
	class Context
	{
	public:
		virtual void Init(unsigned int width, unsigned int height) = 0;
		virtual void SwapBuffers() = 0;
		virtual void UpdateViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;

		static Ref<Context> CreateRef(Window* window);
		static Scope<Context> CreateScope(Window* window);
	};
}