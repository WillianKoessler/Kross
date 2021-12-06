#pragma once
#include "Kross/Renderer/Cameras/Camera.h"

namespace Kross::Camera3D {
	class Perspective : public Kross::Camera::Camera
	{
	public:
		Perspective(float aspectRatio, float fov, const char* name);

		virtual bool isLookingAt() const override;
		virtual void LockAt(const glm::vec3& target) override;
		virtual void unLock() override;
		virtual void SetPosition(const glm::vec3& pos) override;
		virtual void SetRotation(const glm::vec3& rot) override;
		virtual void SetRotation(float angle, Axis a) override;
		virtual void SetProjMat(float fov, float fAspectRatio, float fNear, float fFar) override;

		virtual const glm::mat4& GetVPM()		const override;
		virtual const glm::mat4& GetProjMat()	const override;
		virtual const glm::mat4& GetViewMat()	const override;
		virtual const glm::vec3& GetPosition()	const override;
		virtual const glm::vec3& GetRotation()	const override;

		CAMERA_TYPE(Perspective_3D)
	private:
		inline void RecalculateVPM();
		inline void APIorder();

	private:
		glm::mat4 m_ProjMat;
		glm::mat4 m_ViewMat;
		glm::mat4 m_VPM;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

		bool m_bLookingAt;
		glm::vec3 m_Target;
	};
}
