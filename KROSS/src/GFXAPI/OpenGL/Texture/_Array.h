#pragma once

#include "GFXAPI/OpenGL/GLErrors.h"
#include "Kross/Core/Core.h"
#include "Kross/Renderer/Textures/Array.h"
#include <vector>

namespace Kross::OpenGL::Texture {
	class KAPI T2DArray final : public Kross::Texture::T2DArray
	{
	public:
		T2DArray(const size_t size);
		~T2DArray();

		virtual const unsigned int Get(const Ref<Kross::Texture::T2D> texture) override;
		virtual void Bind(const size_t slot = 0) const override;
		virtual void Add(const Ref<Kross::Texture::Base> texture) override;
		virtual void Del(const Ref<Kross::Texture::Base> texture) override;
		virtual void Del(const size_t index) override;

		virtual const size_t size() const override;
		virtual const size_t* IDs() const override;

		const Ref<Kross::Texture::T2D> operator[](const size_t pos) { if (textures[pos]->GetID()) return Ref<Kross::Texture::T2D>(textures[pos]); }
		operator const Ref<Kross::Texture::T2D>* () { return textures; }
	private:
		size_t nextAvailable = 0;
		size_t currentHead;
		const size_t _size;
		Ref<Kross::Texture::T2D>* textures;
		mutable unsigned int* glTex;
	};
}
