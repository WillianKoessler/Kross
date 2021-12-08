#include <Kross_pch.h>
#include "Perspective.h"

namespace Kross::Camera {
	Perspective::Perspective(float aspectRatio, float fov)
		:
		m_ViewMat(glm::mat4(1.0f)),
		m_VPM(glm::mat4(1.0f)),
		m_Position(glm::vec3(0.0f)),
		m_Rotation({90.0f, 45.0f, 0.0f}),
		m_bLookingAt(true),
		m_Target(glm::vec3(0.0f))
	{
		m_ProjMat = glm::perspective(glm::radians(fov), aspectRatio, 0.1f, 2000.0f);
		KROSS_CORE_INFO("Perspective Camera Constructed");
	}
	bool Perspective::isLookingAt() const { return m_bLookingAt; }
	void Perspective::LockAt(const glm::vec3& target) { m_bLookingAt = true; m_Target = target; }
	void Perspective::unLock() { m_bLookingAt = false; RecalculateVPM(); }
	void Perspective::SetPosition(const glm::vec3& pos) { m_Position = pos; RecalculateVPM(); }
	void Perspective::SetRotation(const glm::vec3& rot) { if (!m_bLookingAt) m_Rotation = rot; RecalculateVPM(); }
	const glm::mat4& Perspective::GetVPM() const { return m_VPM; }
	const glm::mat4& Perspective::GetViewMat() const { return m_ViewMat; }
	const glm::vec3& Perspective::GetPosition() const { return m_Position; }
	const glm::vec3& Perspective::GetRotation() const { return m_Rotation; }
	inline void Perspective::RecalculateVPM()
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
	inline void Perspective::APIorder()
	{
		switch (RendererAPI::GetAPI()) {
			case RendererAPI::API::None:	KROSS_MSGBOX_ERROR("Renderer API (None) is not supported"); return;
			case RendererAPI::API::OpenGL:	m_VPM = m_ProjMat * m_ViewMat;
		}
	}
}