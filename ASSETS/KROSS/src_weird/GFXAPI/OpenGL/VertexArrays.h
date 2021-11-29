#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/VertexArray.h"
#include "Kross/Renderer/Buffer.h"

namespace Kross::OpenGL {
	class KAPI VertexArray final : public Kross::VertexArray
	{
	public:
		VertexArray();
		~VertexArray();

		virtual void Bind() const override;
		virtual void unBind() const override;

		virtual void AddVoxel(const Ref<Kross::Buffer::Vertex>& voxel) override;
		virtual void AddVertex(const Ref<Kross::Buffer::Vertex>& vertex) override;
		virtual void SetIndex(const Ref<Kross::Buffer::Index>& index) override;

		virtual const std::vector<Ref<Buffer::Vertex>>& GetVertex() { return m_vecVertex; }
		virtual const Ref<Buffer::Index>& GetIndex() { return m_Index; }
	private:
		std::vector<Ref<Buffer::Vertex>> m_vecVertex;
		Ref<Buffer::Index> m_Index;
		uint32_t m_RendererID;
	};
}
