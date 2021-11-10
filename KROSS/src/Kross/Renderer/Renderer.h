#pragma once
#include "Kross/Core/Core.h"
#include "RendererAPI.h"
#include "Cameras/Camera.h"
#include "Shaders.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

namespace Kross {
	struct KAPI QuadParams
	{
		glm::vec3				position = { 0.0f, 0.0f, 0.0f };
		glm::vec2				size = { 1.0f, 1.0f };
		Ref<Texture::T2D>		texture = nullptr;
		Ref<Texture::T2DAtlas>	subTexture = nullptr;
		glm::vec4				color = { 1.0f, 1.0f, 1.0f, 1.0f };
		float					rotation = 0.0f;
	};
	struct KAPI CubeParams
	{
		glm::vec3 position = glm::vec3(0.0f);
		glm::vec3 size = glm::vec3(1.0f);
		glm::vec4 color = glm::vec4(1.0f);
	};

	class KAPI Renderer
	{
	public:
		class Scene
		{
			struct Data
			{
				glm::mat4 ViewProjectionMatrix;
			};
		public:
			static void Begin(Camera::Camera& camera);
			static void End();
			static Data* m_Data;
		};
		class Command
		{
		public:
			inline static void Shutdown()
			{
				s_RendererAPI->Shutdown();
			}
			inline static void Init()
			{
				s_RendererAPI->Init();
			}
			inline static void Clear()
			{
				s_RendererAPI->Clear();
			}
			inline static void SetClear(const glm::vec4& color)
			{
				s_RendererAPI->SetClear(color);
			}
			inline static void SetClear(const unsigned int color)
			{
				s_RendererAPI->SetClear(color);
			}
			inline static void DrawIndexed(const Ref<VertexArray>& va)
			{
				s_RendererAPI->DrawIndexed(va);
			}
			inline static void DrawIndexed(const Scope<VertexArray>& va, bool drawPoints = false)
			{
				s_RendererAPI->DrawIndexed(va, drawPoints);
			}
			inline static void OnWindowResize(uint32_t width, uint32_t height)
			{
				s_RendererAPI->SetViewport(width, height);
			}
			inline static void SetMode(Kross::RendererAPI::Mode mode)
			{
				s_RendererAPI->SetMode(mode);
			}
		private:
			static RendererAPI* s_RendererAPI;
		};
		class ShaderLibrary
		{
		public:
			void Clear();
			void Add(const Ref<Shader>& shader);
			void Add(const std::string& name, const Ref<Shader>& shader);
			Ref<Shader> Load(const Ref<Shader>& shader);
			Ref<Shader> Load(const std::string& filepath);
			Ref<Shader> Load(const std::string& name, const std::string& filepath);
			Ref<Shader> Get(const std::string& name);

		private:
			inline const bool exist(const std::string& name) const { return shaders.find(name) != shaders.end(); }

			std::unordered_map<std::string, Ref<Shader>> shaders;
		};
		enum Dimentions { D2 = 0, D3 };

	public:
		static void Shutdown();
		static void Init(Dimentions type);
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4 transform = glm::mat4(1.0f));
		static void Submit(const Ref<Shader>& shader, const Scope<VertexArray>& va, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static Ref<ShaderLibrary> GetShaderLibrary();
	private:
		static Ref<ShaderLibrary> shaderLibrary;
		static Dimentions s_dDims;
	};
}
