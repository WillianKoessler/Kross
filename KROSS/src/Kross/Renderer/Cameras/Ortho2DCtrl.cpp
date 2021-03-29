#include "Kross_pch.h"
#include "Ortho2DCtrl.h"

#include "Kross/Core/Input.h"
#include "Kross/Core/KeyCodes.h"

namespace Kross::Camera {

	Ortho2DCtrl::Ortho2DCtrl(Ortho2D* cam, float AR, bool rot)
		: m_fAR(AR),
		m_bRotation(rot)
	{
		KROSS_PROFILE_FUNC();
		if (camera)
			KROSS_CORE_WARN("[{0}] WARNING: Overriding previous camera: {1}", __FUNCTION__, camera->GetName());

		camera.reset(cam);
		KROSS_CORE_INFO("[{0}] Camera Controller Created", __FUNCTION__);
	}

	Ortho2DCtrl::Ortho2DCtrl(const std::string& name, float AR, bool rot)
		: m_fAR(AR),
		m_bRotation(rot)
	{
		KROSS_PROFILE_FUNC();
		if (camera)
			KROSS_CORE_WARN("[{0}] WARNING: Overriding previous camera: {1}", __FUNCTION__, camera->GetName());

		camera = makeRef<Ortho2D>(-AR * m_fZoom, AR * m_fZoom, -m_fZoom, m_fZoom, name);
		KROSS_CORE_INFO("[{0}] Camera Controller Created", __FUNCTION__);
	}

	Ortho2DCtrl::~Ortho2DCtrl()
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[Kross::Camera::Ortho2DCtrl] Camera Controller Deconstructed");
	}

	void Ortho2DCtrl::OnUpdate(Timestep ts)
	{
		KROSS_PROFILE_FUNC();
		if (Input::IsKeyPressed(KROSS_KEY_W)) position.y += fCameraMoveSpeed * ts;
		if (Input::IsKeyPressed(KROSS_KEY_S)) position.y -= fCameraMoveSpeed * ts;
		if (Input::IsKeyPressed(KROSS_KEY_A)) position.x -= fCameraMoveSpeed * ts;
		if (Input::IsKeyPressed(KROSS_KEY_D)) position.x += fCameraMoveSpeed * ts;
		camera->SetPosition(position);

		if (Input::IsKeyPressed(KROSS_KEY_M)) m_fZoomRate *= 1.1f;
		if (Input::IsKeyPressed(KROSS_KEY_N)) m_fZoomRate *= 0.9f;

		if (m_fZoomRate > -0.001f) m_fZoomRate = -0.001f;

		if (Input::IsKeyPressed(KROSS_KEY_I)) m_fZoom += m_fZoomRate;
		if (Input::IsKeyPressed(KROSS_KEY_O)) m_fZoom -= m_fZoomRate;

		m_fZoom = std::max(m_fZoom, 0.005f);

		if (m_bRotation) {
			if (Input::IsKeyPressed(KROSS_KEY_Q)) fCameraRotation += fCameraRotationSpeed * ts;
			if (Kross::Input::IsKeyPressed(KROSS_KEY_E)) fCameraRotation -= fCameraRotationSpeed * ts;
		camera->SetRotation(fCameraRotation);
		}

		fCameraMoveSpeed = m_fZoom * 1.2f;
		
		CalculateView();
	}
	
	void Ortho2DCtrl::OnEvent(Event & e)
	{
		KROSS_PROFILE_FUNC();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(KROSS_BIND_EVENT_FN(Ortho2DCtrl::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(KROSS_BIND_EVENT_FN(Ortho2DCtrl::OnMouseScrolled));
	}

	Ref<Camera> Ortho2DCtrl::GetCamera()
	{
		return camera;
	}

	const Ref<Camera> Ortho2DCtrl::GetCamera() const
	{
		return camera;
	}

	void Ortho2DCtrl::Zoom(float val)
	{
		m_fZoom = std::clamp<float>(val, 0.05f, 100.0f);
		CalculateView();
	}

	bool Ortho2DCtrl::OnWindowResized(WindowResizeEvent & e)
	{
		KROSS_PROFILE_FUNC();
		m_fAR = (float)e.GetWidth() / (float)e.GetHeight();
		CalculateView();
		return false;
	}
	
	bool Ortho2DCtrl::OnMouseScrolled(MouseScrolledEvent & e)
	{
		KROSS_PROFILE_FUNC();
		Zoom(e.GetYOffSet() * m_fZoom * 0.01f + m_fZoom);
		return false;
	}
	void Ortho2DCtrl::CalculateView()
	{
		bounds = { -m_fAR * m_fZoom, m_fAR * m_fZoom, -m_fZoom, m_fZoom };
		camera->SetProjMat(bounds.left, bounds.right, bounds.bottom, bounds.top);
	}
}