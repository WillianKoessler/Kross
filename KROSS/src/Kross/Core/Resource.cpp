#include "Kross_pch.h"
#include "Resource.h"

#define _CRT_SECURE_NO_WARNINGS
namespace Kross {
	Resource::Resource()
		: m_strName("Unnamed_Resource")
	{
		//m_strName = new char[1];
		//m_strName[0] = '\0';
		//SetName<Resource>("");
	}
	Resource::~Resource()
	{
		KROSS_INFO("'{0}' Destroyed", GetName());
	}
}
#undef _CRT_SECURE_NO_WARNINGS