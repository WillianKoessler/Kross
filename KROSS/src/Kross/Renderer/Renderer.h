#pragma once
#include "Kross/Core/Core.h"
#include "RendererAPI.h"
#include "Cameras/Camera.h"
#include "Shaders.h"
#include "Renderer2D.h"

namespace Kross {

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
			inline static void DrawIndexed(const Scope<VertexArray>& va)
			{
				s_RendererAPI->DrawIndexed(va);
			}
			inline static void OnWindowResize(uint32_t width, uint32_t height)
			{
				s_RendererAPI->SetViewport(width, height);
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

	public:
		static void Shutdown();
		static void Init();
		static void Submit(const Ref<Shader>& shader, const Ref<VertexArray>& va, const glm::mat4 transform = glm::mat4(1.0f));
		static void Submit(const Ref<Shader>& shader, const Scope<VertexArray>& va, const glm::mat4 transform = glm::mat4(1.0f));

		inline static RendererAPI::API GetAPI() { return RendererAPI::GetAPI(); }
		static Ref<ShaderLibrary> GetShaderLibrary();
	private:
		static Ref<ShaderLibrary> shaderLibrary;
	};
}
