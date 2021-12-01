#include <Kross_pch.h>
#include "Shader.h"

#include "Context.h"

namespace Shaders
{
#include "C:/dev/C++/RPG Project/CORE_Development/Sandbox/assets/shaders/DirectX/FramebufferPS.shh"
#include "C:/dev/C++/RPG Project/CORE_Development/Sandbox/assets/shaders/DirectX/FramebufferVS.shh"
}

#define DX_VERTEX_SHADER 1u
#define DX_FRAGMENT_SHADER 2u
#define DX_PIXEL_SHADER DX_FRAGMENT_SHADER



	//void Shader::CreateShader(const std::string& filepath)
	//{
	//	KROSS_PROFILE_FUNC();
	//	KROSS_CORE_INFO("Creating Shader '{1}'...", area, m_strName);
	//	const std::string& src = ReadFile(filepath);
	//	if (src == "")
	//	{
	//		KROSS_CORE_WARN("Shader '{1}' Failed to Create", area, m_strName);
	//	}
	//	else
	//	{
	//		size_t nl = src.find_first_of('\n'), pnl;
	//		const BYTE* vertexShader = PreProcess(ReadFile(src.substr(0, nl)));
	//		pnl = nl + 1;
	//		nl = src.find_first_of('\n');
	//		const BYTE* pixelShader = PreProcess(ReadFile(src.substr(pnl, nl)));
	//		Compile(DX_VERTEX_SHADER, vertexShader);
	//		Compile(DX_PIXEL_SHADER, pixelShader);
	//		KROSS_CORE_INFO("Shader '{1}' Created Successfully", area, m_strName);
	//	}
	//	//KROSS_CORE_TRACE("Shader '{0}', ID {1}", m_strName, m_RendererID);
	//}
	//
	//Shader::Shader(const std::string& filepath)
	//	: m_RendererID(UINT32_MAX)
	//{
	//	KROSS_PROFILE_FUNC();
	//	size_t slash = filepath.find_last_of("/\\");
	//	slash = (slash == std::string::npos) ? 0 : slash + 1;
	//	size_t dot = filepath.rfind('.');
	//	size_t count = (dot == std::string::npos) ? filepath.size() - slash : dot - slash;
	//	m_strName = filepath.substr(slash, count);
	//	CreateShader(filepath);
	//}
	//
	//Shader::Shader(const std::string& name, const std::string& filepath)
	//	: m_RendererID(UINT32_MAX),
	//	m_strName(name)
	//{
	//	KROSS_PROFILE_FUNC();
	//	CreateShader(filepath);
	//}
	//
	//Shader::Shader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
	//	: m_RendererID(UINT32_MAX),
	//	m_strName(name)
	//{
	//	KROSS_PROFILE_FUNC();
	//	KROSS_CORE_INFO("Creating Shader '{1}'...", area, m_strName);
	//	std::unordered_map<unsigned int, const BYTE*> sources;
	//	sources[DX_VERTEX_SHADER] = PreProcess(ReadFile(vertexSource));
	//	sources[DX_FRAGMENT_SHADER] = PreProcess(ReadFile(fragmentSource));
	//	Compile(sources);
	//	KROSS_CORE_INFO("Shader '{1}' Created Successfully", area, m_strName);
	//	//KROSS_CORE_TRACE("Shader '{0}', ID {1}", m_strName, m_RendererID);
	//}
	//
	//Shader::~Shader()
	//{
	//	KROSS_PROFILE_FUNC();
	//	if (m_RendererID != UINT32_MAX)
	//	{
	//		KROSS_CORE_INFO("Deleting Shader '{1}' ...", area, m_strName);
	//		unBind();
	//		glCall(glDeleteProgram(m_RendererID));
	//		UniformCache.clear();
	//		KROSS_CORE_INFO("Shader '{1}' Deleted Successfully", area, m_strName);
	//	}
	//	else
	//	{
	//		UniformCache.clear();
	//		KROSS_CORE_WARN("Trying to delete a Invalid Shader '{1}'", area, m_strName);
	//	}
	//}
	//
	//std::string Shader::ReadFile(const std::string& filepath)
	//{
	//	KROSS_PROFILE_FUNC();
	//	std::string result;
	//	std::ifstream in(filepath, std::ios::in | std::ios::binary);
	//	if (in)
	//	{
	//		in.seekg(0, std::ios::end);
	//		result.resize(in.tellg());
	//		in.seekg(0, std::ios::beg);
	//		in.read(&result[0], result.size());
	//		in.close();
	//		return result;
	//	}
	//	else
	//	{
	//		KROSS_MSGBOX("Could not open file " + filepath, area, _ERROR_);
	//		return "";
	//	}
	//}
	//
	//const BYTE* Shader::PreProcess(const std::string& source)
	//{
	//	KROSS_PROFILE_FUNC();
	//	std::vector<int> buffer;
	//
	//	const char* maskType = "#type";
	//	size_t lengthMaskType= strlen(maskType);
	//	size_t pos = source.find(maskType, 0);
	//	if (pos != std::string::npos)
	//	{
	//		size_t eol = source.find_first_of("\r\n", pos);
	//		if (eol == std::string::npos)
	//		{
	//			KROSS_MSGBOX("line : " + pos, "Syntax Error", _ERROR_);
	//			return nullptr;
	//		}
	//		size_t begin = pos + lengthMaskType + 1;
	//		std::string type = source.substr(begin, eol - begin);
	//		if (!GetShaderType(type))
	//		{
	//			KROSS_MSGBOX("Invalid shader type specification: " + type, area, _ERROR_);
	//			return nullptr;
	//		}
	//
	//		size_t nextlinepos = source.find_first_not_of("\r\n", eol);
	//		pos = source.find(maskType, nextlinepos);
	//		std::string src = 
	//			source.substr(
	//				nextlinepos, 
	//				pos - (nextlinepos == std::string::npos ? source.size() - 1 : nextlinepos)
	//			);
	//
	//		size_t nl, pnl = 0;
	//		while (nl != std::string::npos) {
	//			nl = src.find_first_of(',', pnl);
	//			buffer.push_back(std::stoi(src.substr(pnl, nl - pnl)));
	//			pnl = nl + 1;
	//		}
	//		auto a = buffer.data();
	//	}
	//	else
	//	{
	//		KROSS_CORE_ERROR("Cannot pre-process shader file {1}: FILE EMPTY", area, source);
	//		return nullptr;
	//	}
	//	return sources;
	//}
	//
	//unsigned int Shader::GetShaderType(const std::string & type)
	//{
	//	if(type == "vertex")
	//		return DX_VERTEX_SHADER;
	//	if (type == "fragment" || type == "pixel")
	//		return DX_PIXEL_SHADER;
	//
	//	KROSS_MSGBOX("Unknown shader type: " + type, area, _ERROR_);
	//	return 0;
	//}
namespace Kross::DirectX {
	const char* Shader::area = "[Kross::DirectX::Shader]";

	Shader::Shader(const std::string& name)
	{
		Compile(DX_VERTEX_SHADER, Shaders::PixelShaderBytecode);
		Compile(DX_VERTEX_SHADER, Shaders::VertexShaderBytecode);
	}


	void Shader::Compile(unsigned int type, const BYTE* source)
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_TRACE("Compiling Shader '{1}'...", m_strName);
		HRESULT hr;
		switch (type)
		{
		case DX_VERTEX_SHADER:
		{
			if (FAILED(hr = Context::Get().GetDevice()->CreateVertexShader(
				Shaders::VertexShaderBytecode,
				sizeof(Shaders::VertexShaderBytecode),
				nullptr,
				&pVertexShader)))
			{
				KROSS_CORE_ERROR("Failed to compile Vertex Shader");
			}
		}
		case DX_PIXEL_SHADER:
		{
			if (FAILED(hr = Context::Get().GetDevice()->CreatePixelShader(
				Shaders::PixelShaderBytecode,
				sizeof(Shaders::PixelShaderBytecode),
				nullptr,
				&pPixelShader)))
			{
				KROSS_CORE_ERROR("Failed to compile Pixel Shader", area);
			}
		}
		}
	}

	void Shader::Bind() const
	{
		KROSS_PROFILE_FUNC();
	}

	void Shader::unBind() const
	{
		KROSS_PROFILE_FUNC();
	}

	void Shader::SetInt(const std::string& name, int value) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformInt(name, value);
	}

	void Shader::SetInt2(const std::string& name, const glm::vec2& values) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformInt2(name, values);
	}

	void Shader::SetInt3(const std::string& name, const glm::vec3& values) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformInt3(name, values);
	}

	void Shader::SetInt4(const std::string& name, const glm::vec4& values) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformInt4(name, values);
	}

	void Shader::SetFloat(const std::string& name, float value) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformFloat(name, value);
	}

	void Shader::SetFloat2(const std::string& name, const glm::vec2& values) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformFloat2(name, values);
	}

	void Shader::SetFloat3(const std::string& name, const glm::vec3& values) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformFloat3(name, values);
	}

	void Shader::SetFloat4(const std::string& name, const glm::vec4& values) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformFloat4(name, values);
	}

	void Shader::SetMat3(const std::string& name, const glm::mat3& matrix) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformMat3(name, matrix);
	}

	void Shader::SetMat4(const std::string& name, const glm::mat4& matrix) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformMat4(name, matrix);
	}

	void Shader::UploadUniformInt(const std::string& name, int value) const
	{
	}

	void Shader::UploadUniformInt2(const std::string& name, const glm::vec2& values) const
	{
	}

	void Shader::UploadUniformInt3(const std::string& name, const glm::vec3& values) const
	{
	}

	void Shader::UploadUniformInt4(const std::string& name, const glm::vec4& values) const
	{
	}

	void Shader::UploadUniformFloat(const std::string& name, float value) const
	{
	}

	void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& values) const
	{
	}

	void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& values) const
	{
	}

	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& values) const
	{
	}

	void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
	{
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
	}
}