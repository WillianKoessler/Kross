#include <Kross_pch.h>
#include "Renderer3D.h"


#include "Kross/Core/Application.h"
#include "Kross/Core/Stack.h"

#include "Renderer.h"
#include "RenderTypes.h"
#include "VertexArray.h"
#include "Textures/Array.h"

#include <stdio.h>
#include <inttypes.h>

static constexpr size_t MaxQuadCount = 2048;
static constexpr size_t MaxVertexCount = MaxQuadCount * 4;
static constexpr size_t MaxIndexCount = MaxQuadCount * 6;

static constexpr size_t MaxPointsBatchCount = 2048;

namespace Kross {
	bool Renderer3D::s_bSceneBegan = false;
	bool Renderer3D::s_bBatch = false;
	bool Renderer3D::s_bInitiated = false;

	struct GeometryVertex {
		struct Decompressed { uint8_t x, y, z, r, g, b, a, f; };
		struct Compressed { uint32_t x = 0u, y = 0u; } data;
		static Compressed Compress(uint8_t x, uint8_t y, uint8_t z, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t f) {
			return { (uint32_t)(x << 24u | y << 16u | z << 8u | r), (uint32_t)(g << 24u | b << 16u | a << 8u | f) };
		}
		static Decompressed Decompress(const Compressed& data) {
			Decompressed p;
			p.x = (uint8_t)(data.x >> (8 * 3));
			p.y = (uint8_t)(data.x >> (8 * 2));
			p.z = (uint8_t)(data.x >> (8 * 1));
			p.r = (uint8_t)(data.x >> (8 * 0));
			p.g = (uint8_t)(data.y >> (8 * 3));
			p.b = (uint8_t)(data.y >> (8 * 2));
			p.a = (uint8_t)(data.y >> (8 * 1));
			p.f = (uint8_t)(data.y >> (8 * 0));
			return p;
		}
		std::string ReadCompressed() {
			char c1[128], c2[128];
			try { sprintf_s(c1, "0x%" PRIx32, data.x); sprintf_s(c2, "_%" PRIx32, data.y); } catch (const std::exception& e) { KROSS_CORE_ERROR("{0}", e.what()); return ""; }
			return std::string(c1) + c2;
		}
		std::string ReadDecompressed() {
			Decompressed& dData = Decompress(data);
			char c[256];
			try {
				sprintf_s(c, "x=0x%" PRIx8 ", y=0x%" PRIx8 ", z=0x%" PRIx8 " | r=0x%" PRIx8 ", g=0x%" PRIx8 ", b=0x%" PRIx8 ", a=0x%" PRIx8 " | f=0x%" PRIx8,
					dData.x, dData.y, dData.z, dData.r, dData.g, dData.b, dData.a, dData.f);
			} catch (const std::exception& e) { KROSS_CORE_ERROR("{0}", e.what()); return ""; }
			return c;
		}
		GeometryVertex() = default;
	};
	struct Vertex {
		Vertex() = default;
		glm::vec3 pos = glm::vec3(0.0f);
		glm::vec4 color = glm::vec4(1.0f);
		glm::vec2 texCoord = glm::vec2(0.0f);
		glm::vec3 normal = glm::vec3(0.0f);
		float texIndex = 0.0f;
	};
	struct Quad {
		Quad() = default;
		Quad(Vertex* vertices) { for (int i = 0; i < 4; i++) v[i] = vertices[i]; }
		Vertex v[4];
	};
	struct Cube {
		Cube() = default;
		Cube(Quad* faces) { for (int i = 0; i < 6; i++) f[i] = faces[i]; }
		Quad f[6];
	};
	struct PointBatchData {
		Ref<Shader> voxelShader;
		Scope<VertexArray> va;
		Ref<Buffer::Vertex> vb;
		GeometryVertex myBuffer[MaxPointsBatchCount] = { GeometryVertex() };
		//Vertex myBuffer[MaxPointsBatchCount] = { Vertex() };
		uint32_t pIndex = 0u;
	};
	static PointBatchData* pData;
	struct R3DData {
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
		uint32_t quadIndex = 0;

		//Texture Array
		Scope<Texture::T2DArray> texArray;

		//Renderer Status for ImGui window
		Renderer3D::Stats rendererStats = Renderer3D::Stats();

		//Quad local vertex positions (rotation)
		Quad rotations;
	};
	static R3DData* data;
	const Renderer3D::Stats& Renderer3D::getStats() {
		return data->rendererStats;
	}
	void Renderer3D::ResetStats() {
		data->rendererStats = Stats();
	}
	void Renderer3D::Init() {
		KROSS_PROFILE_FUNC();
		if (s_bInitiated) { KROSS_CORE_WARN("Renderer3D is already initialized. Cannot call Renderer3D::Init(void) twice. Forget to call Renderer3D::Shutdown(void)?"); return; }
		s_bInitiated = true;
		pData = new PointBatchData;

		pData->va = VertexArray::CreateScope();

		pData->vb = Buffer::Vertex::Create(nullptr, sizeof(GeometryVertex) * MaxPointsBatchCount, true);
		pData->vb->SetLayout({
			{ Buffer::ShaderDataType::uInt2, "a_Data"}
			});
		pData->va->AddVoxel(pData->vb);

		Stack<Shader>::instance().Add(pData->voxelShader = Shader::CreateRef("voxelShader", {
			"assets/shaders/OpenGL/voxel/voxel.vert",
			"assets/shaders/OpenGL/voxel/voxel.geom",
			"assets/shaders/OpenGL/voxel/voxel.frag",
			}));


		data = new R3DData;
		data->texArray = Texture::T2DArray::CreateScope(Texture::Base::QueryMaxSlots());

		data->va = VertexArray::CreateScope();

		data->vb = Buffer::Vertex::Create(nullptr, sizeof(Vertex) * MaxVertexCount, true);
		data->vb->SetLayout({
			{ Buffer::ShaderDataType::Float3, "a_Position" },
			{ Buffer::ShaderDataType::Float4, "a_Color"},
			{ Buffer::ShaderDataType::Float2, "a_TexCoord" },
			{ Buffer::ShaderDataType::Float3,  "a_Normal"},
			{ Buffer::ShaderDataType::Float,  "a_TexIndex"},
			});
		data->va->AddVertex(data->vb);

		uint32_t* indices = new uint32_t[MaxIndexCount];
		uint32_t offset = 0;
		for (int i = 0; i < MaxIndexCount; i += 6) {
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

		Vertex v[4];
		v[0].pos = { -0.5f, -0.5f, 0.0f };
		v[0].pos = { 0.5f, -0.5f, 0.0f };
		v[0].pos = { 0.5f,  0.5f, 0.0f };
		v[0].pos = { -0.5f,  0.5f, 0.0f };
		data->rotations = Quad(v);

		Stack<Shader>::instance().Add(data->shader = Shader::CreateRef("assets/shaders/OpenGL/Shader3D.glsl"));
		data->shader->SetFloat("u_Repeat", 1);
		data->shader->SetFloat4("u_Color", { 1,1,1,1 });
		data->shader->SetIntV("u_Textures", Texture::Base::QueryMaxSlots(), nullptr);

		unsigned char* white = new unsigned char[4]{ 255 };

		Stack<Texture::T2D>::instance().Add(data->whiteTex = Texture::T2D::CreateRef(1, 1, "blank", white));
		data->texArray->Add(Stack<Texture::T2D>::instance().Get("blank"));

		delete[] indices;

		RenderCommand::BackCull(false);

		s_bSceneBegan = false;
	}
	void Renderer3D::Shutdown() {
		KROSS_PROFILE_FUNC();
		if (!s_bInitiated) { KROSS_CORE_WARN("Renderer3D is not initialized. Cannot call Renderer3D::Shutdown(void) while Renderer3D::Init(void) is not called."); return; }

		Stack<Texture::T2D>::instance().clear();
		Stack<Shader>::instance().clear();
		delete data, pData;
	}
	void Renderer3D::Begin(Ref<Camera::Camera>& camera) {
		KROSS_PROFILE_FUNC();
		if (s_bSceneBegan) {
			KROSS_CORE_WARN("Calling Renderer3D::Begin(Ref<Camera::Camera>&) without calling Renderer3D::End(void).");
			End();
		} else s_bSceneBegan = true;

		data->shader->Bind();
		data->shader->SetMat4("u_ViewProjection", camera->GetVPM());
		data->shader->SetMat4("u_Transform", glm::mat4(1.0f));

		pData->voxelShader->Bind();
		pData->voxelShader->SetMat4("u_ViewProjection", camera->GetVPM());
		pData->voxelShader->SetMat4("u_Transform", glm::mat4(1.0f));

		BatchBegin();
	}
	void Renderer3D::DrawCube() {

	}
	void Renderer3D::DrawCube(const Params3D& params) {
		static constexpr uint8_t faces = 6;
		static uint8_t currentFace = 0;
		static glm::vec3 pos[faces];
		static glm::vec2 size[faces];
		static glm::vec4 color[faces];
		static glm::vec3 rotation[faces];
		
		QuadParams p = {
			pos[currentFace],
			size[currentFace],
			nullptr,
			nullptr,
			color[currentFace],
			rotation[currentFace],
		};
		p.texture = params.frontTexture;

		BatchQuad(p);
	}
	void Renderer3D::DrawVoxel(const Voxel& params) {
		GeometryVertex& p = pData->myBuffer[pData->pIndex++];
		const Voxel& v = params;
		p.data = GeometryVertex::Compress(
			v.position.x, v.position.y, v.position.z,
			v.color.r, v.color.g, v.color.b, v.color.a,
			params.visibleFaces
		);
		p.data.x = 0x000000FF;
		p.data.y = 0xFFFFFFFF;
		KROSS_CORE_TRACE("C={0} - D={1}", p.ReadCompressed(), p.ReadDecompressed());
	}
	void Renderer3D::End() {
		if (s_bSceneBegan) s_bSceneBegan = false;
		else KROSS_CORE_WARN("Calling Renderer3D::End(void) without calling Renderer3D::Begin(Camera::Camera&). Did you forget to Begin the Scene?");
		BatchEnd();
	}
	void Renderer3D::SwitchShader(const Ref<Shader>& shader) {
		if (shader) {
			data->shader->unBind();
			data->shader = shader;
			data->shader->Bind();
		} else {
			KROSS_CORE_WARN("Shader provided is empty, or not supported."); return;
		}
	}
	void Renderer3D::BatchBegin() {
		if (s_bBatch) { KROSS_CORE_WARN("Batch already initiated."); return; }
		data->quadIndex = 0;
		pData->pIndex = 0;
		s_bBatch = true;
	}
	void Renderer3D::Flush() {
		data->texArray->Bind(0);
		data->va->Bind();
		data->shader->Bind();
		RenderCommand::DrawIndexed(data->va);
		data->rendererStats.DrawCount++;

		data->quadIndex = 0;
		Texture::Base::ResetIndex();

		pData->va->Bind();
		//Renderer::Command::DrawIndexed(pData->va);
		pData->voxelShader->Bind();
		RenderCommand::DrawArrays(pData->va);

		//Renderer::Submit()
		pData->pIndex = 0;
	}
	void Renderer3D::BatchEnd() {
		if (!s_bBatch) { KROSS_CORE_WARN("Ending a non initiated batch"); return; }
		data->vb->upload(data->myBuffer, sizeof(Quad) * data->quadIndex);
		pData->vb->upload(pData->myBuffer, sizeof(GeometryVertex) * pData->pIndex);
		Flush();
		s_bBatch = false;
	}
	void Renderer3D::BatchQuad(QuadParams& params) {
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
		} else if (params.subTexture && !params.texture) {
			tex = (float)data->texArray->Get(params.subTexture->GetTexture());
			auto coords = params.subTexture->GetTexCoords();
			for (int i = 0; i < 4; i++) texCoords[i] = coords[i];
		} else if (params.texture && params.subTexture) { KROSS_CORE_ERROR("Texture AND subTexture was found. Please supply only one of each."); return; } else {
			KROSS_CORE_WARN("Neither Texture nor Subtexture selected. Default Selected.");
			params.texture = Stack<Texture::T2D>::instance().Get("blank");
		}

		if (params.rotation != glm::vec3(0.0f)) {
			for (uint8_t i = 0; i < 4; i++) {
				transform = glm::translate(m4, params.position) *
					(params.rotation.x != 0.0f ? (glm::rotate(m4, params.rotation.x, { 1.0f, 0.0f, 0.0f })) : m4) *
					(params.rotation.y != 0.0f ? (glm::rotate(m4, params.rotation.y, { 0.0f, 1.0f, 0.0f })) : m4) *
					(params.rotation.z != 0.0f ? (glm::rotate(m4, params.rotation.z, { 0.0f, 0.0f, 1.0f })) : m4) *
					glm::scale(m4, glm::vec3(params.size, 0.0f));

				v[i].pos = transform * glm::vec4(data->rotations.v[i].pos, 1.0f);
				v[i].color = params.color;
				v[i].texIndex = tex;
				v[i].texCoord = texCoords[i];
				//v[i].texCoord = { params.texOffSet.x + (params.texSubSize.x * (i == 1 || i == 2)), params.texOffSet.y + (params.texSubSize.y * (i == 2 || i == 3)) };
			}
		} else
			for (uint8_t i = 0; i < 4; i++) {
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
}

