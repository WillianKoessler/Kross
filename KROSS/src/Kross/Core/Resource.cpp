#include "Kross_pch.h"
#include "Resource.h"

#define _CRT_SECURE_NO_WARNINGS
namespace Kross {
	Resource::~Resource()
	{
		KROSS_INFO("'{0}' Destroyed", GetName());
	}
	void Resource::SetName(const char* name)
	{
		if (name != nullptr) {
			if (m_strName != nullptr) {
				if (strcmp(name, m_strName) == 0) return;
				delete m_strName;
			}
			size_t size = strlen(name) + 1;
			m_strName = new char[size];
			memset(m_strName, 0, size);
			strcpy_s(m_strName, size, name);
		} else {
			if (m_strName) delete m_strName;
		}
	}

}
#undef _CRT_SECURE_NO_WARNINGS
