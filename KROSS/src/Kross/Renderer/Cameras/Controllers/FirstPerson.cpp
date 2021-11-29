#include "Kross_pch.h"
#include "FirstPerson.h"

#include "Kross/Core/Input.h"
#include "Kross/Core/KeyCodes.h"
#include "Kross/Core/Application.h"

namespace Kross::Camera3D {
	//glm::vec3 FirstPerson::m_v3Rot = glm::vec3(0.0f);
	//FirstPerson::FirstPerson(Camera::Camera* cam, float aspectRatio, float fov)
	//	: m_fAR(aspectRatio),
	//	m_fFOV(fov),
	//	mouseRotation(glm::vec3(0.0f, 0.0f, 1.0f)) {
	//	KROSS_PROFILE_FUNC();
	//	if (camera)
	//		KROSS_CORE_WARN("[{0}] WARNING: Overriding previous {2} Camera: {1}", __FUNCTION__, camera->GetName(), camera->GetType());

	//	camera.reset(cam);
	//	KROSS_CORE_INFO("[{0}] Camera Controller Created", __FUNCTION__);
	//}

	//FirstPerson::FirstPerson(const std::string& name, float aspectRatio, float fov)
	//	: m_fAR(aspectRatio),
	//	m_fFOV(fov),
	//	mouseRotation(glm::vec3(0.0f, 0.0f, 1.0f)) {
	//	KROSS_PROFILE_FUNC();
	//	if (camera)
	//		KROSS_CORE_WARN("[{0}] WARNING: Overriding previous camera: {1}", __FUNCTION__, camera->GetName());

	//	camera = makeRef<Perspective>(m_fAR, m_fFOV, name);
	//	KROSS_CORE_INFO("[{0}] Camera Controller Created", __FUNCTION__);
	//}

	//FirstPerson::~FirstPerson() {
	//	KROSS_PROFILE_FUNC();
	//	KROSS_CORE_INFO("[{0}] Camera Controller Deconstructed", __FUNCTION__);
	//}

	//void FirstPerson::OnUpdate(double ts) {
	//	KROSS_PROFILE_FUNC();
	//	if (Input::IsKeyReleased(KROSS_KEY_SPACE))
	//		m_bCursorEnabled = !m_bCursorEnabled;

	//	if (!m_bCursorEnabled) camera->unLock();
	//	else camera->LockAt(glm::vec3(0.0f));

	//	Application::Get().GetWindow().CursorEnabled(m_bCursorEnabled);
	//	glm::vec3& position = const_cast<glm::vec3&>(camera->GetPosition());
	//	float speed = m_fSpeed * (float)ts;

	//	const glm::mat4& vpm = camera->GetVPM();
	//	glm::vec3 forward = glm::normalize(glm::vec3(vpm[0][2], vpm[1][2], vpm[2][2])) * speed;
	//	glm::vec3 right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f))) * speed;

	//	if (Input::IsKeyHeld(KROSS_KEY_W)) position += forward;
	//	if (Input::IsKeyHeld(KROSS_KEY_S)) position -= forward;
	//	if (Input::IsKeyHeld(KROSS_KEY_A)) position -= right;
	//	if (Input::IsKeyHeld(KROSS_KEY_D)) position += right;
	//	if (Input::IsKeyHeld(KROSS_KEY_E)) position.y += speed;
	//	if (Input::IsKeyHeld(KROSS_KEY_Q)) position.y -= speed;
	//	camera->SetPosition(position);
	//	camera->SetRotation(mouseRotation);
	//	CalculateView();
	//}

	//void FirstPerson::OnEvent(Event& e) {
	//	KROSS_PROFILE_FUNC();
	//	EventDispatcher dispatcher(e);
	//	dispatcher.Dispatch<WindowResizeEvent>(KROSS_BIND_EVENT_FN(FirstPerson::OnWindowResized));
	//	dispatcher.Dispatch<MouseMovedEvent>(KROSS_BIND_EVENT_FN(FirstPerson::OnMouseMoved));
	//}

	//Ref<Kross::Camera::Camera> FirstPerson::GetCamera() {
	//	return camera;
	//}

	//const Ref<Kross::Camera::Camera> FirstPerson::GetCamera() const {
	//	return camera;
	//}

	//void FirstPerson::CalculateView() {
	//	camera->SetProjMat(m_fFOV, m_fAR, 0.01f, 1000.0f);
	//}

	//bool FirstPerson::OnWindowResized(WindowResizeEvent& e) {
	//	KROSS_PROFILE_FUNC();
	//	m_fAR = (float)e.GetWidth() / (float)e.GetHeight();
	//	CalculateView();
	//	return false;
	//}

	//bool FirstPerson::OnMouseMoved(MouseMovedEvent& e) {
	//	KROSS_PROFILE_FUNC();
	//	if (m_bCursorEnabled) return false;

	//	static glm::vec2 last = glm::vec2(0.0f);
	//	glm::vec2 current = glm::vec2(e.GetX(), e.GetY());
	//	glm::vec2 size = glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

	//	if (current == last) return false;

	//	glm::vec2 now = (current - last) * 0.5f;

	//	mouseRotation.y += now.x;
	//	mouseRotation.x += now.y;

	//	if (mouseRotation.x > m_fYbound) mouseRotation.x = m_fYbound;
	//	else if (mouseRotation.x < -m_fYbound) mouseRotation.x = -m_fYbound;

	//	if (mouseRotation.y > 360.0f) mouseRotation.y = 0.0f;
	//	else if (mouseRotation.y < 0.0f) mouseRotation.y = 360.0f;

	//	//KROSS_CORE_TRACE("MouseMoved {0}, {1}", mouseRotation.y, mouseRotation.x);

	//	last = current;
	//	return false;
	//}
}