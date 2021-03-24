#include <Kross_pch.h>
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shaders.h"
#include "Renderer.h"
#include "Kross/Core/Application.h"
#include "Kross/Core/Stack.h"

#include "Textures/Array.h"
#include "Textures/Atlas.h"

static constexpr size_t MaxQuadCount = 100;
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
		Quad(Vertex tl, Vertex tr, Vertex br, Vertex bl) : tl(tl), tr(tr), br(br), bl(bl) {}
		Vertex tl;
		Vertex tr;
		Vertex br;
		Vertex bl;
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

		//Cache for Texture (Performance Optimization)
		std::vector<Ref<Texture::T2D>> texCache;

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
			data->texCache.resize(Texture::Base::QueryMaxSlots());
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

			data->rotations = Quad(
				Vertex(-0.5f, -0.5f),
				Vertex( 0.5f, -0.5f),
				Vertex( 0.5f,  0.5f),
				Vertex(-0.5f,  0.5f)
			);

			Stack<Shader>::instance().Add(data->shader = Shader::CreateRef("assets/shaders/OpenGL/Shader2D.glsl"));
			data->shader->SetFloat("u_Repeat", 1);
			data->shader->SetFloat4("u_Color", { 1,1,1,1 });

			data->shader->SetIntV("u_Textures", Texture::Base::QueryMaxSlots(), nullptr);

			uint32_t white = 0xffffffff;
			Stack<Texture::T2D>::instance().Add(data->whiteTex = Texture::T2D::CreateRef(1, 1, "blank", &white));

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
	void Renderer2D::BatchQuad(const QuadParams& params)
	{
		KROSS_PROFILE_FUNC();
		if (data->quadIndex + 1 >= MaxQuadCount)
		{
			BatchEnd();
			BatchBegin();
		}
		float tex = 0.0f;
		if (params.texture)
		{
			tex = (float)data->texArray->Get(params.texture);
			if (tex < 0.0f) data->texArray->Add(params.texture);
		}

		glm::vec4 p1(0.0f);
		glm::vec4 p2(0.0f);
		glm::vec4 p3(0.0f);
		glm::vec4 p4(0.0f);
		if (params.rotation == 0.0f)
		{
			p1 = { params.position.x,					params.position.y,					0.0f, 1.0f };
			p2 = { params.position.x + params.size.x,	params.position.y,					0.0f, 1.0f };
			p3 = { params.position.x + params.size.x,	params.position.y + params.size.y,	0.0f, 1.0f };
			p4 = { params.position.x,					params.position.y + params.size.y,	0.0f, 1.0f };
		}
		else
		{
			const glm::mat4 m4(1.0f);
			glm::mat4 transform = glm::translate(m4, glm::vec3(params.position, 0.0f)) *
				glm::rotate(m4, params.rotation, { 0.0f, 0.0f, 1.0f }) *
				glm::scale(m4, glm::vec3(params.size, 0.0f));
			p1 = transform * glm::vec4(data->rotations.tl.pos, 1.0f);
			p2 = transform * glm::vec4(data->rotations.tr.pos, 1.0f);
			p3 = transform * glm::vec4(data->rotations.br.pos, 1.0f);
			p4 = transform * glm::vec4(data->rotations.bl.pos, 1.0f);
		}

		data->myBuffer[data->quadIndex] = Quad(
			Vertex(glm::vec3(p1), params.color, params.texOffSet, tex),
			Vertex(glm::vec3(p2), params.color, { params.texOffSet.x + params.texSubSize.x	, params.texOffSet.y }, tex),
			Vertex(glm::vec3(p3), params.color, { params.texOffSet.x + params.texSubSize.x	, params.texOffSet.y + params.texSubSize.y }, tex),
			Vertex(glm::vec3(p4), params.color, { params.texOffSet.x, params.texOffSet.y + params.texSubSize.y }, tex)
		);

		data->quadIndex++;
		data->rendererStats.QuadCount++;
	}
}

