#pragma once
#include "Kross/Renderer/Cameras/Camera.h"

namespace Kross::Camera2D {
	class Orthographic : public Kross::Camera::Camera
	{
	public:
		Orthographic(float left, float right, float bottom, float top, const std::string& name = "blank");
		~Orthographic();

		virtual bool isLookingAt() const override { return false; }
		virtual void LockAt(const glm::vec3& target) override {}
		virtual void unLock() override {}

		virtual void SetPosition(const glm::vec3& pos) override;
		virtual void SetRotation(const glm::vec3& rot) override;
		virtual void SetRotation(float angle, Axis a) override;
		virtual void SetProjMat(float left, float right, float bottom, float top) override;

		virtual const glm::mat4& GetVPM()		const override { return m_VPM;		}
		virtual const glm::mat4& GetProjMat()	const override { return m_ProjMat;	}
		virtual const glm::mat4& GetViewMat()	const override { return m_ViewMat;	}
		virtual const glm::vec3& GetPosition()	const override { return m_Position;	}
		virtual const glm::vec3& GetRotation()	const override { return m_Rotation; }

		virtual const std::string& GetName() const override { return m_strName; }
		CAMERA_TYPE(Orthographic_2D);
	public:
		std::string m_strName;

	private:
		inline void RecalculateVPM();
		inline void APIorder();
	private:
		glm::mat4 m_ProjMat;
		glm::mat4 m_ViewMat;
		glm::mat4 m_VPM;

		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f);

	};
}
