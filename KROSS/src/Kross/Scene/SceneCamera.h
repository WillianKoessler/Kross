#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Cameras/Camera.h"

namespace Kross {
	class SceneCamera : public Camera::Camera
	{
	public:
		enum class ProjectionType { Perspective = 0u, Orthographic = 1u };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetOrthographic(float fSize, float fNear, float fFar);
		void SetPerspective(float vFOV, float fNear, float fFar);
		void SetViewportSize(const glm::vec2& size);

		float OrthoSize() const { return m_fOrthoSize; }
		void SetOrthoSize(float size) { m_fOrthoSize = size; RecalcProjMat(); }

		float GetPerspVerticalFOV() const { return m_fPerspFOV; }
		void SetPerspVerticalFOV(float value) { m_fPerspFOV = value; RecalcProjMat();  }

		float GetNearClip() const { if (m_ProjType == ProjectionType::Orthographic) return m_fOrthoNear; else return m_fPerspNear; }
		void SetNearClip(float value) { if (m_ProjType == ProjectionType::Orthographic) m_fOrthoNear = value; else m_fPerspNear = value; RecalcProjMat();  }

		float GetFarClip() const { if (m_ProjType == ProjectionType::Orthographic) return m_fOrthoFar; else return m_fPerspFar; }
		void SetFarClip(float value) { if(m_ProjType == ProjectionType::Orthographic) m_fOrthoFar = value; else m_fPerspFar = value; RecalcProjMat(); }

		ProjectionType GetProjType() const { return m_ProjType; }
		void SetProjType(ProjectionType type) { m_ProjType = type; RecalcProjMat(); }

		void RecalcProjMat();
	private:
		ProjectionType m_ProjType = ProjectionType::Orthographic;
		float m_fOrthoSize = 10.0f;
		float m_fOrthoNear = -1.0f, m_fOrthoFar = 1000.0f;
		float m_fAspectRatio = 16.0f / 9.0f;

		float m_fPerspFOV = glm::radians(90.0f);
		float m_fPerspNear = 0.1f, m_fPerspFar = 1000.f;
	};
}