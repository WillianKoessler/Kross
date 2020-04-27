#pragma once

#include "Cameras/Camera.h"
#include "Textures/Textures.h"
#include "Textures/FrameBuffer.h"

namespace Kross {
	class Renderer3D
	{
	public:
		static const Ref<Texture::FrameBuffer> GetFrameBuffer();

		static bool Is_Initiated();
		static void Init();
		static void Shutdown();

		static void Begin(Camera::Camera& camera);
		static void End();

		static void DrawCube(const glm::vec3& position, const glm::vec3& scale = { 0.1f,0.1f,0.1f }, const glm::vec4& color = { 1.0f,1.0f,1.0f,1.0f });
		static void DrawCube(const glm::vec3& position, const glm::vec3& scale, float angle, const glm::vec3& rotation_axis, const glm::vec4& color);
		static void DrawCube(const glm::vec3& position, const glm::vec3& scale, const glm::mat4& rotation, const glm::vec4& color);

	};
}