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

		inline static Camera* GetSelf() { return self; }
		inline static void SetSelf(Camera* other) { self = other; }
	private:
		static Camera* self;
		
	};
}