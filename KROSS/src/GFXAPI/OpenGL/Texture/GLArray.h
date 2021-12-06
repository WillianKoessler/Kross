#pragma once

#include "GFXAPI/OpenGL/GLErrors.h"
#include "Kross/Core/Core.h"
#include "Kross/Renderer/Textures/Array.h"
#include <vector>

namespace Kross::OpenGL::Texture {
	class T2DArray final : public Kross::Texture::T2DArray
	{
		class Entry
		{
			int id;
			Ref<Kross::Texture::T2D> resource;
		public:
			Entry(const Ref<Kross::Texture::T2D>& r) : id(r ? r->GetID() : -1), resource(r) {}
			Entry &operator=(const Entry &other) { this->id = other.id; this->resource = other.resource; return *this; }

			bool validate() const { return resource != nullptr; }

			int ID() const { return id; }
			const Ref<Kross::Texture::T2D>& get() const { return resource; }
			Ref<Kross::Texture::T2D> operator->() { return resource; }

			bool operator <= (Entry& other) { return id <= other.id; }
			bool operator <= (int other) { return id <= other; }
			bool operator > (int other) { return id > other; }

			operator const Ref<Kross::Texture::T2D>& () { return resource; }
			operator int() { return id; }
			operator bool() { return resource != nullptr; }
		};
	public:
		T2DArray(const char* name, const size_t size);
		~T2DArray();

		virtual const int Get(const Ref<Kross::Texture::T2D>& texture) override;
		virtual void Bind(const size_t slot = 0) override;
		virtual void Add(const Ref<Kross::Texture::T2D> texture) override;
		virtual void Del(const Ref<Kross::Texture::T2D> texture) override;
		virtual void Del(const size_t index) override;

		virtual const size_t size() const override;
		virtual const size_t maxSize() const override;
		virtual const unsigned int* IDs() const override;

		const Ref<Kross::Texture::T2D> operator[](const size_t pos) { return textures[pos].get(); }
		//operator const Ref<Kross::Texture::T2D>* () { return textures.data(); }
	private:
		size_t nextAvailable = 0;
		size_t iterator;
		const size_t _size;
		std::vector<Entry> textures;
		int* glTex;

		std::vector<Entry>::const_iterator location(int id)
		{
			return std::lower_bound(textures.begin(), textures.end(), id, [](const Entry& stored, int id) { if (stored.get()) return stored.ID() > id; return true; });
		}
	};
}
