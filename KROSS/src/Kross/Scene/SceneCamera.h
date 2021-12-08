#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Cameras/Camera.h"

namespace Kross {
	class SceneCamera : public Camera::Camera
	{
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrtho(float fSize, float fNear, float fFar);
		void SetViewportSize(const glm::vec2& size);

		float GetOrthoSize() const { return m_fOrthoSize; }
		void GetOrthoSize(float size) { m_fOrthoSize = size; RecalcProjMat(); }
	private:
		void RecalcProjMat();
	private:
		float m_fOrthoSize = 10.0f;
		float m_fOrthoNear = -1.0f, m_fOrthoFar = 1.0f;
		float m_fAspectRatio = 16.0f / 9.0f;
	};
}