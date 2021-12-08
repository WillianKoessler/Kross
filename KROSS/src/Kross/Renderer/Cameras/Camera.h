#pragma once

#include "glm/glm/gtc/matrix_transform.hpp"
#include "Kross/Renderer/RendererAPI.h"

namespace Kross::Camera {

	class Camera
	{
	public:
		Camera() = default;
		Camera(const glm::mat4& projection) : m_ProjMat(projection) {}
		~Camera() {}

		virtual void SetProjMat(const glm::mat4 &projectionMatrix) { m_ProjMat = projectionMatrix; };
		virtual const glm::mat4 &GetProjMat() const { return m_ProjMat; };

	protected:
		glm::mat4 m_ProjMat = glm::mat4(1.0f);
	};

}
