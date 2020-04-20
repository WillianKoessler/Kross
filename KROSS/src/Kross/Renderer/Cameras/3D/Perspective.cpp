#include "Kross_pch.h"
#include "Perspective.h"


namespace Kross::Camera {
	Persp3D::Persp3D(float FOV, float AR, float fNear, float fFar, const std::string& name)
		: m_ProjMat(glm::perspective(glm::radians(FOV), AR, fNear, fFar)),
		m_ViewMat(glm::mat4(1.0f)),
		m_strName(name)

	{
		KROSS_PROFILE_FUNC();
		APIorder();
		if (GetSelf())
		{
			KROSS_CORE_WARN("[ {0} ] |||| WARNING: Overriding previous camera: {1}", __FUNCTION__, GetSelf()->GetName());
			SetSelf(this);
		}
		KROSS_CORE_INFO("[ {0} ] |||| Camera Created", __FUNCTION__);
	}

	Persp3D::~Persp3D()
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[ {0} ] |||| Camera Destroyed", __FUNCTION__);
	}

	void Persp3D::SetProjMat(float FOV, float AR, float fNear, float fFar)
	{
		KROSS_PROFILE_FUNC();
		m_ProjMat = glm::perspective(glm::radians(FOV), AR, fNear, fFar);
	}

	void Persp3D::SetPosition(const glm::vec3& pos)
	{
		m_Position = pos;
		RecalculateVPM();
	}

	void Persp3D::SetRotation(float angle)
	{
		m_Rotation = angle;
		RecalculateVPM();
	}

	inline void Persp3D::RecalculateVPM()
	{
		KROSS_PROFILE_FUNC();
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position);
		transform = glm::rotate(transform, glm::radians(m_Rotation), glm::vec3(0, 0, 1));
		m_ViewMat = glm::inverse(transform);
		APIorder();
	}

	inline void Persp3D::APIorder()
	{
		switch (RendererAPI::GetAPI())
		{
		case RendererAPI::API::None:	KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_);
		case RendererAPI::API::OpenGL:	m_VPM = m_ProjMat * m_ViewMat;
		}
	}
}