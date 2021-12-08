#include "Kross_pch.h"
#include "FirstPerson.h"

#include "Kross/Core/Input.h"
#include "Kross/Events/KeyCodes.h"
#include "Kross/Core/Application.h"
#include "Kross/Util/Util.h"

#define _CRT_SECURE_NO_WARNINGS 

namespace Kross::Camera3D {

	FirstPerson::FirstPerson(const char* cname, float aspectRatio, float fov)
		: m_fFOV(fov), mouseRotation(glm::vec3(0.0f, 0.0f, 1.0f))
	{
		if (camera) KROSS_CORE_WARN("WARNING: Overriding previous camera");
		camera = makeRef<Camera::Perspective>(aspectRatio, fov);
		static const char *a = "_Controller";
		std::string strname(cname);
		strname += "_Controller";
		m_strName = new char[strname.size()];
		memcpy((void*)m_strName, strname.data(), strname.size());

		//forward = glm::vec3(0.0f, 0.0f, 1.0f);
		//right = glm::vec3(1.0f, 0.0f, 0.0f);
		//up = glm::vec3(0.0f, 1.0f, 0.0f);
		KROSS_CORE_INFO("'{0}' Constructed", m_strName);
	}

	FirstPerson::FirstPerson(const char* name, Ref<Camera::Camera> cam, float fov)
		: m_fFOV(fov), mouseRotation(glm::vec3(0.0f, 0.0f, 1.0f)), name(nullptr)
	{
		if (camera) KROSS_CORE_WARN("WARNING: Overriding previous Camera");
		camera = cam;
		m_strName = name;
		//forward = glm::vec3(0.0f, 0.0f, 1.0f);
		//right = glm::vec3(1.0f, 0.0f, 0.0f);
		//up = glm::vec3(0.0f, 1.0f, 0.0f);
		KROSS_CORE_INFO("'{0}' Constructed", m_strName);
	}

	void FirstPerson::OnUpdate(double ts)
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_ERROR("Not implemented");
		//if (Input::IsKeyReleased(Key::Space))
		//	m_bCursorEnabled = !m_bCursorEnabled;

		////if (!m_bCursorEnabled) camera->unLock();
		////else camera->LockAt(glm::vec3(0.0f));

		//Application::Get().GetWindow().CursorEnabled(m_bCursorEnabled);
		//glm::vec3& position = const_cast<glm::vec3&>(camera->GetPosition());
		//float speed = m_fSpeed * (float)ts;

		//static glm::vec3 u = { 0.0f, 1.0f, 0.0f };

		//const glm::mat4& vpm = camera->GetVPM();
		//glm::vec3 forward = glm::normalize(glm::vec3(vpm[0][2], vpm[1][2], vpm[2][2]));
		//glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f,1.0f,0.0f)));
		//glm::vec3 up = glm::normalize(glm::cross(forward, right));
		////char b[256]; sprintf_s(b, "before(%.4f, %.4f, %.4f)", right.x, right.y, right.z);
		////char a[256]; sprintf_s(a, "after(%.4f, %.4f, %.4f)", right.x, right.y, right.z);
		////KROSS_CORE_TRACE("right: {0} | {1}", b, a);

		//char strForward[256];
		//char strRight[256];
		//char strUp[256];
		//sprintf_s(strForward, "forward(%.4f, %.4f, %.4f)", forward.x, forward.y, forward.z);
		//sprintf_s(strRight, "right(%.4f, %.4f, %.4f)", right.x, right.y, right.z);
		//sprintf_s(strUp, "up(%.4f, %.4f, %.4f)", up.x, up.y, up.z);
		////KROSS_CORE_TRACE("{0} | {1} | {2}", strForward, strRight, strUp);


		//if (Input::IsKeyHeld(Key::W)) position += forward * speed;
		//if (Input::IsKeyHeld(Key::S)) position -= forward * speed;
		//if (Input::IsKeyHeld(Key::A)) position -= right * speed;
		//if (Input::IsKeyHeld(Key::D)) position += right * speed;
		//if (Input::IsKeyHeld(Key::E)) position.y += speed;
		//if (Input::IsKeyHeld(Key::Q)) position.y -= speed;
		////if (Input::IsKeyHeld(Key::E)) position += up * speed;
		////if (Input::IsKeyHeld(Key::Q)) position -= up * speed;
		//camera->SetPosition(position);
		//camera->SetRotation(mouseRotation);
	}

	void FirstPerson::OnEvent(Event& e)
	{
		KROSS_PROFILE_FUNC();
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowResizeEvent>(KROSS_BIND_EVENT_FN(FirstPerson::OnWindowResized));
		dispatcher.Dispatch<MouseMovedEvent>(KROSS_BIND_EVENT_FN(FirstPerson::OnMouseMoved));
	}

	Ref<Kross::Camera::Camera> FirstPerson::GetCamera() { return camera; }
	const Ref<Kross::Camera::Camera> FirstPerson::GetCamera() const { return camera; }
	void FirstPerson::Zoom(float val) {}
	bool FirstPerson::OnWindowResized(WindowResizeEvent& e)
	{
		KROSS_CORE_ERROR("Not Implemented");
		//KROSS_PROFILE_FUNC();
		//float ar = (float)e.GetWidth() / (float)e.GetHeight();
		//camera->SetProjMat(m_fFOV, ar, 0.01f, 1000.0f);
		return false;
	}
	bool FirstPerson::OnMouseMoved(MouseMovedEvent& e)
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

		last = current;
		return false;
	}
	bool FirstPerson::OnMouseScrolled(MouseScrolledEvent &e)
	{
		return false;
	}
}