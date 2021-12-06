#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Shaders.h"

namespace Kross::OpenGL {
	class Shader final : public Kross::Shader
	{
		struct UniformLocation
		{
			UniformLocation(const char* n, int l)
				: name(n), location(l)
			{}
			const char* name;
			int location;
		};

	public:
		Shader(const char* filepath);
		Shader(const char* name, const char* filepath);
		Shader(const char* name, const char* vertexSrc, const char* fragmentSrc);
		Shader(const char* name, const std::initializer_list<const char*>& sources);
		~Shader();

		virtual void Bind() const override;
		virtual void unBind() const override;

		virtual void SetIntV  (const char* name, size_t count, int* value)const override;
		virtual void SetInt   (const char* name, int value)				  const override;
		virtual void SetInt2  (const char* name, const glm::vec2& values) const override;
		virtual void SetInt3  (const char* name, const glm::vec3& values) const override;
		virtual void SetInt4  (const char* name, const glm::vec4& values) const override;

		virtual void SetFloat (const char* name, float value)			  const override;
		virtual void SetFloat2(const char* name, const glm::vec2& values) const override;
		virtual void SetFloat3(const char* name, const glm::vec3& values) const override;
		virtual void SetFloat4(const char* name, const glm::vec4& values) const override;

		virtual void SetMat3  (const char* name, const glm::mat3& matrix) const override;
		virtual void SetMat4  (const char* name, const glm::mat4& matrix) const override;

		void UploadUniformIntV	(const char* name, size_t count, int* value)const;
		void UploadUniformInt	(const char* name, int value)			    const;
		void UploadUniformInt2	(const char* name, const glm::vec2& values) const;
		void UploadUniformInt3	(const char* name, const glm::vec3& values) const;
		void UploadUniformInt4	(const char* name, const glm::vec4& values) const;
		void UploadUniformFloat (const char* name, float value)			    const;
		void UploadUniformFloat2(const char* name, const glm::vec2& values) const;
		void UploadUniformFloat3(const char* name, const glm::vec3& values) const;
		void UploadUniformFloat4(const char* name, const glm::vec4& values) const;
		void UploadUniformMat3	(const char* name, const glm::mat3& matrix) const;
		void UploadUniformMat4	(const char* name, const glm::mat4& matrix) const;

		int GetUniformLocation(const char* name) const;
	private:
		//const char* ReadFile(const char* filepath);
		std::unordered_map<uint32_t, const char*> PreProcess(const char* source);
		static uint32_t GetGlType(const char* type);
		static const char* GetShaderType(uint32_t type);
		bool Compile(const std::unordered_map<uint32_t, const char*>& sources);
		uint32_t Compile(uint32_t program, uint32_t type, const char* source);
		bool Link(uint32_t program, const std::vector<uint32_t>& shaders);
		void CreateShader(const char* filepath);

	private:
		uint32_t m_RendererID;
		mutable std::vector<UniformLocation*> m_UniformCache;
	};
}