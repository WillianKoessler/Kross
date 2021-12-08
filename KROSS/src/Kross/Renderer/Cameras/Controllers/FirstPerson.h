#pragma once

#include "Kross/Renderer/Cameras/Cameras/Perspective.h"
#include "Kross/Renderer/Cameras/Controller.h"

#include "Kross/Core/Input.h"
#include "Kross/Events/KeyCodes.h"
#include "Kross/Core/Application.h"


namespace Kross::Camera3D {
	class FirstPerson :
		public Kross::Camera::Controller
	{
	public:
		FirstPerson(const char* name, float aspectRatio, float fov = 90.0f);
		FirstPerson(const char* name, Ref<Camera::Camera> pcamera, float fov = 90.0f);
		~FirstPerson() { delete[] name; }
		virtual void OnUpdate(double ts) override;
		virtual void OnEvent(Event& e) override;

		virtual Ref<Kross::Camera::Camera> GetCamera() override;
		virtual const Ref<Kross::Camera::Camera> GetCamera() const override;

		virtual void Zoom(float val) override;
	private:
		virtual bool OnWindowResized(WindowResizeEvent& e) override;
		virtual bool OnMouseMoved(MouseMovedEvent& e) override;
		virtual bool OnMouseScrolled(MouseScrolledEvent& e) override;
	private:
		float m_fYbound = 89.0f;
		float m_fSpeed = 50.0f;
		float m_fFOV;
		glm::vec3 mouseRotation;
		bool m_bCursorEnabled = true;
		char *name;
//		glm::vec3 forward, right, up;
	};
}
