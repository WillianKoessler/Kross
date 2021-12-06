#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Resource.h"
#include "Kross/Renderer/Buffer.h"

namespace Kross {
	class KAPI VertexArray : public Resource
	{
	public:
		virtual ~VertexArray() = default;

		virtual void Bind() const = 0;
		virtual void unBind() const = 0;

		virtual void AddVoxel(const Ref<Kross::Buffer::Vertex>& voxel) = 0;
		virtual void AddVertex(const Ref<Kross::Buffer::Vertex>& vertex) = 0;
		virtual void SetIndex(const Ref<Kross::Buffer::Index>& index) = 0;

		virtual const std::vector<Ref<Buffer::Vertex>>& GetVertex() = 0;
		virtual const Ref<Buffer::Index>& GetIndex() = 0;

		static Ref<VertexArray> CreateRef(const char* name);
		static Scope<VertexArray> CreateScope(const char* name);
	};
}