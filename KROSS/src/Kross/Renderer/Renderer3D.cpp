#include <Kross_pch.h>
#include "Renderer3D.h"

#include "Kross/Core/Application.h"
#include "Kross/Core/Stack.h"

#include "Renderer.h"
#include "VertexArray.h"
#include "Textures/Array.h"

namespace Kross {
	bool Renderer3D::s_bSceneBegan = false;
	bool Renderer3D::s_bBatch = false;
	bool Renderer3D::s_bRendererInitialized = false;

	struct Vertex
	{
		Vertex() = default;
		Vertex(glm::vec3 p) : pos(p) {}
		Vertex(int x, int y, int z) : pos((float)x, (float)y, (float)z) {}
		Vertex(float x, float y, float z) : pos(x, y, z) {}
		Vertex(const glm::vec3& pos, const glm::vec4& color, const glm::vec2 texCoords = glm::vec2(0.0f)) : pos(pos), color(color), texCoords(texCoords) {}
		glm::vec3 pos = glm::vec3(0.0f); glm::vec4 color = glm::vec4(1.0f); glm::vec2 texCoords = glm::vec2(0.0f);
	};
	struct Cube
	{
		Cube() = default;
		Cube(Vertex* vertices) { for (int i = 0; i < 8; i++) v[i] = vertices[i]; }
		Vertex v[8];
	};

	struct R3DData {
		// Basic 2D Shader
		Ref<Shader> shader;

		//Buffers
		Scope<VertexArray> va;
		Ref<Buffer::Vertex> vb;
		Ref<Buffer::Index> ib;

		//Renderer Status for ImGui window
		Renderer3D::Stats rendererStats = Renderer3D::Stats();
	}; static R3DData* data;

	const Renderer3D::Stats& Renderer3D::getStats()
	{
		return data->rendererStats;
	}
	void Renderer3D::ResetStats()
	{
		data->rendererStats = Stats();
	}
	void Renderer3D::Init()
	{
		KROSS_PROFILE_FUNC();
		if (s_bRendererInitialized) { KROSS_MSGBOX("Renderer3D is already initialized. Cannot call Renderer3D::Init(void) twice. Forget to call Renderer3D::Shutdown(void)?", __FUNCSIG__, _ERROR_); return; }
		s_bRendererInitialized = true;

		data = new R3DData;

		data->va = VertexArray::CreateScope();

		data->vb = Buffer::Vertex::Create(nullptr, sizeof(Vertex) * 8, true);
		data->vb->SetLayout({
			{ Buffer::ShaderDataType::Float3, "a_Position" },
			{ Buffer::ShaderDataType::Float4, "a_Color"},
			{ Buffer::ShaderDataType::Float2, "a_TexCoords"},
			});
		data->va->AddVertex(data->vb);

		uint32_t indices[36] = {
			0, 1, 2, 2, 3, 0, //Top
			0, 3, 4, 4, 5, 0, //Front
			3, 2, 7, 7, 4, 3, //Right
			2, 1, 6, 6, 7, 2, //Back
			1, 0, 5, 5, 6, 1, //Left
			6, 5, 4, 4, 7, 6, //Bottom
		};

		data->ib = Buffer::Index::Create(indices, sizeof(indices));
		data->va->SetIndex(data->ib);

		Stack<Shader>::instance().Add(data->shader = Shader::CreateRef("assets/shaders/OpenGL/Shader3D.glsl"));
		data->shader->SetFloat4("u_Color", { 1,1,1,1 });

		s_bSceneBegan = false;
	}
	void Renderer3D::Shutdown()
	{
		KROSS_PROFILE_FUNC();
		if (s_bRendererInitialized)
		{
			Stack<Shader>::instance().clear();
			delete data;
		}
		else KROSS_MSGBOX("Renderer3D is not initialized. Cannot call Renderer3D::Shutdown(void) while Renderer3D::Init(void) is not called.", __FUNCSIG__, _ERROR_);
	}
	void Renderer3D::Begin(Ref<Camera::Camera>& camera)
	{
		KROSS_PROFILE_FUNC();
		if (!s_bSceneBegan) s_bSceneBegan = true;
		else KROSS_CORE_WARN("Calling Renderer3D::Begin(Camera::Camera&) without calling Renderer3D::End(void). Overriding previous scene!");
		data->shader->Bind();
		data->shader->SetMat4("u_ViewProjection", camera->GetVPM());
		data->shader->SetMat4("u_Transform", glm::mat4(1.0f));
	}
	void Renderer3D::End()
	{
		if (s_bSceneBegan) s_bSceneBegan = false;
		else KROSS_CORE_WARN("Calling Renderer3D::End(void) without calling Renderer3D::Begin(Camera::Camera&). Did you forget to Begin the Scene?");
	}
	void Renderer3D::SwitchShader(const Ref<Shader>& shader)
	{
		if (!shader) { KROSS_CORE_WARN("Shader provided is empty, or not supported."); return; }
		data->shader->unBind();
		data->shader = shader;
		data->shader->Bind();
	}
	void Renderer3D::DrawCube(const CubeParams& params)
	{
		KROSS_PROFILE_FUNC();
		static const glm::mat4 m4(1.0f);
		static glm::mat4 transform(1.0f);
		static Vertex v[1] = { Vertex() };

		for (uint8_t i = 0; i < 1; i++) {
			v[i].pos.x = params.position.x + (params.size.x * (i == 2 || i == 3 || i == 4 || i == 7));
			v[i].pos.y = params.position.y + (params.size.y * (i == 0 || i == 1 || i == 2 || i == 3));
			v[i].pos.z = params.position.z + (params.size.z * (i == 1 || i == 2 || i == 6 || i == 7));

			v[i].color = params.color;

			v[i].texCoords = glm::vec2(1.0f);
		}
		data->vb->upload(v, sizeof(Vertex) * 1);
		Renderer::Command::DrawIndexed(data->va, true);
	}
}

