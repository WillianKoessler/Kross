#include "Kross_pch.h"
#include "Orthographic.h"


namespace Kross::Camera2D {
	Orthographic::Orthographic(float left, float right, float bottom, float top, const std::string& name)
		:	m_ProjMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
			m_ViewMat(glm::mat4(1.0f)),
			m_strName(name)

	{
		KROSS_PROFILE_FUNC();
		APIorder();
		KROSS_CORE_INFO("[Kross::Camera::Ortho] Camera Created");
	}

	Orthographic::~Orthographic()
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[Kross::Camera::Ortho] Camera Destroyed");
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

	inline void Orthographic::RecalculateVPM()
	{
		KROSS_PROFILE_FUNC();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		transform = glm::rotate(transform, glm::radians(m_Rotation.x), glm::vec3(1, 0, 0));
		transform = glm::rotate(transform, glm::radians(m_Rotation.y), glm::vec3(0, 1, 0));
		transform = glm::rotate(transform, glm::radians(m_Rotation.z), glm::vec3(0, 0, 1));
		m_ViewMat = glm::inverse(transform);
		APIorder();
	}

	inline void Orthographic::APIorder()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	KROSS_MSGBOX_ERROR("Renderer API (None) is not supported");
		case RendererAPI::API::OpenGL:	m_VPM = m_ProjMat * m_ViewMat;
		}
	}
}