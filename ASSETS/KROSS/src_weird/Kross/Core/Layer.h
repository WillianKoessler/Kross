#pragma once

#include "Core.h"
#include "Timestep.h"
#include "Kross/Events/Event.h"

namespace Kross {
	class KAPI Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();


		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(double fElapsedTime) {}
		virtual void OnImGuiRender(double fElapsedTime) {}
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_strDebugName; }

		float ar;
	protected:
		std::string m_strDebugName;
	};
}