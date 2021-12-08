#include <Kross_pch.h>
#include "Context.h"

#define GLFW_EXPOSE_NATIVE_WIN32 1
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


namespace Kross::DirectX {
	Context::Context(Window* window)
		: m_Window(window)
	{
		KROSS_PROFILE_FUNC();
		if (!m_Window)
		{
			KROSS_MSGBOX("Window pointer is NULL!", "[Kross::DirectX::Context]", _FATAL_);
		}
		KROSS_INFO("[Kross::DirectX::Context] DirectX Context Created(?)");
	}

	Context::~Context()
	{
		KROSS_INFO("[Kross::DirectX::Context] DirectX Context Destructed(?)");
	}
	
	void Context::Init()
	{
		KROSS_PROFILE_FUNC();

		HWND wnd = glfwGetWin32Window(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));

		DXGI_SWAP_CHAIN_DESC sd = {};
		sd.BufferDesc.Width = m_Window->GetWidth();
		sd.BufferDesc.Height = m_Window->GetHeight();
		sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
		sd.BufferDesc.RefreshRate.Numerator = 1;
		sd.BufferDesc.RefreshRate.Denominator = 60;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		sd.OutputWindow = wnd;
		sd.Windowed = true;

		D3D_FEATURE_LEVEL  FeatureLevelsRequested = D3D_FEATURE_LEVEL_11_0;
		unsigned int numLevelsRequested = 1u;
		D3D_FEATURE_LEVEL  FeatureLevelsSupported;

		const D3D_FEATURE_LEVEL lvl[] = {
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
			D3D_FEATURE_LEVEL_9_3,
			D3D_FEATURE_LEVEL_9_2,
			D3D_FEATURE_LEVEL_9_1
		};
		HRESULT HR;
		unsigned int createFlags = 0u;

#if KROSS_DEBUG
		createFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		//hr = D3D11CreateDevice(
		//	nullptr,                    // Specify nullptr to use the default adapter.
		//	D3D_DRIVER_TYPE_HARDWARE,   // Create a device using the hardware graphics driver.
		//	0,                          // Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
		//	deviceFlags,                // Set debug and Direct2D compatibility flags.
		//	levels,                     // List of feature levels this app can support.
		//	ARRAYSIZE(levels),          // Size of the list above.
		//	D3D11_SDK_VERSION,          // Always set this to D3D11_SDK_VERSION for Windows Store apps.
		//	&device,                    // Returns the Direct3D device created.
		//	&m_featureLevel,            // Returns feature level of device created.
		//	&context                    // Returns the device immediate context.
		//);
		{
			KROSS_PROFILE_SCOPE("Kross::DirectX::Context::Init D3D11CreateDeviceAndSwapChain");
			// create device and front/back buffers
			HR = D3D11CreateDeviceAndSwapChain(
				nullptr,							// Specify nullptr to use the default adapter.
				D3D_DRIVER_TYPE_HARDWARE,			// Create a device using the hardware graphics driver.
				0,									// Should be 0 unless the driver is D3D_DRIVER_TYPE_SOFTWARE.
				createFlags,						// Set debug and Direct2D compatibility flags.
				lvl,								// List of feature levels this app can support.
				_countof(lvl),						// Size of the list above.
				D3D11_SDK_VERSION,					// Always set this to D3D11_SDK_VERSION for Windows Store apps.
				&sd,								// Returns the Swap Chain Description.
				&m_SwapChain,						// Returns the SwapChain created.
				&m_Device,							// Returns the Direct3D device created.
				&FeatureLevelsSupported,			// Returns feature level of device created.
				&m_Context);						// Returns the device immediate context.
		}

		if (HR == E_INVALIDARG)
		{
			KROSS_PROFILE_SCOPE("Kross::DirectX::Context::Init D3D11CreateDeviceAndSwapChain >>>E_INVALIDARG<<<");
			HR = D3D11CreateDeviceAndSwapChain(
				nullptr, 
				D3D_DRIVER_TYPE_HARDWARE, 
				nullptr, 
				createFlags, 
				&lvl[1], 
				_countof(lvl) - 1, 
				D3D11_SDK_VERSION, 
				&sd, 
				&m_SwapChain,
				&m_Device,
				&FeatureLevelsSupported,
				&m_Context);
		} else
		{
			KROSS_MSGBOX("Failed to create device and/or swap chain", "[Kross::DirectX::Context]", _FATAL_);
		}

		// get handle to backbuffer
		Microsoft::WRL::ComPtr<ID3D11Resource> BackBuffer;
		{
			KROSS_PROFILE_SCOPE("Kross::DirectX::Context::Init GetBackBuffer");
			if (FAILED(HR = m_SwapChain->GetBuffer(
				0,
				__uuidof(ID3D11Texture2D),
				(LPVOID*)&BackBuffer)))
			{
				KROSS_MSGBOX("Failed to get back buffer", "[Kross::DirectX::Context]", _FATAL_);
			}
		}
		{
			KROSS_PROFILE_SCOPE("Kross::DirectX::Context::Init CreateRenderTargetView");
			// create a view on backbuffer that we can render to
			if (FAILED(HR = m_Device->CreateRenderTargetView(
				BackBuffer.Get(),
				nullptr,
				&m_RenderTargetView)))
			{
				KROSS_MSGBOX("Failed to create render target view on backbuffer", "[Kross::DirectX::Context]", _FATAL_);
			}
		}
		{
			KROSS_PROFILE_SCOPE("Kross::DirectX::Context::Init OMSetRenderTargets");
			// set backbuffer as the render target using created view
			m_Context->OMSetRenderTargets(1, m_RenderTargetView.GetAddressOf(), nullptr);
		}
		{
			KROSS_PROFILE_SCOPE("Kross::DirectX::Context::Init RSSetViewports");
			// set viewport dimensions
			D3D11_VIEWPORT vp;
			vp.Width = float(m_Window->GetWidth());
			vp.Height = float(m_Window->GetHeight());
			vp.MinDepth = 0.0f;
			vp.MaxDepth = 1.0f;
			vp.TopLeftX = 0.0f;
			vp.TopLeftY = 0.0f;
			m_Context->RSSetViewports(1, &vp);
		}
		
	}

	void Context::SwapBuffers()
	{
		KROSS_PROFILE_FUNC();
	}
	Context& Context::Get(Window* window)
	{
		if (!instance)
			instance = new Context(window);
		return *instance;
	}
	Context& Context::Get()
	{
		return *instance;
	}
}