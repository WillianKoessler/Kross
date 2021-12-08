#pragma once

#include "Kross/Renderer/Cameras/Cameras/Orthographic.h"
#include "Kross/Renderer/Cameras/Controller.h"

namespace Kross::Camera2D {
	class OrthographicCtrl :
		public Kross::Camera::Controller
	{
	public:
		OrthographicCtrl(const char* name, float AR = 1.0f, bool rot = false);
		OrthographicCtrl(const char* name, Ref<Camera::Camera> camera, float AR = 1.0f, bool rot = false);

		virtual void OnUpdate(double ts) override;
		virtual void OnEvent(Event& e) override;

		virtual Ref<Kross::Camera::Camera> GetCamera() override;
		virtual const Ref<Kross::Camera::Camera> GetCamera() const override;

		virtual void Zoom(float val) override;
	private:
		virtual bool OnWindowResized(WindowResizeEvent& e) override;
		virtual bool OnMouseScrolled(MouseScrolledEvent& e) override;
		virtual bool OnMouseMoved(MouseMovedEvent& e) override;
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
