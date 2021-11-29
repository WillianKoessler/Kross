#include <Kross_pch.h>
#include "Context.h"
#include "GFXAPI/OpenGL/GLContext.h"
#include "Kross/Renderer/Renderer.h"
#include "Kross/Core/Window.h"

namespace Kross {
	Ref<Context> Context::CreateRef(Window* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KROSS_MSGBOX("[Kross::Context] Invalid Graphics API", "Context", _FATAL_); return nullptr;
			case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Context>(window);
		}
		KROSS_MSGBOX("[Kross::Context] Unknown Graphics API.", "Context", _FATAL_);
		return nullptr;
	}
	Scope<Context> Context::CreateScope(Window* window)
	{
		switch (Renderer::GetAPI())
		{
			case RendererAPI::API::None:		KROSS_MSGBOX("[Kross::Context] Invalid Graphics API", "Context", _FATAL_); return nullptr;
			case RendererAPI::API::OpenGL:		return makeScope<OpenGL::Context>(window);
		}
		KROSS_MSGBOX("[Kross::Context] Unknown Graphics API.", "Context", _FATAL_);
		return nullptr;
	}
}