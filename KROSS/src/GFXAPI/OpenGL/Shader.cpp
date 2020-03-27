#include <Kross_pch.h>
#include "Shader.h"
#include "GLErrors.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>

namespace Kross::OpenGL {	
	void Shader::CreateShader(const std::string& filepath)
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[{0}] Creating Shader '{1}'...", __FUNCTION__, m_strName);
		if (filepath.rfind('.') != std::string::npos)
		{
			const std::string& src = ReadFile(filepath);
			if (src != "")
			{
				if (Compile(PreProcess(src)))
				{
					KROSS_CORE_INFO("[{0}] Shader '{1}' Created Successfully", __FUNCTION__, m_strName);
					return;
				}
			}
		}
		else
		{
			const std::string& vertex = ReadFile(filepath + ".vert");
			const std::string& fragment = ReadFile(filepath + ".frag");
			if (vertex != "" && fragment != "")
			{
				unsigned int program;
				glCall(program = glCreateProgram());
				unsigned int vsh = Compile(program, GL_VERTEX_SHADER, vertex);
				unsigned int fsh = Compile(program, GL_FRAGMENT_SHADER, fragment);
				if (vsh && fsh && Link(program, { vsh, fsh }))
				{
					KROSS_CORE_INFO("[{0}] Shader '{1}' Created Successfully", __FUNCTION__, m_strName);
					return;
				}
			}
		}
		KROSS_CORE_WARN("[{0}] Shader '{1}' Failed to Create", __FUNCTION__, m_strName);
	}
	Shader::Shader(const std::string& filepath)
		: m_RendererID(UINT32_MAX)
	{
		KROSS_PROFILE_FUNC();
		m_strName = FileName(filepath);
		CreateShader(filepath);
	}
	Shader::Shader(const std::string& name, const std::string& filepath)
		: m_RendererID(UINT32_MAX),
		m_strName(name)
	{
		KROSS_PROFILE_FUNC();
		CreateShader(filepath);
	}
	Shader::Shader(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource)
		: m_RendererID(UINT32_MAX),
		m_strName(name)
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_INFO("[{0}] Creating Shader '{1}'...", __FUNCTION__, m_strName);
		unsigned int program;
		glCall(program = glCreateProgram());
		unsigned int vsh = Compile(program, GL_VERTEX_SHADER, vertexSource);
		unsigned int fsh = Compile(program, GL_FRAGMENT_SHADER, fragmentSource);
		if (vsh && fsh && Link(program, { vsh, fsh }))
			KROSS_CORE_INFO("[{0}] Shader '{1}' Created Successfully", __FUNCTION__, m_strName);
		else
			KROSS_CORE_WARN("[{0}] Shader '{1}' Failed to Create", __FUNCTION__, m_strName);
		//std::unordered_map<GLenum, std::string> sources;
		//sources[GL_VERTEX_SHADER] = vertexSource;
		//sources[GL_FRAGMENT_SHADER] = fragmentSource;
		//if(Compile(sources))
		//	KROSS_CORE_INFO("[{0}] Shader '{1}' Created Successfully", __FUNCTION__, m_strName);
		//else
		//	KROSS_CORE_WARN("[{0}] Shader '{1}' Failed to Create", __FUNCTION__, m_strName);
	}
	Shader::~Shader()
	{
		KROSS_PROFILE_FUNC();
		if (m_RendererID != UINT32_MAX)
		{
			KROSS_CORE_INFO("[{0}] Deleting Shader '{1}' ...", __FUNCTION__, m_strName);
			unBind();
			glCall(glDeleteProgram(m_RendererID));
			UniformCache.clear();
			KROSS_CORE_INFO("[{0}] Shader '{1}' Deleted Successfully", __FUNCTION__, m_strName);
		}
		else
		{
			UniformCache.clear();
			KROSS_CORE_WARN("[{0}] Trying to delete a Invalid Shader '{1}'", __FUNCTION__, m_strName);
		}
	}
	std::string Shader::ReadFile(const std::string& filepath)
	{
		KROSS_PROFILE_FUNC();
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
			return result;
		}
		else
		{
			KROSS_MSGBOX("Could not open file " + filepath, __FUNCTION__, _WARN_);
			return "";
		}
	}
	std::unordered_map<unsigned int, std::string> Shader::PreProcess(const std::string& source)
	{
		KROSS_PROFILE_FUNC();
		std::unordered_map<GLenum, std::string> sources;

		const char* maskType = "#type";
		size_t lengthMaskType= strlen(maskType);
		size_t pos = source.find(maskType, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			if(eol == std::string::npos) KROSS_MSGBOX("line : " + pos, "Syntax Error", _ERROR_);
			size_t begin = pos + lengthMaskType + 1;
			std::string type = source.substr(begin, eol - begin);
			if(!GetGlType(type))
				KROSS_MSGBOX("Invalid shader type specification: " + type, __FUNCTION__, _ERROR_);

			size_t nextlinepos = source.find_first_not_of("\r\n", eol);
			pos = source.find(maskType, nextlinepos);
			sources[GetGlType(type)] = 
				source.substr(
					nextlinepos, 
					pos - (nextlinepos == std::string::npos ? source.size() - 1 : nextlinepos)
				);
		}
		return sources;
	}
	unsigned int Shader::GetGlType(const std::string & type)
	{
		if(type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		KROSS_MSGBOX("Unknown shader type: " + type, __FUNCTION__, _ERROR_);
		return 0;
	}
	const char* Shader::GetShaderType(unsigned int type)
	{
		switch (type)
		{
		case GL_VERTEX_SHADER:			return "Vertex Shader";
		case GL_FRAGMENT_SHADER:		return "Fragment Shader";
		case GL_GEOMETRY_SHADER:		return "Geometry Shader";
		case GL_TESS_CONTROL_SHADER:	return "Tesselation Shade";
		case GL_TESS_EVALUATION_SHADER:	return "Evaluation Shader";
		case GL_COMPUTE_SHADER:			return "Compute Shader";
		}
		return "Unknown Shader";
	}
	bool Shader::Compile(const std::unordered_map<unsigned int, std::string>& sources)
	{
		KROSS_PROFILE_FUNC();
		KROSS_CORE_TRACE("[{0}] Compiling Shader '{1}'...", __FUNCTION__, m_strName);
		
		GLuint program;
		glCall(program = glCreateProgram());
		if(sources.size() > 2) KROSS_CORE_WARN("[{0}] {1} Shader Sources it's not supported. Maximum = 2", __FUNCTION__, sources.size());
		std::array<GLenum, 2> shaderIDs;
		int index = 0;
		for (auto& kv : sources)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;
			
			GLuint shader;
			glCall(shader = glCreateShader(type));

			const GLchar* BinarySource = (const GLchar *)source.c_str();
			glCall(glShaderSource(shader, 1, &BinarySource , 0));

			glCall(glCompileShader(shader));

			GLint isCompiled = 0;
			glCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));
				char* infoLog = new char[maxLength];
				glCall(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));
				glCall(glDeleteShader(shader));
				if(!isCompiled) KROSS_CORE_WARN("[{0}] Shader '{1}' failed to Compile", __FUNCTION__, m_strName);
				KROSS_CORE_ERROR("[{0}]\n{1}", __FUNCTION__,  infoLog);
				return false;
			}
			glCall(glAttachShader(program, shader));
			shaderIDs[index++] = shader;
		}

		KROSS_CORE_TRACE("[{0}] Shader '{1}' Compiled Successfully!", __FUNCTION__, m_strName);
		
		glCall(glLinkProgram(program));

		GLint isLinked = 0;
		glCall(glGetProgramiv(program, GL_LINK_STATUS, (int *)&isLinked));
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));
			char *infoLog = new char[maxLength];
			glCall(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));
			glCall(glDeleteProgram(program));
			if (shaderIDs.size() > 0)
				for (GLenum id : shaderIDs)
				{
					glCall(glDeleteShader(id));
				}
			KROSS_CORE_WARN("[{0}] Shader Failed to Link", __FUNCTION__);
			KROSS_CORE_ERROR("[{0}]\n{1}", __FUNCTION__, infoLog);
			return false;
		}
		else
		{
			m_RendererID = program;

			if (shaderIDs.size() > 0)
				for (GLenum id : shaderIDs)
				{
					glCall(glDetachShader(program, id));
				}
			Bind();
		}
		return true;
	}
	unsigned int Shader::Compile(unsigned int program, unsigned int type, const std::string& source)
	{
		GLuint shader;
		glCall(shader = glCreateShader(type));

		const char* shtype = GetShaderType(type);

		const GLchar* BinarySource = (const GLchar*)source.c_str();
		glCall(glShaderSource(shader, 1, &BinarySource, 0));

		glCall(glCompileShader(shader));

		GLint OK = 0;
		glCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &OK));
		if (!OK)
		{
			GLint maxLength = 0;
			glCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));
			char* log = new char[maxLength];
			glCall(glGetShaderInfoLog(shader, maxLength, &maxLength, &log[0]));
			glCall(glDeleteShader(shader));
			KROSS_CORE_WARN("[{0}] Shader '{1}' failed to Compile. ({2})", __FUNCTION__, m_strName, shtype);
			KROSS_CORE_ERROR("[{0}]{2}\n{1}", __FUNCTION__, log, shtype);
			return 0;
		}
		glCall(glAttachShader(program, shader));
		KROSS_CORE_TRACE("[{0}] Shader '{1}' Compiled Successifuly. ({2})", __FUNCTION__, m_strName, shtype);
		return shader;
	}
	bool Shader::Link(unsigned int program, const std::vector<unsigned int>& shaders)
	{
		glCall(glLinkProgram(program));
		GLint OK = 0;
		glCall(glGetProgramiv(program, GL_LINK_STATUS, (int*)&OK));
		if (!OK)
		{
			GLint maxLength = 0;
			glCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));
			char* log = new char[maxLength];
			glCall(glGetProgramInfoLog(program, maxLength, &maxLength, &log[0]));
			glCall(glDeleteProgram(program));
			if (shaders.size() > 0)
				for (GLenum id : shaders)
				{
					glCall(glDeleteShader(id));
				}
			KROSS_CORE_WARN("[{0}] Shader '{1}' Failed to Link", __FUNCTION__, m_strName);
			KROSS_CORE_ERROR("[{0}]\n{1}", __FUNCTION__, log);
			return false;
		}
		else
		{
			m_RendererID = program;
			if (shaders.size() > 0)
				for (GLenum id : shaders)
				{
					glCall(glDetachShader(program, id));
				}
			Bind();
			return true;
		}
	}
	void Shader::Bind() const
	{
		KROSS_PROFILE_FUNC();
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUseProgram(m_RendererID));
		}
	}
	void Shader::unBind() const
	{
		KROSS_PROFILE_FUNC();
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUseProgram(0));
		}
	}
	void Shader::SetIntV(const std::string& name, size_t count, int* value) const
	{
		KROSS_PROFILE_FUNC();
		UploadUniformIntV(name, count, value);
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
	void Shader::UploadUniformIntV(const std::string& name, size_t count, int* value) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			if (!value)
			{
				int* defaults = new int[count];
				for (size_t i = 0; i < count; i++) defaults[i] = (int)i;
				KROSS_CORE_WARN("[{0}] Nullptr value. Setting defaults.", __FUNCTION__);
				glCall(glUniform1iv(GetUniformLocation(name), (GLsizei)count, defaults));
			}
			else
			{
				glCall(glUniform1iv(GetUniformLocation(name), (GLsizei)count, value));
			}
		}
	}
	void Shader::UploadUniformInt(const std::string& name, int value) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUniform1i(GetUniformLocation(name), value));
		}
	}
	void Shader::UploadUniformInt2(const std::string& name, const glm::vec2& values) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUniform2i(GetUniformLocation(name), (int)values.x, (int)values.y));
		}
	}
	void Shader::UploadUniformInt3(const std::string& name, const glm::vec3& values) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUniform3i(GetUniformLocation(name), (int)values.x, (int)values.y, (int)values.z));
		}
	}
	void Shader::UploadUniformInt4(const std::string& name, const glm::vec4& values) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUniform4i(GetUniformLocation(name), (int)values.x, (int)values.y, (int)values.z, (int)values.w));
		}
	}
	void Shader::UploadUniformFloat(const std::string& name, float value) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUniform1f(GetUniformLocation(name), value));
		}
	}
	void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& values) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUniform2f(GetUniformLocation(name), values.x, values.y));
		}
	}
	void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& values) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUniform3f(GetUniformLocation(name), values.x, values.y, values.z));
		}
	}
	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& values) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUniform4f(GetUniformLocation(name), values.x, values.y, values.z, values.w));
		}
	}
	void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix)));
		}
	}
	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix)));
		}
	}
	int Shader::GetUniformLocation(const std::string& name) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			for (UniformLocation* uniform : UniformCache)
			{
				if (uniform->name == name) return uniform->location;
			}
			int location;
			glCall(location = glGetUniformLocation(m_RendererID, name.c_str()));
			UniformCache.push_back(new UniformLocation(name, location));
			return location;
		}
		return -1;
	}
}
