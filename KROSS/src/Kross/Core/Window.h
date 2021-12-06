#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Events/Event.h"

namespace Kross {
	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowProps
	{
		const char* strTitle = "Kross Engine";
		uint32_t nWidth = 380, nHeight = 240, x = 0, y = 0;
		bool fullscreen = false;
		bool bVSync = true;
		EventCallbackFn EventCallback;
	};

	// Interface representing a desktop system based on Windows
	class KAPI Window
	{
	public:
		virtual ~Window() {};

		virtual void OnUpdate() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual const char* GetName() const = 0;

		// Window attributes
		virtual bool FullScreen(bool) const = 0;
		virtual bool CursorEnabled(bool) const = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void Shutdown() const = 0;

		static Window* Create(WindowProps&& props);
	};
}
