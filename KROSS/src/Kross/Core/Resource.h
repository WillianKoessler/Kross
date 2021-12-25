#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	class KAPI Resource
	{
		char* m_strName = nullptr;
	public:
		~Resource();
		const char* GetName() const { return m_strName; }
		void SetName(const char *name);
	};
}
