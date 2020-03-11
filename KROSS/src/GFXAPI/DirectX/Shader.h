#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Renderer/Shaders.h"
#include "DXBasics.h"

namespace Kross::DirectX {
	class KAPI Shader : public Kross::Shader
	{
	public:
		Shader(const std::string& name);
		//Shader(const std::string& filepath);
		//Shader(const std::string& name, const std::string& filepath);
		//Shader(const std::string& name, const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		virtual void Bind() const override;
		virtual void unBind() const override;

		virtual void SetInt   (const std::string& name, int value)				 const override;
		virtual void SetInt2  (const std::string& name, const glm::vec2& values) const override;
		virtual void SetInt3  (const std::string& name, const glm::vec3& values) const override;
		virtual void SetInt4  (const std::string& name, const glm::vec4& values) const override;

		virtual void SetFloat (const std::string& name, float value)			 const override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& values) const override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& values) const override;
		virtual void SetFloat4(const std::string& name, const glm::vec4& values) const override;

		virtual void SetMat3  (const std::string& name, const glm::mat3& matrix) const override;
		virtual void SetMat4  (const std::string& name, const glm::mat4& matrix) const override;


		void UploadUniformInt(const std::string& name, int value) const;
		void UploadUniformInt2(const std::string& name, const glm::vec2& values) const;
		void UploadUniformInt3(const std::string& name, const glm::vec3& values) const;
		void UploadUniformInt4(const std::string& name, const glm::vec4& values) const;

		void UploadUniformFloat(const std::string& name, float value) const;
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values) const;
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values) const;
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values) const;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const;
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const;

		int GetUniformLocation(const std::string& name) const;
		
		virtual const std::string& GetName() const override { return m_strName; }
	private:
		void Compile(unsigned int type, const BYTE* source);

		//std::string ReadFile(const std::string& filepath);
		//const BYTE* PreProcess(const std::string& source);
		//static unsigned int GetShaderType(const std::string& type);
		//void CreateShader(const std::string& filepath);
		//struct UniformLocation
		//{
		//	UniformLocation(const std::string& n, int l)
		//		: name(n), location(l)
		//	{}
		//	std::string name;
		//	int location;
		//};

	private:
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;

		static const char* area;
		//unsigned int m_RendererID;
		std::string m_strName;
		//mutable std::vector<UniformLocation*> UniformCache;
	};
}