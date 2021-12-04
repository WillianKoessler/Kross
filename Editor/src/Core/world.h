#pragma once
#include "Kross.h"

namespace Kross {
	class world : public Layer
	{
		// main camera
		Ref<Camera::Controller> m_Camera;
		Ref<FrameBuffer> m_Framebuffer;

		Ref<Scene> m_Scene;
		Entity m_SquareEntity;
		Entity m_CameraEntity;
		QuadParams params;

	public:
		world();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnUpdate(double) override;
		virtual void OnEvent(Event& event) override;
	};
}
