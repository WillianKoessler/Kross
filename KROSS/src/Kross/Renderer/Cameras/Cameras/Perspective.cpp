#include "Kross_pch.h"
#include "Perspective.h"

namespace Kross::Camera3D {
	Perspective::Perspective(float aspectRatio, float fov, const char* name)
		: Camera(name, glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 2000.0f))
	{
		m_Rotation = glm::vec3(90.0f, 45.0f, 0.0f);
		APIorder();
		KROSS_CORE_INFO("[{0}] Camera Created", __FUNCTION__);
	}
	Perspective::~Perspective() {
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[{0}] Camera Destroyed", __FUNCTION__);
	}
	void Perspective::LockAt(const glm::vec3& target) {
		m_bLookingAt = true;
		m_Target = target;
	}
	void Perspective::unLock() {
		m_bLookingAt = false;
		RecalculateVPM();
	}
	void Perspective::SetProjMat(float fov, float fAspectRatio, float fNear, float fFar) {
		KROSS_PROFILE_FUNC();
		m_ProjMat = glm::perspective(glm::radians(fov), fAspectRatio, fNear, fFar);
	}
	void Perspective::SetPosition(const glm::vec3& pos) {
		m_Position = pos;
		RecalculateVPM();
	}
	void Perspective::SetRotation(const glm::vec3& rot) {
		if(!m_bLookingAt) m_Rotation = rot;
		RecalculateVPM();
	}
	void Perspective::SetRotation(float angle, Axis a) {
		if (m_bLookingAt) return;
		switch (a) {
			case Axis::X: m_Rotation.x = angle; return;
			case Axis::Y: m_Rotation.y = angle; return;
			case Axis::Z: m_Rotation.z = angle; return;
		}
		KROSS_CORE_WARN("[{0}] Axis of rotation not valid.", __FUNCTION__);
	}
}