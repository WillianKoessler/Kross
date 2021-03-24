#pragma once

#include "Mesh.h"
#include "Kross/Util/NonCopyable.h"
#include "Kross/Util/Singleton.h"

#include "Kross/Renderer/Buffer.h"
#include "Kross/Renderer/VertexArray.h"

namespace Kross::Model {
	class Base : public NonCopyable
	{
	protected:
		Base(const Mesh& mesh);
		~Base();

		void addData(const Mesh& mesh);

		void deleteData();

		void addVBO(const Kross::Buffer::Vertex& data);
		void bindVAO() const;

		int getIndicesCount() const;

		void addEBO(const Kross::Buffer::Index& indices);

		Ref<VertexArray> m_vao = nullptr;
		int m_vboCount = 0;
		int m_indicesCount = 0;
		std::vector<uint32_t> m_buffers;
	};
}
