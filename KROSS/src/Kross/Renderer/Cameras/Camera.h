#pragma once

#include "glm/glm/gtc/matrix_transform.hpp"
#include "Kross/Renderer/RendererAPI.h"
#include "Kross/Core/Resource.h"

namespace Kross::Camera {

#define CAMERA_TYPE(type)\
	static Type GetStaticType() { return Type::##type; }\
	virtual Type GetType() const override { return GetStaticType(); }\

	class Camera : public Resource
	{
	public:
		Camera() { SetName("Unnamed_Camera"); }
		enum class Axis { X, Y, Z };

		virtual bool isLookingAt() const = 0;
		virtual void LockAt(const glm::vec3& target) = 0;
		virtual void unLock() = 0;

		virtual void SetPosition(const glm::vec3& pos) = 0;
		virtual void SetRotation(const glm::vec3& rot) = 0;
		virtual void SetRotation(float angle, Axis a)  = 0;
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
	};

}
