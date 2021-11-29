#include "Kross_pch.h"
#include "Layer.h"
#include "Application.h"

namespace Kross{
	Layer::Layer(const char* name)
		: m_strDebugName(name),
		ar((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight())
	{
		KROSS_CORE_INFO("Layer '{0}' Created", name);
	}

	Layer::~Layer()
	{
		KROSS_CORE_INFO("Layer '{0}' Destructed", m_strDebugName);
	}
}