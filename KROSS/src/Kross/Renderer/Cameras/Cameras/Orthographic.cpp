#include "Kross_pch.h"
#include "Orthographic.h"


namespace Kross::Camera2D {
	Orthographic::Orthographic(float left, float right, float bottom, float top, const char* name)
		: Camera(name, glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
	{
		KROSS_PROFILE_FUNC();
		APIorder();
		KROSS_CORE_INFO("[{0}] Camera Created", __FUNCTION__);
	}

	Orthographic::~Orthographic()
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[{0}] Camera Destroyed", __FUNCTION__);
	}

	void Orthographic::SetProjMat(float left, float right, float bottom, float top)
	{
		KROSS_PROFILE_FUNC();
		m_ProjMat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}

	void Orthographic::SetPosition(const glm::vec3 & pos)
	{
		m_Position = pos;
		RecalculateVPM();
	}

	void Orthographic::SetRotation(const glm::vec3& rot)
	{
		m_Rotation = rot;
	}

	void Orthographic::SetRotation(float angle, Axis a)
	{
		switch (a) {
		case Axis::X: m_Rotation.x = angle; break;
		case Axis::Y: m_Rotation.y = angle; break;
		case Axis::Z: m_Rotation.z = angle; break;
		}
		RecalculateVPM();
	}
}