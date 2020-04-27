#include "Kross_pch.h"
#include "PerspectiveCtrl.h"

#include "Kross/Core/Input.h"
#include "Kross/Core/KeyCodes.h"

#include "Kross/Core/Application.h"

#include "imgui.h"

namespace Kross::Camera {
	Persp3DCtrl::Persp3DCtrl(Persp3D* camera, uint32_t width, uint32_t height)
		:
		m_fAR(height / (float)width),
		m_fFOV(90.0f),
		m_fNear(camera->fNear),
		m_fFar(camera->fFar),
		m_Camera(camera)
	{}
	Persp3DCtrl::Persp3DCtrl(float AR, float FOV, const std::string& name)
		:
		m_fAR(AR),
		m_fFOV(FOV),
		m_fNear(0.01f),
		m_fFar(100.f),
		m_Camera(makeRef<Persp3D>(FOV, AR, m_fNear, m_fFar, name))
	{
		KROSS_PROFILE_FUNC();
		if (GetSelf())
		{
			KROSS_CORE_WARN("[ {0} ] |||| WARNING: '{2}' Is Overriding previous camera: {1}", __FUNCTION__, GetSelf()->GetName(), name);
			SetSelf(m_Camera.get());
		}
		KROSS_CORE_INFO("[ {0} ] |||| Camera Controller '{1}' Created", __FUNCTION__, name);
	}
	Persp3DCtrl::~Persp3DCtrl()
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[ {0} ] |||| Camera Controller Destroyed", __FUNCTION__);
		m_Camera->~Persp3D();
		m_Camera.~shared_ptr();
		SetSelf(nullptr);
	}
	void Persp3DCtrl::OnUpdate(Timestep ts)
	{
		KROSS_PROFILE_FUNC();
		rotate_at += dir_input * ts.GetSeconds() * fCameraRotationSpeed;
		float yaw = glm::radians(rotate_at.y);
		float pitch = glm::radians(rotate_at.x);

		right = glm::normalize(glm::cross(up, dir));

		dir.x = cos(yaw) * cos(pitch);
		dir.z = sin(yaw) * cos(pitch);

		float Ylimit = 90 - maxRotationSpeed;
		if (fabs(rotate_at.x) < Ylimit)
			dir.y = sin(pitch);
		else if (rotate_at.x > Ylimit)
			rotate_at.x = Ylimit;
		else if (rotate_at.x < -Ylimit)
			rotate_at.x = -Ylimit;

		float speed = fCameraMoveSpeed * ts;
		if (move.z != 0) v3cameraPos += move.z * speed * dir;
		if (move.x != 0) v3cameraPos += move.x * speed * right;
		if (move.y != 0) v3cameraPos += move.y * speed * up;

		m_Camera->SetOrientation(up);
		m_Camera->SetDirection(dir + v3cameraPos);
		m_Camera->SetPosition(v3cameraPos);
	}
	void Persp3DCtrl::OnEvent(Event& e)
	{
		KROSS_PROFILE_FUNC();
		static EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(KROSS_BIND_EVENT_FN(Persp3DCtrl::OnWindowResized));
		dispatcher.Dispatch<MouseScrolledEvent>(KROSS_BIND_EVENT_FN(Persp3DCtrl::OnMouseScrolled));
	}
	Ref<Camera> Persp3DCtrl::GetCamera()
	{
		return m_Camera;
	}
	const Ref<Camera> Persp3DCtrl::GetCamera() const
	{
		return m_Camera;
	}
	bool Persp3DCtrl::OnWindowResized(WindowResizeEvent& e)
	{
		KROSS_PROFILE_FUNC();
		size = { e.GetWidth(), e.GetHeight() };
		//m_fAR = size.x / (float)size.y;
		//m_fFOV *= 1 / m_fAR;
		//m_Camera->SetProjMat(m_fFOV, m_fAR, m_fNear, m_fFar);
		return false;
	}
	bool Persp3DCtrl::OnMouseScrolled(MouseScrolledEvent& e)
	{
		KROSS_PROFILE_FUNC();
		return false;
	}
	bool Persp3DCtrl::Input(std::function<bool(int)> func)
	{
		move.y = (int)func(KROSS_KEY_E) - (int)func(KROSS_KEY_Q);
		move.x = (int)func(KROSS_KEY_D) - (int)func(KROSS_KEY_A);
		move.z = (int)func(KROSS_KEY_W) - (int)func(KROSS_KEY_S);

		static glm::vec2 pmouse = (glm::vec2)Input::GetMousePos();
		static constexpr float maxMouse = 2.0f;
		auto& window_pos = ImGui::GetWindowPos();
		mouseMove = (glm::vec2)Input::GetMousePos() - pmouse;
		Input::SetMousePos({
			window_pos.x + size.x / 2,
			window_pos.y + size.y / 2
			});
		pmouse = Input::GetMousePos();

		glm::clamp(mouseMove, { -maxRotationSpeed * 10, -maxRotationSpeed }, { maxRotationSpeed * 10, maxRotationSpeed });

		dir_input.x = -mouseMove.y;
		dir_input.y = -mouseMove.x;

		return false;
	}
	void Persp3DCtrl::DebugWindow()
	{
		if (ImGui::Begin("Camera Settings", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			static auto pv3 = [](const char* msg, const glm::vec3& v3)
			{
				ImGui::Text(msg, v3.x, v3.y, v3.z);
			};
			ImGui::Text("Speed"); ImGui::SameLine(); ImGui::SliderFloat("##Speed", &fCameraMoveSpeed, 0.01f, 2.0f, "%.1f");
			ImGui::Text("Sensibility"); ImGui::SameLine(); ImGui::SliderFloat("##Sensibility", &fCameraRotationSpeed, 1.0f, 360.0f, "%.1f");
			ImGui::Text("Position: X %.1f, Y %.1f, Z %.1f", v3cameraPos.x, v3cameraPos.y, v3cameraPos.z);
			ImGui::Separator();
			ImGui::Text("Output Rotation: %.1f", fCameraRotation);
			ImGui::Text("Rotation Speed: %.1f", fCameraRotationSpeed);
			ImGui::Text("Input Rotation: %d", nCameraRotateAlongZAxis);
			pv3("Direction: X %.1f, Y %.1f, Z %.1f", m_Camera->GetDirection());
			pv3("ctrl dir: X %.1f, Y %.1f, Z %.1f", dir);
			ImGui::Text("Mouse: X %.1f, Y %.1f", mouseMove.x, mouseMove.y);
			pv3("dir_input: X %.1f, Y %.1f, Z %.1f", dir_input);
			pv3("Right: %.1f, %.1f, %.1f", right);
			pv3("Up: %.1f, %.1f, %.1f", up);
			pv3("rotate_at: %.1f, %.1f, %.1f", rotate_at);
		}
		ImGui::End();
	}
}