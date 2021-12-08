#pragma once

#include "Camera.h"
#include "Kross/Core/Core.h"
#include "Kross/Core/Resource.h"
#include "Kross/Events/ApplicationEvent.h"
#include "Kross/Events/MouseEvent.h"


namespace Kross::Camera {
	class Controller : public Resource
	{
	public:
		Controller() { SetName("Unnamed_CameraController"); }

		virtual void OnUpdate(double ts) = 0;
		virtual void OnEvent(Event& e) = 0;

		virtual bool OnWindowResized(WindowResizeEvent& e) = 0;
		virtual bool OnMouseScrolled(MouseScrolledEvent& e) = 0;
		virtual bool OnMouseMoved(MouseMovedEvent& e) = 0;

		virtual Ref<Camera> GetCamera() = 0;
		virtual const Ref<Camera> GetCamera() const = 0;

		virtual void Zoom(float val) = 0;
	protected:

		struct Boundary {
			float left = 0.0f;
			float right = 1.0f;
			float bottom = 1.0f;
			float top = 0.0f;
		} bounds{};

		Ref<Camera> camera;
	};
}