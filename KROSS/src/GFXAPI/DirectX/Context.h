#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Context.h"
#include "Kross/Core/Window.h"
#include "DXBasics.h"

namespace Kross::DirectX {
	class KAPI Context: public Kross::Context
	{
		Context(Window* window);
	public:
		~Context();

		virtual void Init() override;
		virtual void SwapBuffers() override;

		inline Microsoft::WRL::ComPtr<ID3D11Device> GetDevice() const { return m_Device; }
		inline Microsoft::WRL::ComPtr<ID3D11DeviceContext> GetContext() const { return m_Context; }
		inline Microsoft::WRL::ComPtr<IDXGISwapChain> GetSwapChain() const { return m_SwapChain; }

		inline Window* GetWindow() const { return m_Window; }

		static Context& Get(Window* window);
		static Context& Get();
	private:
		static Context* instance;
		Window* m_Window;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			m_SwapChain;
		Microsoft::WRL::ComPtr<ID3D11Device>			m_Device;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		m_Context;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	m_RenderTargetView;
	};
}
