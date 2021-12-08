#include <Kross_pch.h>
#include "GLShader.h"
#include "GLErrors.h"

#include <fstream>
#include <glm/gtc/type_ptr.hpp>
#include "Kross/Util/Util.h"

namespace Kross::OpenGL {
	void Shader::CreateShader(const char *cfilepath)
	{
		KROSS_PROFILE_FUNC();
		KROSS_INFO("Creating Shader '{0}'...", GetName());
		std::string filepath(cfilepath);
		if (filepath.rfind('.') != std::string::npos)
		{
			const char *src = ReadFile(cfilepath);
			if (!strcmp(src, ""))
				if (Compile(PreProcess(src)))
				{
					KROSS_INFO("Shader '{0}' Created Successfully", GetName());
					return;
				}
		}
		KROSS_WARN("Shader '{0}' Failed to Create", GetName());
	}
	Shader::Shader(const char *filepath)
		: m_RendererID(UINT32_MAX)
	{
		KROSS_PROFILE_FUNC();
		SetName(FileName(filepath));
		CreateShader(filepath);
	}
	Shader::Shader(const char *name, const char *filepath)
		: m_RendererID(UINT32_MAX)
	{
		KROSS_PROFILE_FUNC();
		SetName(name);
		CreateShader(filepath);
	}
	Shader::Shader(const char *name, const char *vertexSource, const char *fragmentSource)
		: m_RendererID(UINT32_MAX)
	{
		KROSS_PROFILE_FUNC();
		SetName(name);
		KROSS_INFO("Creating Shader '{0}'...", GetName());
		uint32_t program;
		glCall(program = glCreateProgram());
		uint32_t vsh = Compile(program, GL_VERTEX_SHADER, vertexSource);
		uint32_t fsh = Compile(program, GL_FRAGMENT_SHADER, fragmentSource);
		if (vsh && fsh && Link(program, { vsh, fsh }))
			KROSS_INFO("Shader '{0}' Created Successfully", GetName());
		else
			KROSS_WARN("Shader '{0}' Failed to Create", GetName());
	}
	Shader::Shader(const char *name, const std::initializer_list<const char *> &sources)
		: m_RendererID(UINT32_MAX)
	{
		KROSS_PROFILE_FUNC();
		SetName(name);
		KROSS_INFO("Creating Shader '{0}'...", GetName());
		if (sources.size() < 1) { KROSS_ERROR("List of Sources must have at least 1 source file."); return; }

		uint32_t programID, i = 0u;
		std::vector<uint32_t> shaders;

		glCall(programID = glCreateProgram());

		for (const std::string &path : sources)
			shaders.push_back(Compile(programID, GetGlType(path.substr(path.find_last_of('.')).c_str()), ReadFile(path.c_str())));

		bool valid = true;
		for (int i = 0; i < sources.size(); i++) if (!shaders[i]) valid = false;

		if (valid && Link(programID, shaders)) KROSS_INFO("Shader '{0}' Created Successfully", GetName());
		else KROSS_WARN("Shader '{1}' Failed to Create", GetName());
	}
	Shader::~Shader()
	{
		KROSS_PROFILE_FUNC();
		KROSS_INFO("Deleting Shader '{0}' ...", GetName());
		unBind();
//		for (int64_t i = m_UniformCache.size(); i > -1; i--) delete m_UniformCache[i];
		m_UniformCache.clear();
		if (m_RendererID != UINT32_MAX)
		{
			glCall(glDeleteProgram(m_RendererID));
			KROSS_INFO("Shader '{0}' Deleted Successfully", GetName());
		} else {
			KROSS_WARN("Trying to delete a Invalid Shader '{1}'", GetName());
		}
	}
	//const char *Shader::ReadFile(const char *filepath)
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
	//		return result.c_str();
	//	} else
	//	{
	//		KROSS_MSGBOX_WARN("Could not open file: {0}", filepath);
	//		return "";
	//	}
	//}
	std::unordered_map<uint32_t, const char *> Shader::PreProcess(const char *csource)
	{
		KROSS_PROFILE_FUNC();
		std::unordered_map<GLenum, const char *> sources;
		std::string source(csource);

		const char *maskType = "#type";
		size_t lengthMaskType = strlen(maskType);
		size_t pos = source.find(maskType, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			if (eol == std::string::npos) KROSS_MSGBOX_ERROR("line : {0} Syntax Error", pos);
			size_t begin = pos + lengthMaskType + 1;
			const char *type = source.substr(begin, eol - begin).c_str();
			if (!GetGlType(type))
				KROSS_MSGBOX_ERROR("Invalid shader type specification: {0}", type);
			size_t nextlinepos = source.find_first_not_of("\r\n", eol);
			pos = source.find(maskType, nextlinepos);
			sources[GetGlType(type)] =
				source.substr(
				nextlinepos,
				pos - (nextlinepos == std::string::npos ? source.size() - 1 : nextlinepos)
				).c_str();
		}
		return sources;
	}
	uint32_t Shader::GetGlType(const char *type)
	{
		if (!strcmp(type, "vertex") || !strcmp(type, ".vert"))
			return GL_VERTEX_SHADER;
		if (!strcmp(type, "fragment") || !strcmp(type, "pixel") || !strcmp(type, ".frag"))
			return GL_FRAGMENT_SHADER;
		if (!strcmp(type, "geometry") || !strcmp(type, ".geom"))
			return GL_GEOMETRY_SHADER;
		if (!strcmp(type, "compute") || !strcmp(type, ".comp"))
			return GL_COMPUTE_SHADER;
		if (!strcmp(type, "tess_eval") || !strcmp(type, ".tese"))
			return GL_TESS_EVALUATION_SHADER;
		if (!strcmp(type, "tess_ctrl") || !strcmp(type, ".tesc"))
			return GL_TESS_CONTROL_SHADER;

		KROSS_MSGBOX_ERROR("Unknown shader type: {0}", type);
		return 0;
	}
	const char *Shader::GetShaderType(uint32_t type)
	{
		switch (type)
		{
			case GL_VERTEX_SHADER:			return "Vertex Shader";
			case GL_FRAGMENT_SHADER:		return "Fragment Shader";
			case GL_GEOMETRY_SHADER:		return "Geometry Shader";
			case GL_TESS_CONTROL_SHADER:	return "Tesselation Shader";
			case GL_TESS_EVALUATION_SHADER:	return "Evaluation Shader";
			case GL_COMPUTE_SHADER:			return "Compute Shader";
		}
		return "Unknown Shader";
	}
	bool Shader::Compile(const std::unordered_map<uint32_t, const char *> &sources)
	{
		KROSS_PROFILE_FUNC();
		KROSS_TRACE("Compiling Shader '{0}'...", GetName());

		GLuint program;
		glCall(program = glCreateProgram());
		KROSS_TRACE("Compiling {0} shaders.", sources.size());

		std::vector<GLenum> shadersIDs;
		int index = 0;
		for (auto &kv : sources)
		{
			GLenum type = kv.first;
			const char *source = kv.second;

			GLuint shader;
			glCall(shader = glCreateShader(type));

			glCall(glShaderSource(shader, 1, &source, 0));

			glCall(glCompileShader(shader));

			GLint isCompiled = 0;
			glCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));
				char *infoLog = new char[maxLength];
				glCall(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));
				glCall(glDeleteShader(shader));
				KROSS_WARN("Shader '{0}' failed to Compile", GetName());
				KROSS_ERROR("[{0}]\n{1}", infoLog);
				return false;
			}
			glCall(glAttachShader(program, shader));
			//shaderIDs[index++] = shader;
			shadersIDs.push_back(shader);
		}

		KROSS_TRACE("Shader '{0}' Compiled Successfully!", GetName());

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
			//if (shaderIDs.size() > 0)
				//for (GLenum id : shaderIDs)
			if (shadersIDs.size() > 0)
				for (GLenum id : shadersIDs)
				{
					glCall(glDeleteShader(id));
				}
			KROSS_WARN("Shader Failed to Link");
			KROSS_ERROR("[{0}]", infoLog);
			return false;
		} else
		{
			m_RendererID = program;

			//if (shaderIDs.size() > 0)
			//	for (GLenum id : shaderIDs)
			if (shadersIDs.size() > 0)
				for (GLenum id : shadersIDs)
					glCall(glDetachShader(program, id));
			Bind();
		}
		return true;
	}
	uint32_t Shader::Compile(uint32_t program, uint32_t type, const char *source)
	{
		GLuint shader;
		glCall(shader = glCreateShader(type));

		const char *shtype = GetShaderType(type);

		glCall(glShaderSource(shader, 1, &source, 0));

		glCall(glCompileShader(shader));

		GLint OK = 0;
		glCall(glGetShaderiv(shader, GL_COMPILE_STATUS, &OK));
		if (!OK)
		{
			GLint maxLength = 0;
			glCall(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));
			char *log = new char[maxLength];
			glCall(glGetShaderInfoLog(shader, maxLength, &maxLength, &log[0]));
			glCall(glDeleteShader(shader));
			KROSS_WARN("Shader '{0}' failed to Compile. ({1})", GetName(), shtype);
			KROSS_ERROR("{0} -> {1}", shtype, log);
			KROSS_ERROR("Source: {0}", source);
			return 0;
		}
		glCall(glAttachShader(program, shader));
		KROSS_TRACE("Shader '{0}' Compiled Successifuly. ({1})", GetName(), shtype);
		return shader;
	}
	bool Shader::Link(uint32_t program, const std::vector<uint32_t> &shaders)
	{
		glCall(glLinkProgram(program));
		GLint OK = 0;
		glCall(glGetProgramiv(program, GL_LINK_STATUS, (int *)&OK));
		if (!OK)
		{
			GLint maxLength = 0;
			glCall(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));
			char *log = new char[maxLength];
			glCall(glGetProgramInfoLog(program, maxLength, &maxLength, &log[0]));
			glCall(glDeleteProgram(program));
			if (shaders.size() > 0)
				for (GLenum id : shaders)
				{
					glCall(glDeleteShader(id));
				}
			KROSS_WARN("Shader '{0}' Failed to Link", GetName());
			KROSS_ERROR("[{0}]\n{1}", log);
			return false;
		} else
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
	void Shader::SetIntV(const char *name, size_t count, int *value)	const
	{
		UploadUniformIntV(name, count, value);
	}
	void Shader::SetInt(const char *name, int value)					const { UploadUniformInt(name, value); }
	void Shader::SetInt2(const char *name, const glm::vec2 &values)		const { UploadUniformInt2(name, values); }
	void Shader::SetInt3(const char *name, const glm::vec3 &values)		const { UploadUniformInt3(name, values); }
	void Shader::SetInt4(const char *name, const glm::vec4 &values)		const { UploadUniformInt4(name, values); }
	void Shader::SetFloat(const char *name, float value)				const { UploadUniformFloat(name, value); }
	void Shader::SetFloat2(const char *name, const glm::vec2 &values)	const { UploadUniformFloat2(name, values); }
	void Shader::SetFloat3(const char *name, const glm::vec3 &values)	const { UploadUniformFloat3(name, values); }
	void Shader::SetFloat4(const char *name, const glm::vec4 &values)	const { UploadUniformFloat4(name, values); }
	void Shader::SetMat3(const char *name, const glm::mat3 &matrix)		const { UploadUniformMat3(name, matrix); }
	void Shader::SetMat4(const char *name, const glm::mat4 &matrix)		const { UploadUniformMat4(name, matrix); }
	void Shader::UploadUniformIntV(const char *name, size_t count, int *value) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			if (!value)
			{
				int *defaults = new int[count];
				for (size_t i = 0; i < count; i++) defaults[i] = (int)i;
				KROSS_WARN("Nullptr value. Setting defaults.");
				glCall(glUniform1iv(GetUniformLocation(name), (GLsizei)count, defaults));
				delete defaults;
			} else
			{
				glCall(glUniform1iv(GetUniformLocation(name), (GLsizei)count, value));
			}
		}
	}
	void Shader::UploadUniformInt(const char *name, int value) const { if (m_RendererID != UINT32_MAX) glCall(glUniform1i(GetUniformLocation(name), value)); }
	void Shader::UploadUniformInt2(const char *name, const glm::vec2 &values)	const { if (m_RendererID != UINT32_MAX) glCall(glUniform2i(GetUniformLocation(name), (int)values.x, (int)values.y)); }
	void Shader::UploadUniformInt3(const char *name, const glm::vec3 &values)	const { if (m_RendererID != UINT32_MAX) glCall(glUniform3i(GetUniformLocation(name), (int)values.x, (int)values.y, (int)values.z)); }
	void Shader::UploadUniformInt4(const char *name, const glm::vec4 &values)	const { if (m_RendererID != UINT32_MAX) glCall(glUniform4i(GetUniformLocation(name), (int)values.x, (int)values.y, (int)values.z, (int)values.w)); }
	void Shader::UploadUniformFloat(const char *name, float value)				const { if (m_RendererID != UINT32_MAX) glCall(glUniform1f(GetUniformLocation(name), value)); }
	void Shader::UploadUniformFloat2(const char *name, const glm::vec2 &values) const { if (m_RendererID != UINT32_MAX) glCall(glUniform2f(GetUniformLocation(name), values.x, values.y)); }
	void Shader::UploadUniformFloat3(const char *name, const glm::vec3 &values) const { if (m_RendererID != UINT32_MAX) glCall(glUniform3f(GetUniformLocation(name), values.x, values.y, values.z)); }
	void Shader::UploadUniformFloat4(const char *name, const glm::vec4 &values) const { if (m_RendererID != UINT32_MAX) glCall(glUniform4f(GetUniformLocation(name), values.x, values.y, values.z, values.w)); }
	void Shader::UploadUniformMat3(const char *name, const glm::mat3 &matrix)	const { if (m_RendererID != UINT32_MAX) glCall(glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix))); }
	void Shader::UploadUniformMat4(const char *name, const glm::mat4 &matrix)	const { if (m_RendererID != UINT32_MAX) glCall(glUniformMatrix4fv(GetUniformLocation(name), 1, false, glm::value_ptr(matrix))); }
	int Shader::GetUniformLocation(const char *name) const
	{
		if (m_RendererID != UINT32_MAX)
		{
			for (UniformLocation *uniform : m_UniformCache) if (uniform->name == name) return uniform->location;
			int location;
			glCall(location = glGetUniformLocation(m_RendererID, name));
			m_UniformCache.push_back(new UniformLocation(name, location));
			return location;
		}
		return -1;
	}
}