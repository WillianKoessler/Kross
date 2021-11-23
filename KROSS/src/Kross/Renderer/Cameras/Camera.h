#pragma once

#include "glm/glm/gtc/matrix_transform.hpp"
#include "Kross/Renderer/RendererAPI.h"

namespace Kross::Camera {

#define CAMERA_TYPE(type)\
	static Type GetStaticType() { return Type::##type; }\
	virtual Type GetType() const override { return GetStaticType(); }\
	virtual const char* GetName() const override { return #type; }

	class Camera
	{
	public:
		enum class Axis { X, Y, Z };
		enum class Type { None = 0, Orthographic_2D, Perspective_3D };

	public:
		Camera(const char* name) : m_cpName(name) {}
		Camera(const char* name, const glm::mat4& projection) : m_cpName(name), m_ProjMat(projection) {}
		virtual Type GetType() const { return Type::None; };
		virtual const char* GetName() const { return "Unnamed Camera"; };

		virtual bool isLookingAt() const { return false; };
		virtual void LockAt(const glm::vec3& target) {};
		virtual void unLock() {};

		virtual void SetPosition(const glm::vec3& pos) {};
		virtual void SetRotation(const glm::vec3& rot) {};
		virtual void SetRotation(float angle, Axis a)  {};
		virtual void SetProjMat(float f1, float f2, float f3, float f4) {}; // for more detailed information about arguments, look on the specific type you're using

		virtual const glm::mat4& GetProjMat()	const { return m_ProjMat; };
		virtual const glm::mat4& GetViewMat()	const { return m_ViewMat; };
		virtual const glm::mat4& GetVPM()		const { return m_VPM; };
		virtual const glm::vec3& GetPosition()	const { return m_Position; };
		virtual const glm::vec3& GetRotation()	const { return m_Rotation; };

	protected:
		inline void RecalculateVPM()
		{
			KROSS_PROFILE_FUNC();
			glm::mat4 transform(1.0f);

			if (!m_bLookingAt)
			{
				transform = glm::rotate(transform, glm::radians(m_Rotation.x), { 1, 0, 0 });
				transform = glm::rotate(transform, glm::radians(m_Rotation.y), { 0, 1, 0 });
				transform = glm::rotate(transform, glm::radians(m_Rotation.z), { 0, 0, 1 });
			} else
			{
				transform = glm::lookAt(m_Position, m_Target, { 0.0f, 1.0f, 0.0f });
			}
			APIorder();
			m_ViewMat = glm::translate(transform, -m_Position);
		}

		inline void APIorder()
		{
			switch (RendererAPI::GetAPI())
			{
				case RendererAPI::API::None:	KROSS_MSGBOX("Renderer API (None) is not supported", __FUNCTION__, _ERROR_); return;
				case RendererAPI::API::OpenGL:	m_VPM = m_ProjMat * m_ViewMat;
			}
		}

	protected:
		const char* m_cpName;
		glm::mat4 m_ProjMat = glm::mat4(1.0f);
		glm::mat4 m_ViewMat = glm::mat4(1.0f);
		glm::mat4 m_VPM = glm::mat4(1.0f);
		glm::vec3 m_Position;
		glm::vec3 m_Rotation;
		bool m_bLookingAt;
		glm::vec3 m_Target;
	};

}
