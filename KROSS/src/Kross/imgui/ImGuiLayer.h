#pragma once

#include "Kross/Core/Layer.h"
#include "Kross/Events/KeyEvent.h"
#include "Kross/Events/MouseEvent.h"
#include "Kross/Events/ApplicationEvent.h"

namespace Kross {
	class KAPI ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender(double ts) override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}
