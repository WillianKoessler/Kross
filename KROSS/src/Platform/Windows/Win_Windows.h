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
		Win_Windows(const WindowProps& props);
		virtual ~Win_Windows() override;

		virtual void OnUpdate() override;

		virtual inline unsigned int GetWidth() const override { return m_Data.nWidth; }
		virtual inline unsigned int GetHeight() const override { return m_Data.nHeight; }

		// Window attributes
		virtual inline void SetEventCallback(const EventCallbackFn& callback) override
		{
			KROSS_CORE_TRACE("[Kross::Win_Windows] Callback set");
			m_Data.EventCallback = callback;
		}
		inline virtual void* GetNativeWindow() const override { return m_Window; }
		inline HWND GetWindowHandle() const { return m_WindowHWND; }
		inline Ref<Context> GetContext() const { return m_Context; }
		virtual void SetVSync(bool enable) override;
		virtual bool IsVSync() const override;
	private:
		virtual void Init(const WindowProps& props);
		virtual void Shutdown() const override;
	private:
		HWND m_WindowHWND;
		GLFWwindow* m_Window;
		Ref<Context> m_Context;

		struct WindowData
		{
			std::string strTitle;
			unsigned int nWidth;
			unsigned int nHeight;

			bool bVSync;
			EventCallbackFn EventCallback;
		};

		WindowProps m_Data;
	};
}