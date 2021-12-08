#pragma once

#include "Kross/Core/Window.h"
#include "Kross/Renderer/Context.h"

#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32 1
#include <GLFW/glfw3native.h>

namespace Kross {

	class Win_Windows : public Window
	{
	public:
		Win_Windows(WindowProps&& props);
		virtual ~Win_Windows() override;

		virtual void OnUpdate() override;

		virtual inline uint32_t GetWidth() const override { return m_Data.nWidth; }
		virtual inline uint32_t GetHeight() const override { return m_Data.nHeight; }
		virtual inline const char* GetName() const override { return m_Data.strTitle; }

		// Window attributes
		virtual bool FullScreen(bool active) const override;
		virtual bool CursorEnabled(bool active) const override;
		virtual inline void SetEventCallback(const EventCallbackFn& callback) override
		{
			KROSS_TRACE("Callback set");
			m_Data.EventCallback = callback;
		}
		inline virtual void* GetNativeWindow() const override { return m_Window; }
		inline HWND GetWindowHandle() const { return m_WindowHWND; }
		inline const Scope<Context>& GetContext() const { return m_Context; }
		virtual void SetVSync(bool enable) override;
		virtual bool IsVSync() const override;
	private:
		virtual void Init(WindowProps&& props);
		virtual void Shutdown() const override;
	private:
		HWND m_WindowHWND;
		GLFWwindow* m_Window;
		GLFWmonitor* m_Monitor;
		Scope<Context> m_Context;

		using WindowData = WindowProps;
		WindowProps m_Data;
	};
}