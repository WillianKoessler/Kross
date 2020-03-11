#pragma once
#include "Kross/Renderer/Cameras/Camera.h"

namespace Kross::Camera {
	class Ortho2D : public Camera
	{
	public:
		Ortho2D(float left, float right, float bottom, float top, const std::string& name = "blank");
		~Ortho2D();

		virtual void SetPosition(const glm::vec3& pos) override;
		virtual void SetRotation(float angle) override;
		virtual void SetProjMat(float left, float right, float bottom, float top) override;

		virtual const glm::mat4& GetVPM()		const override { return m_VPM;		}
		virtual const glm::mat4& GetProjMat()	const override { return m_ProjMat;	}
		virtual const glm::mat4& GetViewMat()	const override { return m_ViewMat;	}
		virtual const glm::vec3& GetPosition()	const override { return m_Position;	}
		virtual const float GetRotation()		const override { return m_Rotation;	}

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

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		float m_Rotation = 0.0f;

	};
}
