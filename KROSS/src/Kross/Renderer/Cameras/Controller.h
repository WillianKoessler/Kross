#pragma once

#include "Camera.h"
#include "Kross/Core/Core.h"
#include "Kross/Events/ApplicationEvent.h"
#include "Kross/Events/MouseEvent.h"
#include "Kross/Core/Timestep.h"


namespace Kross::Camera {
	class Controller
	{
	public:
		virtual void OnUpdate(Timestep ts) = 0;
		virtual void OnEvent(Event& e) = 0;

		virtual bool OnWindowResized(WindowResizeEvent& e) = 0;
		virtual bool OnMouseScrolled(MouseScrolledEvent& e) = 0;

		virtual Ref<Camera> GetCamera() = 0;
		virtual const Ref<Camera> GetCamera() const = 0;

		virtual void Zoom(float val) = 0;
	protected:
		virtual void CalculateView() = 0;

		struct Boundary {
			float left;
			float right;
			float bottom;
			float top;
		} bounds;

		Ref<Camera> camera;
		glm::vec3 position = { 0.0f, 0.0f, 0.0f };
	};
}