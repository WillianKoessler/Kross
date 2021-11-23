#include <Kross_pch.h>
#include "Renderer2D.h"


#include "Kross/Core/Application.h"
#include "Kross/Core/Stack.h"

#include "Renderer.h"
#include "RenderTypes.h"
#include "VertexArray.h"
#include "Textures/Array.h"

static constexpr size_t MaxQuadCount = 2048;
static constexpr size_t MaxVertexCount = MaxQuadCount * 4;
static constexpr size_t MaxIndexCount = MaxQuadCount * 6;

namespace Kross {
	bool Renderer2D::s_bSceneBegan = false;
	bool Renderer2D::s_bBatch = false;
	bool Renderer2D::s_bInitiated = false;

	struct Vertex
	{
		Vertex() = default;
		Vertex(float x, float y) :
			pos(x, y, 0.0f)
		{
		}
		Vertex(const glm::vec3& pos, const glm::vec4& color, const glm::vec2& texCoord, float texIndex) :
			pos(pos), color(color), texCoord(texCoord), texIndex(texIndex)
		{
		}
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec4 color = glm::vec4(1.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		float texIndex = 0.0f;
	};
	struct Quad
	{
		Quad() = default;
		Quad(Vertex* vertices) { for (int i = 0; i < 4; i++) v[i] = vertices[i]; }
		Vertex v[4];
	};

	struct R2DData
	{
		// Basic 2D Shader
		Ref<Shader> shader;

		// 1x1 White Texture
		Ref<Texture::T2D> whiteTex;
		uint32_t whiteTexIndex;

		//Batch data
		Scope<VertexArray> va;
		Ref<Buffer::Vertex> vb;
		Ref<Buffer::Index> ib;
		uint32_t* indices;

		//Batch Buffer
		Quad myBuffer[MaxQuadCount] = { Quad() };

		//Batch Quad Counter
		uint32_t quadIndex = 0;

		//Texture Array
		Scope<Texture::T2DArray> texArray;

		//Renderer Status for ImGui window
		Renderer2D::Stats rendererStats = Renderer2D::Stats();

		//Quad first vertex positions
		glm::vec4 basePositions[4];
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
		if (s_bInitiated) { KROSS_MSGBOX("Renderer2D is already initialized. Cannot call Renderer2D::Init(void) twice. Forget to call Renderer2D::Shutdown(void)?", __FUNCSIG__, _ERROR_); return; }
		s_bInitiated = true;

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

		data->indices = new uint32_t[MaxIndexCount];
		uint32_t offset = 0;
		for (int i = 0; i < MaxIndexCount; i += 6)
		{
			data->indices[i + 0] = 0 + offset;
			data->indices[i + 1] = 1 + offset;
			data->indices[i + 2] = 2 + offset;

			data->indices[i + 3] = 2 + offset;
			data->indices[i + 4] = 3 + offset;
			data->indices[i + 5] = 0 + offset;

			offset += 4;
		}

		data->ib = Buffer::Index::Create(data->indices, sizeof(uint32_t) * MaxIndexCount);
		data->va->SetIndex(data->ib);

		data->basePositions[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		data->basePositions[1] = glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f);
		data->basePositions[2] = glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f);
		data->basePositions[3] = glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f);

		Stack<Shader>::instance().Add(data->shader = Shader::CreateRef("assets/shaders/OpenGL/Shader2D.glsl"));
		data->shader->SetFloat("u_Repeat", 1);
		data->shader->SetFloat4("u_Color", { 1,1,1,1 });

		data->shader->SetIntV("u_Textures", Texture::Base::QueryMaxSlots(), nullptr);

		unsigned char* white = new unsigned char[4]{ 255 };

		Stack<Texture::T2D>::instance().Add(data->whiteTex = Texture::T2D::CreateRef(1, 1, "blank", white));
		data->texArray->Add(Stack<Texture::T2D>::instance().Get("blank"));

		s_bSceneBegan = false;
	}
	void Renderer2D::Shutdown()
	{
		KROSS_PROFILE_FUNC();
		if (!s_bInitiated) { KROSS_MSGBOX("Renderer2D is not initialized. Cannot call Renderer2D::Shutdown(void) while Renderer2D::Init(void) is not called.", __FUNCSIG__, _ERROR_); return; }

		Stack<Texture::T2D>::instance().clear();
		Stack<Shader>::instance().clear();
		delete[] data->indices;
		delete data;
	}
	void Renderer2D::Begin(Ref<Camera::Camera>& camera)
	{
		KROSS_PROFILE_FUNC();
		if (!s_bSceneBegan)
			s_bSceneBegan = true;
		else
			KROSS_CORE_WARN("Calling Renderer2D::Begin(Camera::Camera&) without calling Renderer2D::End(void). Overriding previous scene!");
		data->shader->Bind();
		data->shader->SetMat4("u_ViewProjection", camera->GetVPM());
		data->shader->SetMat4("u_Transform", glm::mat4(1.0f));
	}
	void Renderer2D::Begin(const Camera::Camera& camera)
	{
		KROSS_PROFILE_FUNC();
		if (!s_bSceneBegan)
			s_bSceneBegan = true;
		else
			KROSS_CORE_WARN("Calling Renderer2D::Begin(Camera::Camera&) without calling Renderer2D::End(void). Overriding previous scene!");
		data->shader->Bind();
		data->shader->SetMat4("u_ViewProjection", camera.GetVPM());
		data->shader->SetMat4("u_Transform", glm::mat4(1.0f));
	}
	void Renderer2D::BatchBegin()
	{
		if (s_bBatch == false)
		{
			data->quadIndex = 0;
			s_bBatch = true;
		} else
		{
			KROSS_CORE_WARN("Batch already initiated.");
		}
	}
	void Renderer2D::Flush()
	{
		data->texArray->Bind(0);
		data->va->Bind();
		RenderCommand::DrawIndexed(data->va);
		data->rendererStats.DrawCount++;

		data->quadIndex = 0;
		Texture::Base::ResetIndex();
	}
	void Renderer2D::BatchEnd()
	{
		if (s_bBatch)
		{
			data->vb->upload(data->myBuffer, sizeof(Quad) * data->quadIndex);
			Flush();
			s_bBatch = false;
		} else
		{
			KROSS_CORE_WARN("Ending a non initiated batch");
		}
	}
	void Renderer2D::End()
	{
		if (s_bSceneBegan)
		{
			s_bSceneBegan = false;
		} else
			KROSS_CORE_WARN("Calling Renderer2D::End(void) without calling Renderer2D::Begin(Camera::Camera&). Did you forget to Begin the Scene?");
	}
	void Renderer2D::SwitchShader(const Ref<Shader>& shader)
	{
		if (shader)
		{
			data->shader->unBind();
			data->shader = shader;
			data->shader->Bind();
		} else
		{
			KROSS_CORE_WARN("Shader provided is empty, or not supported."); return;
		}
	}
	void Renderer2D::BatchQuad(const QuadParams& params)
	{
		KROSS_PROFILE_FUNC();
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
		
		float tex = (float)data->whiteTexIndex;

		if (params.texture && !params.subTexture)
		{
			tex = (float)data->texArray->Get(params.texture);
			texCoords[0] = { 0.0f, 0.0f };
			texCoords[1] = { 1.0f, 0.0f };
			texCoords[2] = { 1.0f, 1.0f };
			texCoords[3] = { 0.0f, 1.0f };
		} else if (params.subTexture && !params.texture)
		{
			tex = (float)data->texArray->Get(params.subTexture->GetTexture());
			auto coords = params.subTexture->GetTexCoords();
			for (int i = 0; i < 4; i++) texCoords[i] = coords[i];
		} else if (params.texture && params.subTexture) { KROSS_CORE_ERROR_("[{0}] Texture AND subTexture was found. Please supply only one of each.", __FUNCTION__); return; }

		if (params.rotation != glm::vec3(0.0f))
			for (uint8_t i = 0; i < 4; i++)
			{
				transform = glm::translate(m4, params.position) *
					(params.rotation.x != 0.0f ? (glm::rotate(m4, params.rotation.x, { 1.0f, 0.0f, 0.0f })) : m4) *
					(params.rotation.y != 0.0f ? (glm::rotate(m4, params.rotation.y, { 0.0f, 1.0f, 0.0f })) : m4) *
					(params.rotation.z != 0.0f ? (glm::rotate(m4, params.rotation.z, { 0.0f, 0.0f, 1.0f })) : m4) *
					glm::scale(m4, glm::vec3(params.size, 0.0f));

				v[i].pos = transform * data->basePositions[i];
				v[i].color = params.color;
				v[i].texIndex = tex;
				v[i].texCoord = texCoords[i];
				//v[i].texCoord = { params.texOffSet.x + (params.texSubSize.x * (i == 1 || i == 2)), params.texOffSet.y + (params.texSubSize.y * (i == 2 || i == 3)) };
			}
		else
			for (uint8_t i = 0; i < 4; i++)
			{
				v[i].pos = {
					params.position.x + (params.size.x * (i == 1 || i == 2)),
					params.position.y + (params.size.y * (i == 2 || i == 3)),
					params.position.z };
				v[i].color = params.color;
				v[i].texIndex = tex;
				v[i].texCoord = texCoords[i];
				//v[i].texCoord = { params.texOffSet.x + (params.texSubSize.x * (i == 1 || i == 2)), params.texOffSet.y + (params.texSubSize.y * (i == 2 || i == 3)) };
			}

		data->myBuffer[data->quadIndex] = Quad(v);
		data->quadIndex++;
		data->rendererStats.QuadCount++;
	}
	void Renderer2D::BatchQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		KROSS_PROFILE_FUNC();
		static float tex = (float)data->texArray->Get(data->whiteTex);
		static glm::vec2 texCoords[4] = {
							{ 0.0f, 0.0f },
							{ 1.0f, 0.0f },
							{ 1.0f, 1.0f },
							{ 0.0f, 1.0f }
		};

		if ((size_t)data->quadIndex + 1 >= MaxQuadCount) { BatchEnd(); BatchBegin(); }

		Vertex* v = data->myBuffer[data->quadIndex].v;

		for (uint8_t i = 0; i < 4; i++)
		{
			v[i].pos = transform * data->basePositions[i];
			v[i].color = color;
			v[i].texIndex = tex;
			v[i].texCoord = texCoords[i];
		}
		data->quadIndex++;
		data->rendererStats.QuadCount++;
	}
	void Renderer2D::BatchQuad(const glm::mat4& transform, const Ref<Texture::T2D>& texture)
	{
		KROSS_PROFILE_FUNC();
		static float tex = (float)data->texArray->Get(data->whiteTex);
		static glm::vec2 texCoords[4] = {
							{ 0.0f, 0.0f },
							{ 1.0f, 0.0f },
							{ 1.0f, 1.0f },
							{ 0.0f, 1.0f }
		};

		if ((size_t)data->quadIndex + 1 >= MaxQuadCount) { BatchEnd(); BatchBegin(); }

		if (texture)
			tex = (float)data->texArray->Get(texture);

		Vertex* v = data->myBuffer[data->quadIndex].v;

		for (uint8_t i = 0; i < 4; i++)
		{
			v[i].pos = transform * data->basePositions[i];
			v[i].color = glm::vec4(1.0f);
			v[i].texIndex = tex;
			v[i].texCoord = texCoords[i];
		}
		data->quadIndex++;
		data->rendererStats.QuadCount++;
	}
}

