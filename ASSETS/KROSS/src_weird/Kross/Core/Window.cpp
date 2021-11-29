#include "Kross_pch.h"
#include "Window.h"

#ifdef KROSS_PLATFORM_WINDOWS
#include "Platform/Windows/Win_Windows.h"
#else
#error Only Windows is supported
#endif


namespace Kross {
	Window* Window::Create(WindowProps&& props)
	{
#ifdef KROSS_PLATFORM_WINDOWS
		return new Win_Windows(std::move(props));
#else
#error Only Windows is supported
#endif
	}
}