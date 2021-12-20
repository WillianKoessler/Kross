#include <Kross_pch.h>
#include "Renderer2D.h"


#include "Kross/Core/Application.h"
#include "Kross/Core/Stack.h"

#include "Renderer.h"
#include "RenderTypes.h"
#include "VertexArray.h"
#include "Textures/Array.h"

#include "Kross/Core/Input.h"

static constexpr size_t MaxQuadCount = 512;
static constexpr size_t MaxVertexCount = MaxQuadCount * 4;
static constexpr size_t MaxIndexCount = MaxQuadCount * 6;

namespace Kross {
	bool Renderer2D::s_bSceneBegan = false;
	bool Renderer2D::s_bBatch = false;
	bool Renderer2D::s_bInitiated = false;

	struct Vertex
	{
		Vertex() = default;
		Vertex(float x, float y)
			: pos(x, y, 0.0f) {}
		Vertex(const glm::vec3 &pos, const glm::vec4 &color, const glm::vec2 &texCoord, float texIndex)
			: pos(pos), color(color), texCoord(texCoord), texIndex(texIndex) {}
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec4 color = glm::vec4(1.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		float texIndex = -1.0f;
		void print() const {
			printf("\tVertex([%.2f, %.2f, %.2f], [%.2f, %.2f, %.2f, %.2f], [%.2f, %.2f], [%.2f])\n",
				pos.x, pos.y, pos.z,
				color.r, color.g, color.b, color.a,
				texCoord.x, texCoord.y,
				texIndex
			);
		}
	};
	struct Quad
	{
		void print(int index) const { printf("Quad %d{\n", index); for (int i = 0; i < 4; i++) v[i].print(); printf("}\n"); }
		Quad() = default;
		Quad(Vertex *vertices) { for (int i = 0; i < 4; i++) v[i] = vertices[i]; }
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
		uint32_t *indices;

		//Batch Buffer
		Quad *myBuffer;
		Quad *readHead;

		Vertex *vBuffer;
		Vertex *vReadHead;

		//Batch Quad Counter
		uint32_t quadIndex = 0;

		//Texture Array
		Scope<Texture::T2DArray> texArray;

		//Renderer Status for ImGui window
		Renderer2D::Stats rendererStats = Renderer2D::Stats();

		//Quad first vertex positions
		glm::vec4 basePositions[4];
	};
	static R2DData *data;
	const Renderer2D::Stats &Renderer2D::getStats()
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
		if (s_bInitiated) { KROSS_WARN("Renderer2D is already initialized. Cannot call Renderer2D::Init(void) twice. Forget to call Renderer2D::Shutdown(void)?"); return; }
		s_bInitiated = true;

		data = new R2DData;

		data->myBuffer = new Quad[MaxQuadCount];
		data->readHead = data->myBuffer;

		data->vBuffer = new Vertex[MaxVertexCount];
		data->vReadHead = data->vBuffer;

		data->texArray = Texture::T2DArray::CreateScope("Renderer2D_TextureArray", Texture::Base::QueryMaxSlots());

		data->va = VertexArray::CreateScope("Renderer2D_VAO");

		data->vb = Buffer::Vertex::Create("Renderer2D_VBO", nullptr, sizeof(Vertex) * MaxVertexCount, true);
		data->vb->SetLayout({
			{ Buffer::ShaderDataType::Float3, "a_Position" },
			{ Buffer::ShaderDataType::Float4, "a_Color"},
			{ Buffer::ShaderDataType::Float2, "a_TexCoord" },
			{ Buffer::ShaderDataType::Float,  "a_TexIndex"}
			});
		data->va->AddVertex(data->vb);

		uint32_t *indices = new uint32_t[MaxIndexCount];
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

		data->ib = Buffer::Index::Create("Renderer2D_IBO", indices, sizeof(uint32_t) * MaxIndexCount);
		data->va->SetIndex(data->ib);

		data->basePositions[0] = glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f);
		data->basePositions[1] = glm::vec4(0.5f, -0.5f, 0.0f, 1.0f);
		data->basePositions[2] = glm::vec4(0.5f, 0.5f, 0.0f, 1.0f);
		data->basePositions[3] = glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f);


		Stack<Shader>::Add(data->shader = Shader::CreateRef("Shader2D", {
			"assets/shaders/OpenGL/Shader2D.vert",
			"assets/shaders/OpenGL/Shader2D.frag"
			}));
		data->shader->SetFloat("u_Repeat", 1);
		data->shader->SetFloat4("u_Color", { 1,1,1,1 });

		data->shader->SetIntV("u_Textures", Texture::Base::QueryMaxSlots(), nullptr);

		unsigned char *white = new unsigned char[4]{ 255 };

		Stack<Texture::T2D>::Add(data->whiteTex = Texture::T2D::CreateRef("blank", 1, 1, white));
		data->texArray->Get(Stack<Texture::T2D>::Get("blank"));

		delete[] indices;

		RenderCommand::BackCull(true);

		s_bSceneBegan = false;
	}
	void Renderer2D::Shutdown()
	{
		KROSS_PROFILE_FUNC();
		if (!s_bInitiated) { KROSS_WARN("Renderer2D is not initialized. Cannot call Renderer2D::Shutdown(void) while Renderer2D::Init(void) is not called."); return; }
		delete[] data->myBuffer;
		delete[] data->vBuffer;
		delete data;
	}
	void Renderer2D::Begin(const Camera::Editor &camera)
	{
		KROSS_PROFILE_FUNC();
		if (!s_bSceneBegan) s_bSceneBegan = true;
		else KROSS_WARN("Calling {0} without calling Renderer2D::End(void). Overriding previous scene!", __FUNCTION__);
		data->shader->Bind();
		data->shader->SetMat4("u_ViewProjection", camera.GetVPM());
		BatchBegin();
	}
	void Renderer2D::Begin(const CameraComponent &camera, const TransformComponent &transform)
	{
		KROSS_PROFILE_FUNC();
		if (!s_bSceneBegan) s_bSceneBegan = true;
		else KROSS_WARN("Calling {0} without calling Renderer2D::End(void). Overriding previous scene!", __FUNCTION__);
		data->shader->Bind();
		data->shader->SetMat4("u_ViewProjection", camera.camera.GetProjMat() * glm::inverse((glm::mat4)transform));
		BatchBegin();
	}
	void Renderer2D::Begin(Ref<Camera::Camera> &camera)
	{
		KROSS_NOT_IMPLEMENTED;
		//KROSS_PROFILE_FUNC();
		//if (!s_bSceneBegan) s_bSceneBegan = true;
		//else KROSS_WARN("Calling {0} without calling Renderer2D::End(void). Overriding previous scene!", __FUNCTION__);
		//data->shader->Bind();
		//data->shader->SetMat4("u_ViewProjection", camera->GetVPM());
		//data->shader->SetMat4("u_Transform", glm::mat4(1.0f));
		//BatchBegin();
	}
	void Renderer2D::BatchBegin()
	{
		if (s_bBatch) { KROSS_WARN("Batch already initiated."); return; }
		data->quadIndex = 0;
		data->readHead = data->myBuffer;
		data->vReadHead = data->vBuffer;
		s_bBatch = true;
	}
	void Renderer2D::Flush()
	{
		if (data->quadIndex == 0) return;

		uint32_t size = (uint32_t)((uint8_t*)data->readHead - (uint8_t*)data->myBuffer);
		data->vb->Upload(data->myBuffer, size);

		data->texArray->Bind();
		data->va->Bind();
		data->shader->Bind();

		RenderCommand::DrawIndexed(data->va, data->quadIndex);
		data->rendererStats.DrawCount++;

		data->quadIndex = 0;
		Texture::Base::ResetIndex();
	}
	void Renderer2D::BatchEnd()
	{
		if (!s_bBatch) { KROSS_WARN("Ending a non initiated batch"); return; }
		s_bBatch = false;


		if (Input::IsKeyPressed(Key::Space)) {
			KROSS_TRACE("Printing buffer: ");
			for (uint32_t i = 0; i < data->quadIndex; i++)
				data->myBuffer[i].print(i);
		}

		Flush();

		//data->vb->Clear();
		//memset(data->myBuffer, 0, sizeof(data->myBuffer));
	}
	void Renderer2D::End()
	{
		if (!s_bSceneBegan) { KROSS_WARN("Ending a non Initiated Scene. Did you forget to Begin the Scene?", __FUNCTION__); return; }
		s_bSceneBegan = false;
		BatchEnd();
	}
	void Renderer2D::SwitchShader(const Ref<Shader> &shader)
	{
		if (shader) {
			data->shader->unBind();
			data->shader = shader;
			data->shader->Bind();
		} else { KROSS_WARN("Shader provided is empty, or not supported."); return; }
	}
	void Renderer2D::BatchQuad(const QuadParams &params)
	{
		KROSS_NOT_IMPLEMENTED;
		//KROSS_PROFILE_FUNC();
		//
		//if (!s_bBatch) return;
		//
		//static const glm::mat4 m4(1.0f);
		//static glm::mat4 transform(1.0f);
		//static Vertex v[4] = { Vertex() };
		//static glm::vec2 texCoords[4] = {
		//					{ 0.0f, 0.0f },
		//					{ 1.0f, 0.0f },
		//					{ 1.0f, 1.0f },
		//					{ 0.0f, 1.0f }
		//};
		//
		//if ((size_t)data->quadIndex + 1 >= MaxQuadCount) { BatchEnd(); BatchBegin(); }
		//
		//float tex = (float)data->texArray->Get(data->whiteTex);
		//
		//if (params.texture && !params.subTexture)
		//{
		//	tex = (float)data->texArray->Get(params.texture);
		//	texCoords[0] = { 0.0f, 0.0f };
		//	texCoords[1] = { 1.0f, 0.0f };
		//	texCoords[2] = { 1.0f, 1.0f };
		//	texCoords[3] = { 0.0f, 1.0f };
		//} else if (params.subTexture && !params.texture) {
		//	tex = (float)data->texArray->Get(params.subTexture->GetTexture());
		//	auto coords = params.subTexture->GetTexCoords();
		//	for (int i = 0; i < 4; i++) texCoords[i] = coords[i];
		//} else if (params.texture && params.subTexture) { KROSS_ERROR("Texture AND subTexture was found. Please supply only one of each."); return; }
		//
		//if (params.rotation != glm::vec3(0.0f)) {
		//	for (uint8_t i = 0; i < 4; i++)
		//	{
		//		transform = glm::translate(m4, params.position) *
		//			glm::rotate(m4, params.rotation.x, { 1.0f, 0.0f, 0.0f }) *
		//			glm::rotate(m4, params.rotation.y, { 0.0f, 1.0f, 0.0f }) *
		//			glm::rotate(m4, params.rotation.z, { 0.0f, 0.0f, 1.0f }) *
		//			glm::scale(m4, glm::vec3(params.size, 0.0f));
		//
		//		v[i].pos = transform * data->basePositions[i];
		//		v[i].color = params.color;
		//		v[i].texIndex = tex;
		//		v[i].texCoord = texCoords[i];
		//	}
		//} else {
		//	for (uint8_t i = 0; i < 4; i++)
		//	{
		//		v[i].pos = {
		//			params.position.x + (0.5f * params.size.x * (i == 1 || i == 2)) - (0.5f * params.size.x * (i == 0 || i == 3)),
		//			params.position.y + (0.5f * params.size.y * (i == 2 || i == 3)) - (0.5f * params.size.y * (i == 0 || i == 1)),
		//			params.position.z };
		//		v[i].color = params.color;
		//		v[i].texIndex = tex;
		//		v[i].texCoord = texCoords[i];
		//		//v[i].texCoord = { params.texOffSet.x + (params.texSubSize.x * (i == 1 || i == 2)), params.texOffSet.y + (params.texSubSize.y * (i == 2 || i == 3)) };
		//	}
		//}
		//data->myBuffer[data->quadIndex] = Quad(v);
		//data->quadIndex++;
		//data->rendererStats.QuadCount++;
	}
	void Renderer2D::BatchQuad(const glm::mat4 &transform, const glm::vec4 &color, const Ref<Texture::T2D>& sprite)
	{
		KROSS_PROFILE_FUNC();
		if (!s_bBatch) return;
		static const float whiteTex = (float)data->texArray->Get(data->whiteTex);
		static const glm::vec2 texCoords[4] = {
							{ 0.0f, 0.0f },
							{ 1.0f, 0.0f },
							{ 1.0f, 1.0f },
							{ 0.0f, 1.0f }
		};

		if ((size_t)data->quadIndex + 1 >= MaxQuadCount) { BatchEnd(); BatchBegin(); }

		float tex = whiteTex;
		if (sprite) tex = (float)data->texArray->Get(sprite);

		Quad *quad = data->readHead;

		for (uint8_t i = 0; i < 4; i++)
		{
			quad->v[i].pos = transform * data->basePositions[i];
			quad->v[i].color = color;
			quad->v[i].texIndex = tex;
			quad->v[i].texCoord = texCoords[i];
		}
		data->readHead++;

		data->quadIndex += 6;
		data->rendererStats.QuadCount++;
	}
	void Renderer2D::DebugAxis()
	{
		static Timestep time;
		float ts = (float)time.GetLap();
		static float count = 0.0f;
		count += ts * 100.0f;

		// WORLD ORIGIN
		static bool axis = true;
		static bool zero = true;
		{
			QuadParams params;
			params.texture = data->whiteTex;
			params.position = glm::vec3(0.0f, 0.0f, 0.0f);

			params.color = glm::vec4(1.0f, 0.0f, 0.0f, .7f);
			BatchQuad(params);
			params.rotation = glm::vec3(0.0f, glm::pi<float>(), 0.0f);
			BatchQuad(params);

			params.color = glm::vec4(0.0f, 1.0f, 0.0f, .7f);
			params.rotation = glm::vec3(0.0f, glm::half_pi<float>(), 0.0f);
			BatchQuad(params);
			params.rotation = glm::vec3(0.0f, glm::half_pi<float>() * 3.0f, 0.0f);
			BatchQuad(params);

			params.color = glm::vec4(0.0f, 0.0f, 1.0f, .7f);
			params.rotation = glm::vec3(glm::half_pi<float>(), 0.0f, 0.0f);
			BatchQuad(params);
			params.rotation = glm::vec3(glm::half_pi<float>() * 3.0f, 0.0f, 0.0f);
			BatchQuad(params);
		}

		QuadParams params;
		// Y
		if (axis)
		{
			params.color = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
			params.size = { 0.2f, 2.0f };
			params.position = glm::vec3(0.0f, 1.0f, 0.0f);
			params.rotation = glm::vec3(0.0f);
			for (int i = 0; i < 4; i++) {
				params.rotation.y = glm::half_pi<float>() * i;
				BatchQuad(params);
			}

			params.color = glm::vec4(0.0f, 1.0f, 1.0f, 1.0f);
			params.size = { 2.f, .2f };
			params.position = glm::vec3(1.0f, 0.0f, 0.0f);
			params.rotation = glm::vec3(0.0f);
			for (int i = 0; i < 4; i++) {
				params.rotation.x = glm::half_pi<float>() * i;
				BatchQuad(params);
			}

			params.color = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);
			params.size = { 2.f, .2f };
			params.position = glm::vec3(0.0f, 0.0f, 1.0f);

			for (int i = 0; i < 4; i++) {
				params.rotation = glm::vec3(glm::half_pi<float>(), glm::half_pi<float>() * i, glm::half_pi<float>());
				BatchQuad(params);
			}
		}
	}
}

