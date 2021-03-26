#pragma once

#include "Cameras/Camera.h"
#include "Textures/Textures.h"
#include "Shaders.h"

namespace Kross {
	struct QuadParams
	{
		glm::vec3			position	= { 0.0f, 0.0f, 0.0f };
		glm::vec2			size		= { 1.0f, 1.0f };
		Ref<Texture::T2D>	texture		= NULL;
		glm::vec4			color		= { 1.0f, 1.0f, 1.0f, 1.0f };
		float				rotation	= 0.0f;
		glm::vec2			texOffSet	= { 0.0f, 0.0f };
		glm::vec2			texSubSize	= { 1.0f, 1.0f };
	};

	class Renderer2D
	{
		static bool SceneBegan;
		static bool batch;

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

		static void Begin(Camera::Camera& camera);
		static void BatchBegin();
		static void Flush();
		static void BatchEnd();
		static void End();

		static void SwitchShader(const Ref<Shader>& shader);

		static void BatchQuad(const QuadParams& params);
	};

}
