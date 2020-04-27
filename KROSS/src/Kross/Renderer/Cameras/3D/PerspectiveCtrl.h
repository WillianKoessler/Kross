#pragma once

#include "Perspective.h"
#include "Kross/Renderer/Cameras/Controller.h"

namespace Kross::Camera {
	class Persp3DCtrl :
		public Controller
	{
	public:
		Persp3DCtrl(float AR, float FOV, const std::string& name = "default");
		Persp3DCtrl(Persp3D* camera, uint32_t width, uint32_t height);
		~Persp3DCtrl();

		virtual void OnUpdate(Timestep ts) override;
		virtual void OnEvent(Event& e) override;
		virtual bool Input(std::function<bool(int)>) override;
		virtual void DebugWindow() override;

		virtual Ref<Camera> GetCamera() override;
		virtual const Ref<Camera> GetCamera() const override;

	private:
		virtual bool OnWindowResized(WindowResizeEvent& e) override;
		virtual bool OnMouseScrolled(MouseScrolledEvent& e) override;
		
		float m_fFOV;
		float m_fAR;
		const float m_fNear;
		const float m_fFar;

		float fCameraMoveSpeed = 1.0f;

		float fCameraRotation = 0.0f;
		int nCameraRotateAlongZAxis = 0;
		float fCameraRotationSpeed = 180.0f;
		float maxRotationSpeed = 2.0f;

		glm::uvec2 size = { 0, 0 };
		glm::ivec3 move = glm::ivec3(0);
		glm::vec2 mouseMove = glm::vec2(0.0f);

		glm::vec3 rotate_at = glm::vec3(0.0f);
		glm::vec3 lookat = glm::vec3(0.0f);

		glm::vec3 front = { 0.0f, 0.0f, 1.0f };
		glm::vec3 up = { 0.0f, 1.0f, 0.0f };
		glm::vec3 right = { 1.0f, 0.0f, 0.0f };

		// vector of angles in respective axis (x turn on X axis, y turn on Y axis)
		glm::vec3 dir_input = glm::vec3(0.0f);
		glm::vec3 dir = { 0.0f, 0.0f, 1.0f };

		glm::vec3 v3cameraPos = { 0.0f, 0.0f, -1.0f };

		Ref<Persp3D> m_Camera;
	};
}
