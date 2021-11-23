#pragma once
#include "Kross/Renderer/Cameras/Camera.h"

namespace Kross::Camera3D {
	class Perspective : public Kross::Camera::Camera
	{
	public:
		Perspective(float aspectRatio, float fov, const char* name = "blank");
		~Perspective();

		virtual bool isLookingAt() const override { return m_bLookingAt; }
		virtual void LockAt(const glm::vec3& target) override;
		virtual void unLock() override;

		virtual void SetPosition(const glm::vec3& pos) override;
		virtual void SetRotation(const glm::vec3& rot) override;
		virtual void SetRotation(float angle, Axis a) override;
		virtual void SetProjMat(float fov, float fAspectRatio, float fNear, float fFar) override;

		virtual const glm::mat4& GetVPM()		const override { return m_VPM; }
		virtual const glm::mat4& GetProjMat()	const override { return m_ProjMat; }
		virtual const glm::mat4& GetViewMat()	const override { return m_ViewMat; }
		virtual const glm::vec3& GetPosition()	const override { return m_Position; }
		virtual const glm::vec3& GetRotation()	const override { return m_Rotation; }

		CAMERA_TYPE(Perspective_3D);
	};
}
