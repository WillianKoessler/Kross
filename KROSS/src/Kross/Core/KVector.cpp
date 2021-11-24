#include <Kross_pch.h>
#include "KVector.h"

#include <vector>

namespace Kross {
	template<typename T>
	inline KVector<T>::KVector(KVector<T>&& other)
		: m_pData(nullptr), m_uDataSize(other.m_uDataSize)
	{
		Reallocate(m_uDataSize);
		memmove_s(m_pData, m_uDataSize, other.m_pData, other.m_uDataSize);
	}
	template<typename T>
	inline KVector<T>::KVector(const KVector<T>&)
		: m_pData(nullptr), m_uDataSize(other.m_uDataSize)
	{
		Reallocate(m_uDataSize);
		memmove_s(m_pData, m_uDataSize, other.m_pData, other.m_uDataSize);
	}
	template<typename T>
	void KVector<T>::Reallocate(uint64_t size)
	{
		m_pData = (T*)realloc(m_pData, size);
	}
}