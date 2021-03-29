#pragma once

#include "Ortho2D.h"
#include "Controller.h"

namespace Kross::Camera {
	class Ortho2DCtrl :
		public Controller
	{
	public:
		Ortho2DCtrl(const std::string& name, float AR = 1.0f, bool rot = false);
		Ortho2DCtrl(Ortho2D* camera, float AR = 1.0f, bool rot = false);
		~Ortho2DCtrl();

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;

		virtual Ref<Camera> GetCamera() override;
		virtual const Ref<Camera> GetCamera() const override;

		virtual void Zoom(float val) override;
	private:
		virtual bool OnWindowResized(WindowResizeEvent& e) override;
		virtual bool OnMouseScrolled(MouseScrolledEvent& e) override;
		virtual void CalculateView() override;
	private:
		float m_fAR;
		float m_fZoomRate = -0.1f;
		float m_fZoom = 1.0f;
		float fCameraMoveSpeed = 5.0f;
		float fCameraRotation = 0;
		float fCameraRotationSpeed = 180.0f;
		bool m_bRotation;

	};
}
