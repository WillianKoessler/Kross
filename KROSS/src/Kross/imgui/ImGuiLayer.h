#pragma once

#include "Kross/Core/Layer.h"
#include "Kross/Events/KeyEvent.h"
#include "Kross/Events/MouseEvent.h"
#include "Kross/Events/ApplicationEvent.h"

namespace Kross {
	class KAPI ImGuiLayer : public Layer
	{
		bool m_bBlockEvents = false;
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual bool IsBlockingEvents() const { return m_bBlockEvents; }
		virtual void BlockEvents(bool block) { m_bBlockEvents = block; }
		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnEvent(Event &e) override;
		virtual void OnGuiRender(double ts) override;

		void Begin();
		void End();

		void SetDarkTheme();
	};
}
