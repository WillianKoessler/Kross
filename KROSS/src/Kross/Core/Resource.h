#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	class KAPI Resource
	{
		const char* m_strName;
	public:
		Resource();
		~Resource();
		const char* GetName() const { return m_strName; }
		void SetName(const char *name) { m_strName = name; }
		//template<class T, typename...Args>
		//const char* SetName(Args&&...args)
		//{
		//	(append(m_strName, typeid(T).name(), args), ...);
		//	return m_strName;
		//}
	};
}