#pragma once

#include "Kross/Renderer/Cameras/Camera.h"

#include <string>

namespace Kross::Camera {
	class Persp3D : public Camera
	{
	public:
		Persp3D(float FOV, float AR, float fNear, float fFar, const std::string& name = "blank");
		~Persp3D();

		virtual void SetPosition(const glm::vec3& pos) override;
		virtual void SetRotation(float angle) override;
		virtual void SetProjMat(float FOV, float AR, float fNear, float fFar) override;

		virtual const glm::mat4& GetVPM()		const override { return m_VPM; }
		virtual const glm::mat4& GetProjMat()	const override { return m_ProjMat; }
		virtual const glm::mat4& GetViewMat()	const override { return m_ViewMat; }
		virtual const glm::vec3& GetPosition()	const override { return m_Position; }
		virtual const float GetRotation()		const override { return m_Rotation; }

		virtual const std::string& GetName() const override { return m_strName; }
		CAMERA_TYPE(Perspective_3D);
	public:
		std::string m_strName;

	private:
		inline void RecalculateVPM();
		inline void APIorder();

	private:
		glm::mat4 m_ProjMat;
		glm::mat4 m_ViewMat;
		glm::mat4 m_VPM;

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;

	};
}
