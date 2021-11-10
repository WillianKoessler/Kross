#pragma once

#include "glm/glm/gtc/matrix_transform.hpp"
#include "Kross/Renderer/RendererAPI.h"
#include <string>

namespace Kross::Camera {

#define CAMERA_TYPE(type)\
	static Type GetStaticType() { return Type::##type; }\
	virtual Type GetType() const override { return GetStaticType(); }\
	virtual const std::string GetCameraName() const override { return (const std::string)#type; }

	class Camera
	{
	public:
		enum Axis { X, Y, Z };

	public:
		virtual void SetPosition(const glm::vec3& pos) = 0;
		virtual void SetRotation(const glm::vec3& rot) = 0;
		virtual void SetRotation(float angle, Axis a) = 0;
		virtual void SetProjMat(float f1, float f2, float f3, float f4) = 0; // for more detailed information about arguments, look on the specific type you're using

		virtual const glm::mat4& GetProjMat()	const = 0;
		virtual const glm::mat4& GetViewMat()	const = 0;
		virtual const glm::mat4& GetVPM()		const = 0;
		virtual const glm::vec3& GetPosition()	const = 0;
		virtual const glm::vec3& GetRotation()	const = 0;

		enum class Type
		{
			None = 0, Orthographic_2D, Perspective_3D
		};
		
		virtual Type GetType() const = 0;
		virtual const std::string GetCameraName() const = 0;
		virtual const std::string& GetName() const = 0;

		inline static Camera* GetSelf() { return self; }
		inline static void SetSelf(Camera* other) { self = other; }
		Camera(const std::string& name) : m_strName(name) {}

	private:
		static Camera* self;

	protected:
		std::string m_strName;

		glm::mat4 m_ProjMat;
		glm::mat4 m_ViewMat = glm::mat4(1.0f);
		glm::mat4 m_VPM;

		glm::vec3 m_Position = glm::vec3(0.0f);
		glm::vec3 m_Rotation = glm::vec3(0.0f);
	};

}
