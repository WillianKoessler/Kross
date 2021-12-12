#include "Kross_pch.h"
#include "SceneCamera.h"

namespace Kross {
	SceneCamera::SceneCamera()
	{
		RecalcProjMat();
	}

	void SceneCamera::SetOrthographic(float fSize, float fNear, float fFar)
	{
		m_ProjType = ProjectionType::Orthographic;
		m_fOrthoSize = fSize;
		m_fOrthoNear = fNear, m_fOrthoFar = fFar;
		RecalcProjMat();
	}
	void SceneCamera::SetPerspective(float vFOV, float fNear, float fFar)
	{
		m_ProjType = ProjectionType::Perspective;
		m_fPerspFOV = vFOV;
		m_fPerspNear = fNear;
		m_fPerspFar = fFar;
		RecalcProjMat();
	}
	void SceneCamera::SetViewportSize(const glm::vec2 &size)
	{
		if (size.x < 1 || size.y < 1) return;
		m_fAspectRatio = size.x / size.y;
		RecalcProjMat();
	}
	void SceneCamera::RecalcProjMat()
	{
		switch (m_ProjType) {
			case ProjectionType::Orthographic:
				m_ProjMat = glm::ortho(
					-m_fOrthoSize * m_fAspectRatio * 0.5f,
					m_fOrthoSize * m_fAspectRatio * 0.5f,
					-m_fOrthoSize * 0.5f,
					m_fOrthoSize * 0.5f,
					m_fOrthoNear,
					m_fOrthoFar
				);
				break;

			case ProjectionType::Perspective:
				m_ProjMat = glm::perspective(
					m_fPerspFOV,
					m_fAspectRatio,
					m_fPerspNear,
					m_fPerspFar
				);
				break;
		}
	}
}