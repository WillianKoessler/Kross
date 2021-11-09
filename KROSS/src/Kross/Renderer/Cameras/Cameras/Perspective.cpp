#include "Kross_pch.h"
#include "Perspective.h"

namespace Kross::Camera3D {
	Perspective::Perspective(float aspectRatio, float fov, const std::string& name)
		: m_ProjMat(glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 2000.0f)),
		m_Position(glm::vec3(0.0f))
	{

	}
	Perspective::~Perspective()
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[Kross::Camera3D::Perspective] Camera Destroyed");
	}

	void Perspective::SetProjMat(float fov, float fAspectRatio, float fNear, float fFar)
	{
		KROSS_PROFILE_FUNC();
		m_ProjMat = glm::perspective(glm::radians(fov), fAspectRatio, fNear, fFar);
	}

	void Perspective::SetPosition(const glm::vec3& pos)
	{
		m_Position = pos;
		RecalculateVPM();
	}

	void Perspective::SetRotation(const glm::vec3& rot)
	{
		m_Rotation = rot;
		RecalculateVPM();
	}

	void Perspective::SetRotation(float angle, Axis a)
	{
	}

	inline void Perspective::RecalculateVPM()
	{
		KROSS_PROFILE_FUNC();
		glm::mat4 transform(1.0f);
		transform = glm::rotate(transform, glm::radians(m_Rotation.x), {1, 0, 0});
		transform = glm::rotate(transform, glm::radians(m_Rotation.y), {0, 1, 0});
		transform = glm::rotate(transform, glm::radians(m_Rotation.z), {0, 0, 1});
		m_ViewMat = glm::translate(transform, -m_Position);
//		m_ViewMat = glm::lookAt(m_Position, m_Position + m_Rotation, {0.0f,1.0f,0.0f});
		APIorder();
	}

	inline void Perspective::APIorder()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	KROSS_MSGBOX("Renderer API (None) is not supported", "[Kross::Camera::Ortho]", _ERROR_);
		case RendererAPI::API::OpenGL:	m_VPM = m_ProjMat * m_ViewMat;
		}
	}
}