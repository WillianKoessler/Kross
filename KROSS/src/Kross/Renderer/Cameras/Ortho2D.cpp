#include "Kross_pch.h"
#include "Ortho2D.h"


namespace Kross::Camera {
	Ortho2D::Ortho2D(float left, float right, float bottom, float top, const std::string& name)
		:	m_ProjMat(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)),
			m_ViewMat(glm::mat4(1.0f)),
			m_strName(name)

	{
		KROSS_PROFILE_FUNC();
		APIorder();
		if (GetSelf())
		{
			KROSS_CORE_WARN("[Kross::Camera::Ortho2D] WARNING: Overriding previous camera: {0}", GetSelf()->GetName());
			SetSelf(this);
		}
		KROSS_CORE_INFO("[Kross::Camera::Ortho2D] Camera Created");
	}

	Ortho2D::~Ortho2D()
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[Kross::Camera::Ortho2D] Camera Destroyed");
	}

	void Ortho2D::SetProjMat(float left, float right, float bottom, float top)
	{
		KROSS_PROFILE_FUNC();
		m_ProjMat = glm::ortho(left, right, bottom, top, -1.0f, 1.0f);
	}

	void Ortho2D::SetPosition(const glm::vec3 & pos)
	{
		m_Position = pos;
		RecalculateVPM();
	}

	void Ortho2D::SetRotation(float angle)
	{
		m_Rotation = angle;
		RecalculateVPM();
	}

	inline void Ortho2D::RecalculateVPM()
	{
		KROSS_PROFILE_FUNC();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMat = glm::inverse(transform);
		APIorder();
	}

	inline void Ortho2D::APIorder()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	KROSS_MSGBOX("Renderer API (None) is not supported", "[Kross::Camera::Ortho2D]", _ERROR_);
		case RendererAPI::API::OpenGL:	m_VPM = m_ProjMat * m_ViewMat;
		}
	}
}