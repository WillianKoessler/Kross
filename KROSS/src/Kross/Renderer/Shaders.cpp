#include "Kross_pch.h"
#include "Shaders.h"
#include "Renderer.h"

#include "GFXAPI/OpenGL/Shader.h"
//#include "GFXAPI/DirectX/Shader.h"

namespace Kross {

	Ref<Shader> Shader::CreateRef(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_CORE_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Shader>(name, vertexSource, fragmentSource);
		//case RendererAPI::API::DirectX:		return makeRef<DirectX::Shader>(name);
		}

		KROSS_CORE_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Ref<Shader> Shader::CreateRef(const std::string& name, const std::initializer_list<std::string>& sources)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_CORE_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Shader>(name, sources);
			//case RendererAPI::API::DirectX:		return makeRef<DirectX::Shader>(name);
		}

		KROSS_CORE_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Ref<Shader> Shader::CreateRef(const std::string& name, const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_CORE_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Shader>(name, filepath);
		//case RendererAPI::API::DirectX:		return makeRef<DirectX::Shader>(name);
		}

		KROSS_CORE_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Ref<Shader> Shader::CreateRef(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_CORE_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Shader>(filepath);
		//case RendererAPI::API::DirectX: {KROSS_CORE_ERROR("[Kross::Shader] Shader Constructor for DirectX Shader not available"); return nullptr; }
		}

		KROSS_CORE_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Scope<Shader> Shader::CreateScope(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_CORE_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::Shader>(name, vertexSource, fragmentSource);
		//case RendererAPI::API::DirectX:		return makeScope<DirectX::Shader>(name);
		}

		KROSS_CORE_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Scope<Shader> Shader::CreateScope(const std::string& name, const std::initializer_list<std::string>& sources)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_CORE_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::Shader>(name, sources);
			//case RendererAPI::API::DirectX:		return makeScope<DirectX::Shader>(name);
		}

		KROSS_CORE_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Scope<Shader> Shader::CreateScope(const std::string& name, const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_CORE_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::Shader>(name, filepath);
		//case RendererAPI::API::DirectX:		return makeScope<DirectX::Shader>(name);
		}

		KROSS_CORE_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Scope<Shader> Shader::CreateScope(const std::string& filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_CORE_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::Shader>(filepath);
		//case RendererAPI::API::DirectX: {KROSS_CORE_ERROR("[Kross::Shader] Shader Constructor for DirectX Shader not available"); return nullptr; }
		}

		KROSS_CORE_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}
}
