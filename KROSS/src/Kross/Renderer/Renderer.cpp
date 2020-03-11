#include "Kross_pch.h"
#include "Renderer.h"

#include "GFXAPI/OpenGL/RenderersAPI.h"
//#include "GFXAPI/DirectX/RenderersAPI.h"

namespace Kross {
	Renderer::Scene::Data* Renderer::Scene::m_Data = new Renderer::Scene::Data;
	Ref<Renderer::ShaderLibrary> Renderer::shaderLibrary = makeRef<Renderer::ShaderLibrary>();
	RendererAPI* Renderer::Command::s_RendererAPI = new GraphicsAPI::RendererAPI;

	void Renderer::Init()
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_TRACE("[Kross::Renderer] Renderer Initiated");

		Renderer::Command::Init();
		Renderer2D::Init();
	}

	void Renderer::Shutdown()
	{
		KROSS_PROFILE_FUNC();
		shaderLibrary->Clear();
		Renderer::Command::Shutdown();
		Renderer2D::Shutdown();
		KROSS_CORE_TRACE("[Kross::Renderer] Renderer Finished");
	}

	void Renderer::Scene::Begin(Camera::Camera& camera)
	{
		Scene::m_Data->ViewProjectionMatrix = camera.GetVPM();
	}

	void Renderer::Scene::End()
	{
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4 transform)
	{
		shader->Bind();

		shader->SetMat4("u_ViewProjection", Scene::m_Data->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		va->Bind();
		Renderer::Command::DrawIndexed(va);
	}

	void Renderer::Submit(const Ref<Shader>& shader, const Scope<VertexArray>& va, const glm::mat4 transform)
	{
		shader->Bind();

		shader->SetMat4("u_ViewProjection", Scene::m_Data->ViewProjectionMatrix);
		shader->SetMat4("u_Transform", transform);

		va->Bind();
		Renderer::Command::DrawIndexed(va);
	}

	Ref<Renderer::ShaderLibrary> Renderer::GetShaderLibrary()
	{
		return shaderLibrary;
	}

	void Renderer::ShaderLibrary::Clear()
	{
		shaders.clear();
	}

	void Renderer::ShaderLibrary::Add(const Ref<Shader>& shader)
	{
		const std::string& name = shader->GetName();
		Add(name, shader);
	}

	void Renderer::ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	{
		if (exist(name)) { KROSS_CORE_WARN("[Kross::Renderer::ShaderLibrary] Shader '{0}' already in library. ", name); }
		shaders[name] = shader;
	}

	Ref<Shader> Renderer::ShaderLibrary::Load(const Ref<Shader>& shader)
	{
		Add(shader);
		return shader;
	}

	Ref<Shader> Renderer::ShaderLibrary::Load(const std::string & filepath)
	{
		Ref<Shader> shader = Shader::CreateRef(filepath);
		Add(shader);
		return shader;
	}

	Ref<Shader> Renderer::ShaderLibrary::Load(const std::string & name, const std::string & filepath)
	{
		Ref<Shader> shader = Shader::CreateRef(name, filepath);
		Add(name, shader);
		return shader;
	}

	Ref<Shader> Renderer::ShaderLibrary::Get(const std::string& name)
	{
		if (!exist(name)) { KROSS_CORE_WARN("[Kross::Renderer::ShaderLibrary] Shader '{0}' NOT FOUND in library. ", name); }
		return shaders[name];
	}
}
