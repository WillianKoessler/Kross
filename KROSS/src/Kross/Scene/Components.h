#pragma once

#include <glm/glm.hpp>

#include "Kross/Renderer/Cameras/Camera.h"

namespace Kross {
	struct TransformComponent
	{
		glm::mat4 Transform = glm::mat4(1.0f);
		TransformComponent() = default;
		TransformComponent(const TransformComponent&) = default;
		TransformComponent(const glm::mat4& mat) : Transform(mat) {}
		operator glm::mat4& () { return Transform; }
		operator const glm::mat4& () const { return Transform; }
	};

	struct SpriteComponent
	{
		glm::vec4 tint = glm::vec4(1.0f);
		SpriteComponent() = default;
		SpriteComponent(const SpriteComponent&) = default;
		SpriteComponent(const glm::vec4& tintColor) : tint(tintColor) {}
	};

	struct TagComponent
	{
		const char* tag = "TAG_NULL";
		TagComponent() = default;
		TagComponent(const TagComponent&) = default;
		TagComponent(const char* tag) : tag(tag) {}
	};
	struct CameraComponent
	{
		Camera::Camera camera;
		CameraComponent() = default;
		CameraComponent(const CameraComponent&) = default;
		CameraComponent(const Camera::Camera& camera) : camera(camera) {}
	};
}