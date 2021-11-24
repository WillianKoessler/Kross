#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	template<typename T>
	class KAPI KVector
	{
		T* m_pData = nullptr;
		uint64_t m_uDataSize = 0u;
		const uint64_t m_uDataStride = sizeof(T);
	public:
		KVector() = default;
		KVector(KVector<T>&&);
		KVector(const KVector<T>&);



	private:
		inline void Reallocate(uint64_t size);
	};

	template<typename T>
	class KAPI KVector_iterator
	{
	public:

	};
}