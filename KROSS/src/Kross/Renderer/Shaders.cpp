#include "Kross_pch.h"
#include "Shaders.h"
#include "Renderer.h"

#include "GFXAPI/OpenGL/GLShader.h"
//#include "GFXAPI/DirectX/Shader.h"

namespace Kross {

	Ref<Shader> Shader::CreateRef(const char* name, const char* vertexSource, const char* fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Shader>(name, vertexSource, fragmentSource);
		//case RendererAPI::API::DirectX:		return makeRef<DirectX::Shader>(name);
		}

		KROSS_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Ref<Shader> Shader::CreateRef(const char* name, const std::initializer_list<const char *>& sources)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Shader>(name, sources);
			//case RendererAPI::API::DirectX:		return makeRef<DirectX::Shader>(name);
		}

		KROSS_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Ref<Shader> Shader::CreateRef(const char* name, const char* filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Shader>(name, filepath);
		//case RendererAPI::API::DirectX:		return makeRef<DirectX::Shader>(name);
		}

		KROSS_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Ref<Shader> Shader::CreateRef(const char* filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeRef<OpenGL::Shader>(filepath);
		//case RendererAPI::API::DirectX: {KROSS_ERROR("[Kross::Shader] Shader Constructor for DirectX Shader not available"); return nullptr; }
		}

		KROSS_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Scope<Shader> Shader::CreateScope(const char* name, const char* vertexSource, const char* fragmentSource)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::Shader>(name, vertexSource, fragmentSource);
		//case RendererAPI::API::DirectX:		return makeScope<DirectX::Shader>(name);
		}

		KROSS_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Scope<Shader> Shader::CreateScope(const char* name, const std::initializer_list<const char *>& sources)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::Shader>(name, sources);
			//case RendererAPI::API::DirectX:		return makeScope<DirectX::Shader>(name);
		}

		KROSS_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Scope<Shader> Shader::CreateScope(const char* name, const char* filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::Shader>(name, filepath);
		//case RendererAPI::API::DirectX:		return makeScope<DirectX::Shader>(name);
		}

		KROSS_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}

	Scope<Shader> Shader::CreateScope(const char* filepath)
	{
		switch (Renderer::GetAPI())
		{
		case RendererAPI::API::None: { KROSS_ERROR("Renderer API (None) is not supported"); return nullptr; }
		case RendererAPI::API::OpenGL:		return makeScope<OpenGL::Shader>(filepath);
		//case RendererAPI::API::DirectX: {KROSS_ERROR("[Kross::Shader] Shader Constructor for DirectX Shader not available"); return nullptr; }
		}

		KROSS_ERROR("Renderer API (None) is not supported");
		return nullptr;
	}
}
