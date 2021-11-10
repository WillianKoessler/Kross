#pragma once
#include "Kross/Renderer/Cameras/Camera.h"

namespace Kross::Camera2D {
	class Orthographic : public Kross::Camera::Camera
	{
	public:
		Orthographic(float left, float right, float bottom, float top, const std::string& name = "blank");
		~Orthographic();

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
	private:
		inline void RecalculateVPM();
		inline void APIorder();
	};
}
