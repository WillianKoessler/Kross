#pragma once

#include "Core.h"
#include "Kross/Events/Event.h"

namespace Kross {
	using EventCallbackFn = std::function<void(Event&)>;

	struct WindowProps
	{
		std::string strTitle;
		unsigned int nWidth, nHeight, x, y;
		bool fullscreen;
		bool bVSync;
		EventCallbackFn EventCallback;

		WindowProps(const std::string& title = "Kross Engine",
			unsigned int width = 380,
			unsigned int height = 240,
			bool fullscreen = false,
			bool vsync = true)
			:
			strTitle(title),
			nWidth(width),
			nHeight(height),
			x(0),
			y(0),
			fullscreen(fullscreen),
			bVSync(vsync)
		{
		}

		void operator =(const WindowProps& other)
		{
			strTitle = other.strTitle;
			nWidth = other.nWidth;
			nHeight = other.nHeight;
			bVSync = other.bVSync;
			EventCallback = other.EventCallback;
		}
	};

	// Interface representing a desktop system based on Windows
	class KAPI Window
	{
	public:
		virtual ~Window() {};

		virtual void OnUpdate(bool) = 0;

		virtual unsigned int GetWidth() const = 0;
		virtual unsigned int GetHeight() const = 0;

		// Window attributes
		virtual bool FullScreen(bool) const = 0;
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual void Shutdown() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};
}
