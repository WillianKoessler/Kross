#include "Kross_pch.h"
#include "Renderer.h"

#include "GFXAPI/OpenGL/GLRenderersAPI.h"
//#include "GFXAPI/DirectX/RenderersAPI.h"

#include "Renderer2D.h"
#include "voxelRenderer.h"

namespace Kross {
	KAPI RendererAPI* RenderCommand::s_RendererAPI = new OpenGL::RendererAPI;
	KAPI uint8_t Renderer::s_uDims = 2;

	void Renderer::Init(uint8_t dims)
	{
		KROSS_PROFILE_FUNC();
		s_uDims = dims;

		RenderCommand::Init();
		switch (s_uDims) {
		case Renderer::TWO_D: Renderer2D::Init(); break;
		case Renderer::THREE_D: KROSS_ERROR("3D is not supported yet."); break;
		case Renderer::VOXEL:	VoxelRenderer::Init(); break;
		default: { KROSS_ERROR("Renderer::Dimentions not supported"); return; }
		}

		KROSS_TRACE("Renderer Initiated");
	}

	void Renderer::Shutdown()
	{
		KROSS_PROFILE_FUNC();

		RenderCommand::Shutdown();

		switch (s_uDims) {
		case Renderer::TWO_D: Renderer2D::Shutdown(); break;
		case Renderer::THREE_D: KROSS_ERROR("3D is not supported yet."); break;
		case Renderer::VOXEL:	VoxelRenderer::Shutdown(); break;
		default: { KROSS_ERROR("Renderer::Dimentions not supported"); return; }
		}
		KROSS_TRACE("Renderer Finished");
	}

	//void Renderer::Scene::Begin(Camera::Camera& camera)
	//{
	//	Scene::m_Data->ViewProjectionMatrix = camera.GetVPM();
	//}

	//void Renderer::Scene::End()
	//{
	//}

	//void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4 transform)
	//{
	//	shader->Bind();

	//	shader->SetMat4("u_ViewProjection", Scene::m_Data->ViewProjectionMatrix);
	//	shader->SetMat4("u_Transform", transform);

	//	va->Bind();
	//	Renderer::Command::DrawIndexed(va);
	//}

	//void Renderer::Submit(const Ref<Shader>& shader, const Scope<VertexArray>& va, const glm::mat4 transform)
	//{
	//	shader->Bind();

	//	shader->SetMat4("u_ViewProjection", Scene::m_Data->ViewProjectionMatrix);
	//	shader->SetMat4("u_Transform", transform);

	//	va->Bind();
	//	Renderer::Command::DrawIndexed(va);
	//}

	//Ref<Renderer::ShaderLibrary> Renderer::GetShaderLibrary()
	//{
	//	return shaderLibrary;
	//}

	//void Renderer::ShaderLibrary::Clear()
	//{
	//	shaders.clear();
	//}

	//void Renderer::ShaderLibrary::Add(const Ref<Shader>& shader)
	//{
	//	const std::string& name = shader->GetName();
	//	Add(name, shader);
	//}

	//void Renderer::ShaderLibrary::Add(const std::string& name, const Ref<Shader>& shader)
	//{
	//	if (exist(name)) { KROSS_WARN("Shader '{1}' already in library. ", name); }
	//	shaders[name] = shader;
	//}

	//Ref<Shader> Renderer::ShaderLibrary::Load(const Ref<Shader>& shader)
	//{
	//	Add(shader);
	//	return shader;
	//}

	//Ref<Shader> Renderer::ShaderLibrary::Load(const std::string & filepath)
	//{
	//	Ref<Shader> shader = Shader::CreateRef(filepath);
	//	Add(shader);
	//	return shader;
	//}

	//Ref<Shader> Renderer::ShaderLibrary::Load(const std::string & name, const std::string & filepath)
	//{
	//	Ref<Shader> shader = Shader::CreateRef(name, filepath);
	//	Add(name, shader);
	//	return shader;
	//}

	//Ref<Shader> Renderer::ShaderLibrary::Get(const std::string& name)
	//{
	//	if (!exist(name)) { KROSS_WARN("Shader '{1}' NOT FOUND in library. ", name); }
	//	return shaders[name];
	//}
}
