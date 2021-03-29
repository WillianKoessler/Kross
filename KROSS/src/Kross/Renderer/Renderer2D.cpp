#include <Kross_pch.h>
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shaders.h"
#include "Renderer.h"
#include "Kross/Core/Application.h"
#include "Kross/Core/Stack.h"

#include "Textures/Array.h"
#include "Textures/Atlas.h"

static constexpr size_t MaxQuadCount = 2048;
static constexpr size_t MaxVertexCount = MaxQuadCount * 4;
static constexpr size_t MaxIndexCount = MaxQuadCount * 6;

namespace Kross {
	bool Renderer2D::SceneBegan = false;
	bool Renderer2D::batch = false;

	struct Vertex
	{
		Vertex() = default;
		Vertex(float x, float y) :
			pos(x, y, 0.0f) {}
		Vertex(const glm::vec3& pos, const glm::vec4& color, const glm::vec2& texCoord, float texIndex) :
			pos(pos), color(color), texCoord(texCoord), texIndex(texIndex) {}
		glm::vec3 pos;
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::vec2 texCoord = glm::vec2(0.0f, 0.0f);
		float texIndex = 0.0f;
	};
	struct Quad
	{
		Quad() = default;
		Quad(Vertex* vertices) { for (int i = 0; i < 4; i++) v[i] = vertices[i]; }
		Vertex v[4];
	};

	struct R2DData {
		// Basic 2D Shader
		Ref<Shader> shader;

		// 1x1 White Texture
		Ref<Texture::T2D> whiteTex;

		//Batch data
		Scope<VertexArray> va;
		Ref<Buffer::Vertex> vb;
		Ref<Buffer::Index> ib;

		//Batch Buffer
		Quad myBuffer[MaxQuadCount] = { Quad() };

		//Batch Quad Counter
		unsigned int quadIndex = 0;

		//Texture Array
		Scope<Texture::T2DArray> texArray;

		//Renderer Status for ImGui window
		Renderer2D::Stats rendererStats = Renderer2D::Stats();

		//Quad local vertex positions (rotation)
		Quad rotations;
	};
	static R2DData* data;
	const Renderer2D::Stats& Renderer2D::getStats()
	{
		return data->rendererStats;
	}
	void Renderer2D::ResetStats()
	{
		data->rendererStats = Stats();
	}
	void Renderer2D::Init()
	{
		KROSS_PROFILE_FUNC();
		static bool called = false;
		if (!called)
		{
			data = new R2DData;
			data->texArray = Texture::T2DArray::CreateScope(Texture::Base::QueryMaxSlots());

			data->va = VertexArray::CreateScope();

			data->vb = Buffer::Vertex::Create(nullptr, sizeof(Vertex) * MaxVertexCount, true);
			data->vb->SetLayout({
				{ Buffer::ShaderDataType::Float3, "a_Position" },
				{ Buffer::ShaderDataType::Float4, "a_Color"},
				{ Buffer::ShaderDataType::Float2, "a_TexCoord" },
				{ Buffer::ShaderDataType::Float,  "a_TexIndex"}
								});
			data->va->AddVertex(data->vb);

			uint32_t indices[MaxIndexCount];
			uint32_t offset = 0;
			for (int i = 0; i < MaxIndexCount; i += 6)
			{
				indices[i + 0] = 0 + offset;
				indices[i + 1] = 1 + offset;
				indices[i + 2] = 2 + offset;

				indices[i + 3] = 2 + offset;
				indices[i + 4] = 3 + offset;
				indices[i + 5] = 0 + offset;

				offset += 4;
			}

			data->ib = Buffer::Index::Create(indices, sizeof(indices));
			data->va->SetIndex(data->ib);
			Vertex v[4] = {
				Vertex(-0.5f, -0.5f),
				Vertex(0.5f, -0.5f),
				Vertex(0.5f,  0.5f),
				Vertex(-0.5f,  0.5f)
			};
			data->rotations = Quad(v);

			Stack<Shader>::instance().Add(data->shader = Shader::CreateRef("assets/shaders/OpenGL/Shader2D.glsl"));
			data->shader->SetFloat("u_Repeat", 1);
			data->shader->SetFloat4("u_Color", { 1,1,1,1 });

			data->shader->SetIntV("u_Textures", Texture::Base::QueryMaxSlots(), nullptr);

			unsigned char* white = new unsigned char[4];
			white[0] = 255;
			white[1] = 255;
			white[2] = 255;
			white[3] = 255;

			Stack<Texture::T2D>::instance().Add(data->whiteTex = Texture::T2D::CreateRef(1, 1, "blank", white));
			data->texArray->Add(Stack<Texture::T2D>::instance().Get("blank"));

			SceneBegan = false;
			called = true;
		}
		else
		{
			KROSS_MSGBOX("Renderer2D is already initialized. Cannot call Renderer2D::Init(void) twice. Forget to call Renderer2D::Shutdown(void)?", __FUNCSIG__, _ERROR_);
		}
	}
	void Renderer2D::Shutdown()
	{
		KROSS_PROFILE_FUNC();
		static bool called = false;
		if (!called)
		{
			Stack<Texture::T2D>::instance().clear();
			Stack<Shader>::instance().clear();
			delete data;
		}
		else
		{
			KROSS_MSGBOX("Renderer2D is not initialized. Cannot call Renderer2D::Shutdown(void) while Renderer2D::Init(void) is not called.", __FUNCSIG__, _ERROR_);
		}
	}
	void Renderer2D::Begin(Camera::Camera& camera)
	{
		KROSS_PROFILE_FUNC();
		if (!SceneBegan)
			SceneBegan = true;
		else
			KROSS_CORE_WARN("Calling Renderer2D::Begin(Camera::Camera&) without calling Renderer2D::End(void). Overriding previous scene!");
		data->shader->Bind();
		data->shader->SetMat4("u_ViewProjection", camera.GetVPM());
		data->shader->SetMat4("u_Transform", glm::mat4(1.0f));
	}
	void Renderer2D::BatchBegin()
	{
		if (batch == false)
		{
			data->quadIndex = 0;
			batch = true;
		}
		else
		{
			KROSS_CORE_WARN("Batch already initiated.");
		}
	}
	void Renderer2D::Flush()
	{
		data->texArray->Bind(0);
		data->va->Bind();
		Renderer::Command::DrawIndexed(data->va);
		data->rendererStats.DrawCount++;

		data->quadIndex = 0;
		Texture::Base::ResetIndex();
	}
	void Renderer2D::BatchEnd()
	{
		if (batch)
		{
			data->vb->upload(data->myBuffer, sizeof(Quad) * data->quadIndex);
			Flush();
			batch = false;
		}
		else
		{
			KROSS_CORE_WARN("Ending a non initiated batch");
		}
	}
	void Renderer2D::End()
	{
		if (SceneBegan)
		{
			SceneBegan = false;
		}
		else
			KROSS_CORE_WARN("Calling Renderer2D::End(void) without calling Renderer2D::Begin(Camera::Camera&). Did you forget to Begin the Scene?");
	}
	void Renderer2D::SwitchShader(const Ref<Shader>& shader)
	{
		if (shader) {
			data->shader->unBind();
			data->shader = shader;
			data->shader->Bind();
		}
		else {
			KROSS_CORE_WARN("Shader provided is empty, or not supported."); return;
		}
	}
	void Renderer2D::BatchQuad(const QuadParams& params)
	{
		KROSS_PROFILE_FUNC();
		static float tex = (float)data->texArray->Get(data->whiteTex);
		static const glm::mat4 m4(1.0f);
		static glm::mat4 transform(1.0f);
		static Vertex v[4] = { Vertex() };
		static glm::vec2 texCoords[4] = {
							{ 0.0f, 0.0f },
							{ 1.0f, 0.0f },
							{ 1.0f, 1.0f },
							{ 0.0f, 1.0f }
		};

		if ((size_t)data->quadIndex + 1 >= MaxQuadCount) { BatchEnd(); BatchBegin(); }
		if (params.texture && !params.subTexture) {
			tex = (float)data->texArray->Get(params.texture);
			texCoords[0] = { 0.0f, 0.0f };
			texCoords[1] = { 1.0f, 0.0f };
			texCoords[2] = { 1.0f, 1.0f };
			texCoords[3] = { 0.0f, 1.0f };
		}
		else if (params.subTexture && !params.texture) {
			tex = (float)data->texArray->Get(params.subTexture->GetTexture());
			auto coords = params.subTexture->GetTexCoords();
			for (int i = 0; i < 4; i++) texCoords[i] = coords[i];
		}
		else if (params.texture && params.subTexture) { KROSS_CORE_ERROR_("[{0}] Texture AND subTexture was found. Please use only one of those.", __FUNCTION__); return; }
		else { KROSS_CORE_WARN("[{0}] Neither Texture nor Subtexture selected. Batch aborted.", __FUNCTION__); return; }

		for (char i = 0; i < 4; i++)
		{
			if (params.rotation != 0.0f) {
				transform = glm::translate(m4, params.position) *
					glm::rotate(m4, params.rotation, { 0.0f, 0.0f, 1.0f }) *
					glm::scale(m4, glm::vec3(params.size, 0.0f));
				v[i].pos = transform * glm::vec4(data->rotations.v[i].pos, 1.0f);
			}
			else {
				v[i].pos = {
					params.position.x + (params.size.x * (i == 1 || i == 2)),
					params.position.y + (params.size.y * (i == 2 || i == 3)),
					params.position.z };
			}
			v[i].color = params.color;
			v[i].texIndex = tex;
			v[i].texCoord = texCoords[i];
			//v[i].texCoord = { params.texOffSet.x + (params.texSubSize.x * (i == 1 || i == 2)), params.texOffSet.y + (params.texSubSize.y * (i == 2 || i == 3)) };
		}

		data->myBuffer[data->quadIndex] = Quad(v);
		data->quadIndex++;
		data->rendererStats.QuadCount++;
	}
}

