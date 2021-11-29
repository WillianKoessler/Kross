#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	class KAPI Layer
	{
	public:
		Layer(const char* name = "Layer");
		virtual ~Layer();


		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(double fElapsedTime) {}
		virtual void OnImGuiRender(double fElapsedTime) {}
		virtual void OnEvent(class Event& event) {}

		inline const char* GetName() const { return m_strDebugName; }

		float ar;
	protected:
		const char* m_strDebugName;
	};
}