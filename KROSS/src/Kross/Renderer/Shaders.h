#pragma once

#include "Kross/Core/Core.h"

#include "glm/glm/fwd.hpp"

namespace Kross {
	enum class ShaderType
	{
		None = 0,
		OpenGL
	};

	class Shader
	{
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void unBind() const = 0;

		virtual const char* GetName() const = 0;

		virtual void SetIntV	(const char* name, size_t count, int* value)const = 0;
		virtual void SetInt		(const char* name, int value)			   const = 0;
		virtual void SetInt2	(const char* name, const glm::vec2& values) const = 0;
		virtual void SetInt3	(const char* name, const glm::vec3& values) const = 0;
		virtual void SetInt4	(const char* name, const glm::vec4& values) const = 0;
		virtual void SetFloat	(const char* name, float value			 ) const = 0;
		virtual void SetFloat2	(const char* name, const glm::vec2& values) const = 0;
		virtual void SetFloat3	(const char* name, const glm::vec3& values) const = 0;
		virtual void SetFloat4	(const char* name, const glm::vec4& values) const = 0;
		virtual void SetMat3	(const char* name, const glm::mat3& matrix) const = 0;
		virtual void SetMat4	(const char* name, const glm::mat4& matrix) const = 0;

		static Ref<Shader> CreateRef(const char* name, const char* vertexSource, const char* fragmentSource);
		static Ref<Shader> CreateRef(const char* name, const std::initializer_list<std::string>& sources);
		static Ref<Shader> CreateRef(const char* name, const char* filepath);
		static Ref<Shader> CreateRef(const char* filepath);
		static Scope<Shader> CreateScope(const char* name, const char* vertexSource, const char* fragmentSource);
		static Scope<Shader> CreateScope(const char* name, const std::initializer_list<std::string>& sources);
		static Scope<Shader> CreateScope(const char* name, const char* filepath);
		static Scope<Shader> CreateScope(const char* filepath);
	};
}
