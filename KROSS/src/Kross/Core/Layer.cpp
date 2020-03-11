#include "Kross_pch.h"
#include "Layer.h"
#include "Application.h"

namespace Kross{
	Layer::Layer(const std::string & name)
		: m_strDebugName(name),
		ar((float)Application::Get().GetWindow().GetWidth() / (float)Application::Get().GetWindow().GetHeight())
	{
		KROSS_CORE_INFO("[Kross::Layer] Layer '{0}' Created", name);
	}

	Layer::~Layer()
	{
		//__debugbreak();
		KROSS_CORE_INFO("[Kross::Layer] Layer '{0}' Destructed", m_strDebugName);
	}
}