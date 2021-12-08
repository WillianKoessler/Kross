#include "Kross_pch.h"
#include "SceneCamera.h"

namespace Kross {
	SceneCamera::SceneCamera()
	{
		RecalcProjMat();
	}

	void SceneCamera::SetOrtho(float fSize, float fNear, float fFar)
	{
		m_fOrthoSize = fSize;
		m_fOrthoNear = fNear;
		m_fOrthoFar = fFar;
		RecalcProjMat();
	}

	void SceneCamera::SetViewportSize(const glm::vec2& size)
	{
		if (size.x < 1|| size.y < 1) return;
		m_fAspectRatio = size.x / size.y;
		RecalcProjMat();
	}
	void SceneCamera::RecalcProjMat()
	{
		float left = -m_fOrthoSize * m_fAspectRatio * 0.5f;
		float right = m_fOrthoSize * m_fAspectRatio * 0.5f;
		float bottom = -m_fOrthoSize * 0.5f;
		float top = m_fOrthoSize * 0.5f;
		m_ProjMat = glm::ortho(left, right, bottom, top, m_fOrthoNear, m_fOrthoFar);
	}
}