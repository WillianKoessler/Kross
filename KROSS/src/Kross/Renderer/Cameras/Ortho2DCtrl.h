#pragma once

#include "Ortho2D.h"
#include "Controller.h"

namespace Kross::Camera {
	class Ortho2DCtrl :
		public Controller
	{
	public:
		Ortho2DCtrl(float AR = 1.0f, bool rot = false, const std::string& name = "default");
		Ortho2DCtrl(bool rot, const std::string& name = "default", float AR = 1.0f);
		Ortho2DCtrl(const std::string& name, float AR = 1.0f, bool rot = false);

		Ortho2DCtrl(Ortho2D* camera, float AR = 1.0f, bool rot = false);
		~Ortho2DCtrl();

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;
		virtual bool Input(std::function<bool(int)>) override;
		virtual void DebugWindow() override;

		virtual Ref<Camera> GetCamera() override;
		virtual const Ref<Camera> GetCamera() const override;
		virtual const float GetZoom() const override { return m_fZoom; }

	private:
		virtual bool OnWindowResized(WindowResizeEvent& e) override;
		virtual bool OnMouseScrolled(MouseScrolledEvent& e) override;
	public:
		float m_fAR;
		float m_fZoomRate = -0.1f;
		float m_fZoom = 1.0f;
		float fCameraMoveSpeed = 5.0f;
		float fCameraRotation = 0.0f;
		int fCameraRotationSpeed = 180;
		bool m_bRotation;
		glm::uvec2 size = { 0, 0 };
		glm::ivec3 move = { 0, 0, 0 };

		glm::vec3 v3cameraPos = { 0.0f, 0.0f, 0.0f };

		Ref<Ortho2D> m_Camera;
	};
}
