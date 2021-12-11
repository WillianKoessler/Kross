#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Resource.h"

namespace Kross {
	class KAPI Layer : public Resource
	{
		bool m_bPassEvents = true;
	public:
		Layer(const char* name);

		bool IsPassingEvents() const { return m_bPassEvents; }
		void PassEvents(bool pass) { m_bPassEvents = pass; }
		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(double fElapsedTime) {}
		virtual void OnImGuiRender(double fElapsedTime) {}
		virtual void OnEvent(class Event& event) {}
	};
}