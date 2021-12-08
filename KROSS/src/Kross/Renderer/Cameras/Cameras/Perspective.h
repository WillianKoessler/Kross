#pragma once
#include "Kross/Renderer/Cameras/Camera.h"

namespace Kross::Camera {
	class Perspective : public Kross::Camera::Camera
	{
	public:
		Perspective(float aspectRatio, float fov);

		bool isLookingAt() const;
		void LockAt(const glm::vec3& target);
		void unLock();
		void SetPosition(const glm::vec3& pos);
		void SetRotation(const glm::vec3& rot);
		const glm::mat4& GetVPM()		const;
		const glm::mat4& GetViewMat()	const;
		const glm::vec3& GetPosition()	const;
		const glm::vec3 &GetRotation()	const;

	private:
		inline void RecalculateVPM();
		inline void APIorder();

	private:
		glm::mat4 m_ViewMat;
		glm::mat4 m_VPM;

		glm::vec3 m_Position;
		glm::vec3 m_Rotation;

		bool m_bLookingAt;
		glm::vec3 m_Target;
	};
}
