#pragma once

#include "Kross/Renderer/Cameras/Cameras/Perspective.h"
#include "Kross/Renderer/Cameras/Controller.h"

#include "Kross/Core/Input.h"
#include "Kross/Core/KeyCodes.h"
#include "Kross/Core/Application.h"


namespace Kross::Camera3D {
	class FirstPerson :
		public Kross::Camera::Controller
	{
	public:
		FirstPerson(const std::string& name, float aspectRatio, float fov = 90.0f)
			: m_fAR(aspectRatio),
			m_fFOV(fov),
			mouseRotation(glm::vec3(0.0f, 0.0f, 1.0f))
		{
			KROSS_PROFILE_FUNC();
			if (camera)
				KROSS_CORE_WARN("[{0}] WARNING: Overriding previous camera: {1}", __FUNCTION__, camera->GetName());

			camera = makeRef<Perspective>(m_fAR, m_fFOV, name);
			KROSS_CORE_INFO("[{0}] Camera Controller Created", __FUNCTION__);
		}
		FirstPerson(Camera::Camera* pcamera, float aspectRatio, float fov = 90.0f)
			: m_fAR(aspectRatio),
			m_fFOV(fov),
			mouseRotation(glm::vec3(0.0f, 0.0f, 1.0f))
		{
			KROSS_PROFILE_FUNC();
			if (pcamera)
				KROSS_CORE_WARN("[{0}] WARNING: Overriding previous {2} Camera: {1}", __FUNCTION__, camera->GetName(), camera->GetType());

			camera.reset(pcamera);
			KROSS_CORE_INFO("[{0}] Camera Controller Created", __FUNCTION__);
		}
		~FirstPerson()
		{
			KROSS_PROFILE_FUNC();
			KROSS_CORE_INFO("[{0}] Camera Controller Deconstructed", __FUNCTION__);
		}



		virtual void OnUpdate(double ts) override
		{
			KROSS_PROFILE_FUNC();
			if (Input::IsKeyReleased(KROSS_KEY_SPACE))
				m_bCursorEnabled = !m_bCursorEnabled;

			if (!m_bCursorEnabled) camera->unLock();
			else camera->LockAt(glm::vec3(0.0f));

			Application::Get().GetWindow().CursorEnabled(m_bCursorEnabled);
			glm::vec3& position = const_cast<glm::vec3&>(camera->GetPosition());
			float speed = m_fSpeed * (float)ts;

			const glm::mat4& vpm = camera->GetVPM();
			glm::vec3 forward = glm::normalize(glm::vec3(vpm[0][2], vpm[1][2], vpm[2][2])) * speed;
			glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))) * speed;

			if (Input::IsKeyHeld(KROSS_KEY_W)) position += forward;
			if (Input::IsKeyHeld(KROSS_KEY_S)) position -= forward;
			if (Input::IsKeyHeld(KROSS_KEY_A)) position -= right;
			if (Input::IsKeyHeld(KROSS_KEY_D)) position += right;
			if (Input::IsKeyHeld(KROSS_KEY_E)) position.y += speed;
			if (Input::IsKeyHeld(KROSS_KEY_Q)) position.y -= speed;
			camera->SetPosition(position);
			camera->SetRotation(mouseRotation);
			CalculateView();
		}

		virtual void OnEvent(Event& e) override
		{
			KROSS_PROFILE_FUNC();
			EventDispatcher dispatcher(e);
			dispatcher.Dispatch<WindowResizeEvent>(KROSS_BIND_EVENT_FN(FirstPerson::OnWindowResized));
			dispatcher.Dispatch<MouseMovedEvent>(KROSS_BIND_EVENT_FN(FirstPerson::OnMouseMoved));
		}

		virtual Ref<Kross::Camera::Camera> GetCamera() override
		{
			return camera;
		}

		virtual const Ref<Kross::Camera::Camera> GetCamera() const override
		{
			return camera;
		}


		virtual void Zoom(float val) override {};
	private:
		virtual void CalculateView() override
		{
			camera->SetProjMat(m_fFOV, m_fAR, 0.01f, 1000.0f);
		}
	private:
		virtual bool OnWindowResized(WindowResizeEvent& e) override
		{
			KROSS_PROFILE_FUNC();
			m_fAR = (float)e.GetWidth() / (float)e.GetHeight();
			CalculateView();
			return false;
		}		virtual bool OnMouseScrolled(MouseScrolledEvent& e) override { return false; };
		virtual bool OnMouseMoved(MouseMovedEvent& e) override
		{
			KROSS_PROFILE_FUNC();
			if (m_bCursorEnabled) return false;

			static glm::vec2 last = glm::vec2(0.0f);
			glm::vec2 current = glm::vec2(e.GetX(), e.GetY());
			glm::vec2 size = glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

			if (current == last) return false;

			glm::vec2 now = (current - last) * 0.5f;

			mouseRotation.y += now.x;
			mouseRotation.x += now.y;

			if (mouseRotation.x > m_fYbound) mouseRotation.x = m_fYbound;
			else if (mouseRotation.x < -m_fYbound) mouseRotation.x = -m_fYbound;

			if (mouseRotation.y > 360.0f) mouseRotation.y = 0.0f;
			else if (mouseRotation.y < 0.0f) mouseRotation.y = 360.0f;

			//KROSS_CORE_TRACE("MouseMoved {0}, {1}", mouseRotation.y, mouseRotation.x);

			last = current;
			return false;
		}
	private:
		float m_fYbound = 89.0f;
		float m_fSpeed = 50.0f;
		float m_fAR;
		float m_fFOV;
		glm::vec3 mouseRotation;
		bool m_bCursorEnabled = false;
	};
}
