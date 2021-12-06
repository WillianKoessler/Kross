#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Resource.h"

namespace Kross {
	class KAPI Layer : public Resource
	{
	public:
		Layer(const char* name);

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(double fElapsedTime) {}
		virtual void OnImGuiRender(double fElapsedTime) {}
		virtual void OnEvent(class Event& event) {}
	};
}