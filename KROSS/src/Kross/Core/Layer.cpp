#include "Kross_pch.h"
#include "Layer.h"
#include "Application.h"

namespace Kross{
	Layer::Layer(const char* name)
	{
		SetName(name);
		KROSS_CORE_INFO("'{0}' Constructed", GetName());
	}
}