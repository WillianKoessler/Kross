#pragma once

#include "Kross/Renderer/Cameras/Cameras/Perspective.h"
#include "Kross/Renderer/Cameras/Controller.h"

namespace Kross::Camera3D {
	class FirstPerson :
		public Kross::Camera::Controller
	{
	public:
		FirstPerson(const char* name, float aspectRatio, float fov = 90.0f);
		FirstPerson(Camera::Camera* camera, float aspectRatio, float fov = 90.0f);
		~FirstPerson();

		static glm::vec3 m_v3Rot;

		virtual void OnUpdate(double ts) override;
		virtual void OnEvent(Event& e) override;

		virtual Ref<Kross::Camera::Camera> GetCamera() override;
		virtual const Ref<Kross::Camera::Camera> GetCamera() const override;

		virtual void Zoom(float val) override {};
	private:
		virtual void CalculateView() override;

	private:
		virtual bool OnWindowResized(WindowResizeEvent& e) override;
		virtual bool OnMouseScrolled(MouseScrolledEvent& e) override { return false; };
		virtual bool OnMouseMoved(MouseMovedEvent& e) override;
	private:
		float m_fYbound = 89.0f;
		float m_fSpeed = 50.0f;
		float m_fAR;
		float m_fFOV;
		glm::vec3 mouseRotation;
		bool m_bCursorEnabled = false;
	};
}
