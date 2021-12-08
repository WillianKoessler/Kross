#include <Kross_pch.h>
#include "VoxelRenderer.h"

#include "Kross/Core/Application.h"
#include "Kross/Core/Stack.h"

#include "Renderer.h"
#include "RenderTypes.h"
#include "VertexArray.h"
#include "Textures/Array.h"

#include <inttypes.h>

static constexpr size_t MaxQuadCount = 2048;
static constexpr size_t MaxVertexCount = MaxQuadCount * 4;
static constexpr size_t MaxIndexCount = MaxQuadCount * 6;

static constexpr size_t MaxPointsBatchCount = 2048;

namespace Kross {
	bool VoxelRenderer::s_bSceneBegan = false;
	bool VoxelRenderer::s_bBatch = false;
	bool VoxelRenderer::s_bInitiated = false;

	struct VoxelVertex
	{
		struct Decompressed { uint8_t x, y, z, r, g, b, a, f; };
		struct Compressed { uint32_t x = 0u, y = 0u; };
		static Compressed Compress(uint8_t x, uint8_t y, uint8_t z, uint8_t r, uint8_t g, uint8_t b, uint8_t a, uint8_t f)
		{
			return { (uint32_t)(x << 24u | y << 16u | z << 8u | r), (uint32_t)(g << 24u | b << 16u | a << 8u | f) };
		}
		static Decompressed Decompress(const Compressed& data)
		{
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
		//std::string ReadCompressed()
		//{
		//	char c1[128], c2[128];
		//	try { sprintf_s(c1, "0x%" PRIx32, data.x); sprintf_s(c2, "_%" PRIx32, data.y); } catch (const std::exception& e) { KROSS_ERROR("{0}", e.what()); return ""; }
		//	return std::string(c1) + c2;
		//}
		//std::string ReadDecompressed()
		//{
		//	Decompressed& dData = Decompress(data);
		//	char c[256];
		//	try {
		//		sprintf_s(c, "x=0x%" PRIx8 ", y=0x%" PRIx8 ", z=0x%" PRIx8 " | r=0x%" PRIx8 ", g=0x%" PRIx8 ", b=0x%" PRIx8 ", a=0x%" PRIx8 " | f=0x%" PRIx8,
		//			dData.x, dData.y, dData.z, dData.r, dData.g, dData.b, dData.a, dData.f);
		//	} catch (const std::exception& e) { KROSS_ERROR("{0}", e.what()); return ""; }
		//	return c;
		//}

		glm::vec3 pos;
		glm::vec4 color;
		VoxelVertex() = default;
	};
	struct PointBatchData
	{
		VoxelRenderer::Stats rendererStats = VoxelRenderer::Stats();
		Ref<Shader> shader;
		Scope<VertexArray> va;
		Ref<Buffer::Vertex> vb;
		VoxelVertex myBuffer[MaxPointsBatchCount] = { VoxelVertex() };
		uint32_t pIndex = 0u;
	};
	static PointBatchData* data;
	const VoxelRenderer::Stats& VoxelRenderer::getStats()
	{
		return data->rendererStats;
	}
	void VoxelRenderer::ResetStats()
	{
		data->rendererStats = Stats();
	}
	void VoxelRenderer::Init()
	{
		KROSS_PROFILE_FUNC();
		KROSS_WARN("CALLING UNFINISHED CODE. BUGS MAY HAPPEN");
		if (s_bInitiated) { KROSS_WARN("VoxelRenderer is already initialized. Cannot call VoxelRenderer::Init(void) twice. Forget to call VoxelRenderer::Shutdown(void)?"); return; }
		s_bInitiated = true;
		data = new PointBatchData;

		data->va = VertexArray::CreateScope("Voxel VertexArray");

		data->vb = Buffer::Vertex::Create("Voxel VertexBuffer", nullptr, sizeof(VoxelVertex) * MaxPointsBatchCount, true);
		data->vb->SetLayout({
			//{ Buffer::ShaderDataType::uInt2, "a_Data"}
			{Buffer::ShaderDataType::Float3, "a_Pos"},
			{Buffer::ShaderDataType::Float, "a_Color"}
			});
		data->va->AddVoxel(data->vb);

		Stack<Shader>::Add(data->shader = Shader::CreateRef("voxelShader", {
			"assets/shaders/OpenGL/voxel/voxel.vert",
			"assets/shaders/OpenGL/voxel/voxel.geom",
			"assets/shaders/OpenGL/voxel/voxel.frag",
			}));

		RenderCommand::BackCull(false);

		s_bSceneBegan = false;
	}
	void VoxelRenderer::Shutdown()
	{
		KROSS_PROFILE_FUNC();
		if (!s_bInitiated) { KROSS_WARN("VoxelRenderer is not initialized. Cannot call VoxelRenderer::Shutdown(void) while VoxelRenderer::Init(void) is not called."); return; }

		Stack<Texture::T2D>::clear();
		Stack<Shader>::clear();
		delete data;
	}
	void VoxelRenderer::Begin(Ref<Camera::Camera>& camera)
	{
		KROSS_ERROR("Not Implemented");
		//KROSS_PROFILE_FUNC();
		//if (s_bSceneBegan) {
		//	KROSS_WARN("Calling VoxelRenderer::Begin(Ref<Camera::Camera>&) without calling VoxelRenderer::End(void).");
		//	End();
		//} else s_bSceneBegan = true;

		//data->shader->Bind();
		//data->shader->SetMat4("u_ViewProjection", camera->GetVPM());
		//data->shader->SetMat4("u_Transform", glm::mat4(1.0f));
	}
	void VoxelRenderer::DrawVoxel(const Voxel& params)
	{
		VoxelVertex& p = data->myBuffer[data->pIndex++];
		const Voxel& v = params;
		//p.data = VoxelVertex::Compress(
		//	v.position.x, v.position.y, v.position.z,
		//	v.color.r, v.color.g, v.color.b, v.color.a,
		//	params.visibleFaces
		//);
		//p.data.x = 0x000000FF;
		//p.data.y = 0xFFFFFFFF;
		//KROSS_TRACE("C={0} - D={1}", p.ReadCompressed(), p.ReadDecompressed());

		p.pos = params.position;
		p.color = params.color;
	}
	void VoxelRenderer::End()
	{
		if (s_bSceneBegan) s_bSceneBegan = false;
		else KROSS_WARN("Calling VoxelRenderer::End(void) without calling VoxelRenderer::Begin(Camera::Camera&). Did you forget to Begin the Scene?");
	}
	void VoxelRenderer::SwitchShader(const Ref<Shader>& shader)
	{
		if (shader) {
			data->shader->unBind();
			data->shader = shader;
			data->shader->Bind();
		} else {
			KROSS_WARN("Shader provided is empty, or not supported."); return;
		}
	}
}

