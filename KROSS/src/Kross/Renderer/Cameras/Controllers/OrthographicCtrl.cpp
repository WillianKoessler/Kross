#include "Kross_pch.h"
#include "OrthographicCtrl.h"

#include "Kross/Core/Input.h"
#include "Kross/Core/KeyCodes.h"

namespace Kross::Camera2D {

	OrthographicCtrl::OrthographicCtrl(Orthographic* cam, float AR, bool rot)
		: m_fAR(AR),
		m_bRotation(rot)
	{
		KROSS_PROFILE_FUNC();
		if (camera)
			KROSS_CORE_WARN("[{0}] WARNING: Overriding previous camera: {1}", __FUNCTION__, camera->GetName());

		camera.reset(cam);
		KROSS_CORE_INFO("[{0}] Camera Controller Created", __FUNCTION__);
	}

	OrthographicCtrl::OrthographicCtrl(const char* name, float AR, bool rot)
		: m_fAR(AR),
		m_bRotation(rot)
	{
		KROSS_PROFILE_FUNC();
		if (camera)
			KROSS_CORE_WARN("[{0}] WARNING: Overriding previous camera: {1}", __FUNCTION__, camera->GetName());

		camera = makeRef<Orthographic>(-AR * m_fZoom, AR * m_fZoom, -m_fZoom, m_fZoom, name);
		KROSS_CORE_INFO("[{0}] Camera Controller Created", __FUNCTION__);
	}

	OrthographicCtrl::~OrthographicCtrl()
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[Kross::Camera::OrthographicCtrl] Camera Controller Deconstructed");
	}

	void OrthographicCtrl::OnUpdate(double ts)
	{
		KROSS_PROFILE_FUNC();
		glm::vec3& position = const_cast<glm::vec3&>(camera->GetPosition());
		if (Input::IsKeyHeld(KROSS_KEY_W)) position.y += fCameraMoveSpeed * (float)ts;
		if (Input::IsKeyHeld(KROSS_KEY_S)) position.y -= fCameraMoveSpeed * (float)ts;
		if (Input::IsKeyHeld(KROSS_KEY_A)) position.x -= fCameraMoveSpeed * (float)ts;
		if (Input::IsKeyHeld(KROSS_KEY_D)) position.x += fCameraMoveSpeed * (float)ts;
		camera->SetPosition(position);

		if (Input::IsKeyHeld(KROSS_KEY_M)) m_fZoomRate *= 1.1f;
		if (Input::IsKeyHeld(KROSS_KEY_N)) m_fZoomRate *= 0.9f;

		if (m_fZoomRate > -0.001f) m_fZoomRate = -0.001f;

		if (Input::IsKeyHeld(KROSS_KEY_I)) m_fZoom += m_fZoomRate;
		if (Input::IsKeyHeld(KROSS_KEY_O)) m_fZoom -= m_fZoomRate;

		m_fZoom = std::max(m_fZoom, 0.005f);

		if (m_bRotation) {
			if (Input::IsKeyHeld(KROSS_KEY_Q)) fCameraRotation += fCameraRotationSpeed * (float)ts;
			if (Input::IsKeyHeld(KROSS_KEY_E)) fCameraRotation -= fCameraRotationSpeed * (float)ts;
		camera->SetRotation(fCameraRotation, Kross::Camera::Camera::Axis::Z);
		}

		fCameraMoveSpeed = m_fZoom * 1.2f;
		
		CalculateView();
	}
	
	void OrthographicCtrl::OnEvent(Event & e)
	{
		KROSS_PROFILE_FUNC();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(KROSS_BIND_EVENT_FN(OrthographicCtrl::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(KROSS_BIND_EVENT_FN(OrthographicCtrl::OnMouseScrolled));
	}

	Ref<Kross::Camera::Camera> OrthographicCtrl::GetCamera()
	{
		return camera;
	}

	const Ref<Kross::Camera::Camera> OrthographicCtrl::GetCamera() const
	{
		return camera;
	}

	void OrthographicCtrl::Zoom(float val)
	{
		m_fZoom = std::clamp<float>(val, 0.05f, 100.0f);
		CalculateView();
	}

	bool OrthographicCtrl::OnWindowResized(WindowResizeEvent & e)
	{
		KROSS_PROFILE_FUNC();
		m_fAR = (float)e.GetWidth() / (float)e.GetHeight();
		CalculateView();
		return false;
	}

	bool OrthographicCtrl::OnMouseScrolled(MouseScrolledEvent& e)
	{
		KROSS_PROFILE_FUNC();
		Zoom(e.GetYOffSet() * m_fZoom * 0.01f + m_fZoom);
		return false;
	}

	bool OrthographicCtrl::OnMouseMoved(MouseMovedEvent& e)
	{
		KROSS_PROFILE_FUNC();
		return false;
	}

	void OrthographicCtrl::CalculateView()
	{
		bounds = { -m_fAR * m_fZoom, m_fAR * m_fZoom, -m_fZoom, m_fZoom };
		camera->SetProjMat(bounds.left, bounds.right, bounds.bottom, bounds.top);
	}
}