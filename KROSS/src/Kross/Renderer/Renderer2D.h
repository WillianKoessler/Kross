#pragma once

#include "Cameras/Camera.h"
#include "Textures/Textures.h"
#include "Textures/Atlas.h"
#include "Shaders.h"

namespace Kross {
	struct QuadParams;
	class Renderer2D
	{
		static bool s_bSceneBegan;
		static bool s_bBatch;
		static bool s_bInitiated;

	public:
		struct Stats
		{
			uint32_t DrawCount = 0;
			uint32_t QuadCount = 0;
		};

		Renderer2D() = delete;
		Renderer2D(Renderer2D&& other) = delete;
		Renderer2D& operator =(Renderer2D&& other) = delete;

		static const Stats& getStats();
		static void ResetStats();

		static void Init();
		static void Shutdown();

		static void Begin(Ref<Camera::Camera>& camera);
		static void Begin(const Camera::Camera& camera);
		static void BatchBegin();
		static void Flush();
		static void BatchEnd();
		static void End();

		static void SwitchShader(const Ref<Shader>& shader);

		static void BatchQuad(const QuadParams& params);
		static void BatchQuad(const glm::mat4& transform, const glm::vec4& color);
		static void BatchQuad(const glm::mat4& transform, const Ref<Texture::T2D>& color);
	};

}
