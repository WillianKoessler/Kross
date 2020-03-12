#pragma once

#include "Cameras/Camera.h"
#include "Textures/Textures.h"

namespace Kross {
	struct QuadParams
	{
		glm::vec2			position	= { 0.0f, 0.0f };
		glm::vec2			size		= { 1.0f, 1.0f };
		Ref<Texture::T2D>	texture		= NULL;
		glm::vec4			color		= { 1.0f, 1.0f, 1.0f, 1.0f };
		float				repeat		= 1;
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


		//static void DrawQuad(const glm::vec2& position, float size,				const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void DrawQuad(const glm::vec3& position, float size,				const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void DrawQuad(const glm::vec2& position, const glm::vec2& size,	const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void DrawQuad(const glm::vec3& position, const glm::vec2& size,	const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		////
		//static void DrawQuad(const glm::vec2& position, float size,				const Ref<Texture::T2D>& texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void DrawQuad(const glm::vec3& position, float size,				const Ref<Texture::T2D>& texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void DrawQuad(const glm::vec2& position, const glm::vec2& size,	const Ref<Texture::T2D>& texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void DrawQuad(const glm::vec3& position, const glm::vec2& size,	const Ref<Texture::T2D>& texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		////
		////
		//static void DrawRotatedQuad(const glm::vec2& position, float size,				float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void DrawRotatedQuad(const glm::vec3& position, float size,				float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,	float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size,	float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		////
		//static void DrawRotatedQuad(const glm::vec2& position, float size,				const Ref<Texture::T2D>& texture, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void DrawRotatedQuad(const glm::vec3& position, float size,				const Ref<Texture::T2D>& texture, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size,	const Ref<Texture::T2D>& texture, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size,	const Ref<Texture::T2D>& texture, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		////
		////
		////
		//static void BatchQuad(const glm::vec2& position, float size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void BatchQuad(const glm::vec3& position, float size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void BatchQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void BatchQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		////		
		//static void BatchQuad(const glm::vec2& position, float size, const Ref<Texture::T2D>& texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void BatchQuad(const glm::vec3& position, float size, const Ref<Texture::T2D>& texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void BatchQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture::T2D>& texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void BatchQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture::T2D>& texture, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);

		static void BatchQuad(const QuadParams& params);


		//static void BatchRotatedQuad(const glm::vec2& position, float size, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void BatchRotatedQuad(const glm::vec3& position, float size, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void BatchRotatedQuad(const glm::vec2& position, const glm::vec2& size, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		//static void BatchRotatedQuad(const glm::vec3& position, const glm::vec2& size, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f });
		////
		//static void BatchRotatedQuad(const glm::vec2& position, float size, const Ref<Texture::T2D>& texture, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void BatchRotatedQuad(const glm::vec3& position, float size, const Ref<Texture::T2D>& texture, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void BatchRotatedQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture::T2D>& texture, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);
		//static void BatchRotatedQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture::T2D>& texture, float rotation = 0, const glm::vec4& color = { 1.0f, 1.0f, 1.0f, 1.0f }, float repeat = 1);

	};

}
