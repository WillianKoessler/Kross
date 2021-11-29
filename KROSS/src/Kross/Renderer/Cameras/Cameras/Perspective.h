#pragma once
#include "Kross/Renderer/Cameras/Camera.h"

namespace Kross::Camera3D {
	class Perspective : public Kross::Camera::Camera
	{
	public:
		Perspective(float aspectRatio, float fov, const std::string& name = "blank")
			: m_ProjMat(glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 2000.0f)),
			m_Position(glm::vec3(0.0f)),
			m_Rotation({ 90.0f, 45.0f, 0.0f })
		{
		}
		~Perspective()
		{
			KROSS_PROFILE_FUNC();
			KROSS_CORE_INFO("[{0}] Camera Destroyed", __FUNCTION__);
		}

		virtual bool isLookingAt() const override { return m_bLookingAt; }
		virtual void LockAt(const glm::vec3& target) override
		{
			m_bLookingAt = true;
			m_Target = target;
		}
		virtual void unLock() override
		{
			m_bLookingAt = false;
			RecalculateVPM();
		}

		virtual void SetPosition(const glm::vec3& pos) override
		{
			m_Position = pos;
			RecalculateVPM();
		}
		virtual void SetRotation(const glm::vec3& rot) override
		{
			if (!m_bLookingAt) m_Rotation = rot;
			RecalculateVPM();
		}
		virtual void SetRotation(float angle, Axis a) override
		{
			if (m_bLookingAt) return;
			switch (a) {
				case Axis::X: m_Rotation.x = angle; return;
				case Axis::Y: m_Rotation.y = angle; return;
				case Axis::Z: m_Rotation.z = angle; return;
			}
			KROSS_CORE_WARN("[{0}] Axis of rotation not valid.", __FUNCTION__);
		}
		virtual void SetProjMat(float fov, float fAspectRatio, float fNear, float fFar) override
		{
			KROSS_PROFILE_FUNC();
			m_ProjMat = glm::perspective(glm::radians(fov), fAspectRatio, fNear, fFar);
		}

		virtual const glm::mat4& GetVPM()		const override { return m_VPM; }
		virtual const glm::mat4& GetProjMat()	const override { return m_ProjMat; }
		virtual const glm::mat4& GetViewMat()	const override { return m_ViewMat; }
		virtual const glm::vec3& GetPosition()	const override { return m_Position; }
		virtual const glm::vec3& GetRotation()	const override { return m_Rotation; }

		virtual const std::string& GetName() const override { return m_strName; }
		CAMERA_TYPE(Perspective_3D);
	public:
		std::string m_strName;

	private:
		inline void RecalculateVPM()
		{
			KROSS_PROFILE_FUNC();
			glm::mat4 transform(1.0f);

			if (!m_bLookingAt) {
				transform = glm::rotate(transform, glm::radians(m_Rotation.x), { 1, 0, 0 });
				transform = glm::rotate(transform, glm::radians(m_Rotation.y), { 0, 1, 0 });
				transform = glm::rotate(transform, glm::radians(m_Rotation.z), { 0, 0, 1 });
			} else {
				transform = glm::lookAt(m_Position, m_Target, { 0.0f, 1.0f, 0.0f });
			}
			APIorder();
			m_ViewMat = glm::translate(transform, -m_Position);
		}
		inline void APIorder()
		{
			switch (RendererAPI::GetAPI()) {
				case RendererAPI::API::None:	KROSS_MSGBOX_ERROR("Renderer API (None) is not supported"); return;
				case RendererAPI::API::OpenGL:	m_VPM = m_ProjMat * m_ViewMat;
			}
		}

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
