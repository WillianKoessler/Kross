#include "Kross_pch.h"
#include "Ortho2DCtrl.h"

#include "Kross/Core/Input.h"
#include "Kross/Core/KeyCodes.h"

#include "imgui.h"

namespace Kross::Camera {

	Ortho2DCtrl::Ortho2DCtrl(Ortho2D* camera, float AR, bool rot)
		: m_fAR(AR),
		m_bRotation(rot),
		m_Camera(camera)
	{}

	Ortho2DCtrl::Ortho2DCtrl(float AR, bool rot, const std::string& name)
		: m_fAR(AR),
		m_bRotation(rot),
		m_Camera(makeRef<Ortho2D>(-AR * m_fZoom, AR* m_fZoom, -m_fZoom, m_fZoom, name))
	{
		KROSS_PROFILE_FUNC();
		if (GetSelf())
		{
			KROSS_CORE_WARN("[ {0} ] |||| WARNING: Overriding previous camera: {1}", __FUNCTION__, GetSelf()->GetName());
			SetSelf(m_Camera.get());
		}
		KROSS_CORE_INFO("[ {0} ] |||| Camera Controller Created", __FUNCTION__);
	}

	Ortho2DCtrl::Ortho2DCtrl(const std::string& name, float AR, bool rot)
		: m_fAR(AR),
		m_bRotation(rot),
		m_Camera(makeRef<Ortho2D>(-AR * m_fZoom, AR* m_fZoom, -m_fZoom, m_fZoom, name))
	{
		KROSS_PROFILE_FUNC();
		if (GetSelf())
		{
			KROSS_CORE_WARN("[ {0} ] |||| WARNING: Overriding previous camera: {1}", __FUNCTION__, GetSelf()->GetName());
			SetSelf(m_Camera.get());
		}
		KROSS_CORE_INFO("[ {0} ] |||| Camera Controller Created", __FUNCTION__);
	}

	Ortho2DCtrl::Ortho2DCtrl(bool rot, const std::string& name, float AR)
		: m_fAR(AR),
		m_bRotation(rot),
		m_Camera(makeRef<Ortho2D>(-AR * m_fZoom, AR* m_fZoom, -m_fZoom, m_fZoom, name))
	{
		KROSS_PROFILE_FUNC();
		if (GetSelf())
		{
			KROSS_CORE_WARN("[ {0} ] |||| WARNING: Overriding previous camera: {1}", __FUNCTION__, GetSelf()->GetName());
			SetSelf(m_Camera.get());
		}
		KROSS_CORE_INFO("[ {0} ] |||| Camera Controller Created", __FUNCTION__);
	}


	Ortho2DCtrl::~Ortho2DCtrl()
	{
		KROSS_PROFILE_FUNC();
		m_Camera->~Ortho2D();
		m_Camera.~shared_ptr();
		SetSelf(nullptr);
		KROSS_CORE_INFO("[ {0} ] |||| Camera Controller Deconstructed", __FUNCTION__);
	}

	void Ortho2DCtrl::OnUpdate(Timestep ts)
	{
		KROSS_PROFILE_FUNC();
		v3cameraPos.x += move.x * ts * fCameraMoveSpeed;
		v3cameraPos.y += move.y * ts * fCameraMoveSpeed;
		m_Camera->SetPosition(v3cameraPos);

		fCameraRotation += move.z * ts * fCameraRotationSpeed;
		m_Camera->SetRotation(fCameraRotation);

		fCameraMoveSpeed = m_fZoom * 1.2f;
	}

	void Ortho2DCtrl::OnEvent(Event& e)
	{
		KROSS_PROFILE_FUNC();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(KROSS_BIND_EVENT_FN(Ortho2DCtrl::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(KROSS_BIND_EVENT_FN(Ortho2DCtrl::OnMouseScrolled));
	}

	Ref<Camera> Ortho2DCtrl::GetCamera()
	{
		return m_Camera;
	}

	const Ref<Camera> Ortho2DCtrl::GetCamera() const
	{
		return m_Camera;
	}

	bool Ortho2DCtrl::OnWindowResized(WindowResizeEvent& e)
	{
		KROSS_PROFILE_FUNC();
		size = { e.GetWidth(), e.GetHeight() };
		m_fAR = size.x / (float)size.y;
		m_Camera->SetProjMat(-m_fAR * m_fZoom, m_fAR * m_fZoom, -m_fZoom, m_fZoom);
		//m_Camera->SetProjMat(-m_fAR * m_fZoom, m_fAR * m_fZoom, -m_fZoom, m_fZoom);
		//m_Camera->SetProjMat(-m_fAR, m_fAR, -m_fZoom, m_fZoom);
		return false;
	}

	bool Ortho2DCtrl::OnMouseScrolled(MouseScrolledEvent& e)
	{
		KROSS_PROFILE_FUNC();
		m_fZoom += e.GetYOffSet() * (m_fZoom * 0.01f);
		m_fZoom = std::max(m_fZoom, 0.005f);
		m_Camera->SetProjMat(-m_fAR * m_fZoom, m_fAR * m_fZoom, -m_fZoom, m_fZoom);
		return false;
	}
	bool Ortho2DCtrl::Input(std::function<bool(int)> func)
	{
		move.x = func(KROSS_KEY_D) - func(KROSS_KEY_A);
		move.y = func(KROSS_KEY_W) - func(KROSS_KEY_S);

		if (func(KROSS_KEY_M)) m_fZoomRate *= 1.1f;
		if (func(KROSS_KEY_N)) m_fZoomRate *= 0.9f;

		if (m_fZoomRate > -0.001f) m_fZoomRate = -0.001f;

		if (func(KROSS_KEY_I)) m_fZoom += m_fZoomRate;
		if (func(KROSS_KEY_O)) m_fZoom -= m_fZoomRate;

		m_fZoom = std::max(m_fZoom, 0.005f);

		if (m_bRotation) move.z = (func(KROSS_KEY_Q) - func(KROSS_KEY_E));
		return false;
	}
	void Ortho2DCtrl::DebugWindow()
	{
		if (ImGui::Begin("Camera", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("Position: X %.1f, Y %.1f", v3cameraPos.x, v3cameraPos.y);
			ImGui::Text("Input Position: X %d, Y %d", move.x, move.y);
			ImGui::Separator();
			ImGui::Text("Rotation: %s", m_bRotation ? "Enable" : "Disable");
			ImGui::Text("Output Rotation: %.1f", fCameraRotation);
			ImGui::Text("Rotation Speed: %d", fCameraRotationSpeed);
			ImGui::Text("Input Rotation: %d", move.z);
		}
		ImGui::End();
	}
}