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

		virtual const std::string& GetName() const = 0;

		virtual void SetIntV	(const std::string& name, size_t count, int* value)const = 0;
		virtual void SetInt		(const std::string& name, int value)			   const = 0;
		virtual void SetInt2	(const std::string& name, const glm::vec2& values) const = 0;
		virtual void SetInt3	(const std::string& name, const glm::vec3& values) const = 0;
		virtual void SetInt4	(const std::string& name, const glm::vec4& values) const = 0;
		virtual void SetFloat	(const std::string& name, float value			 ) const = 0;
		virtual void SetFloat2	(const std::string& name, const glm::vec2& values) const = 0;
		virtual void SetFloat3	(const std::string& name, const glm::vec3& values) const = 0;
		virtual void SetFloat4	(const std::string& name, const glm::vec4& values) const = 0;
		virtual void SetMat3	(const std::string& name, const glm::mat3& matrix) const = 0;
		virtual void SetMat4	(const std::string& name, const glm::mat4& matrix) const = 0;

		static Ref<Shader> CreateRef(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		static Ref<Shader> CreateRef(const std::string& name, const std::initializer_list<std::string>& sources);
		static Ref<Shader> CreateRef(const std::string& name, const std::string& filepath);
		static Ref<Shader> CreateRef(const std::string& filepath);
		static Scope<Shader> CreateScope(const std::string& name, const std::string& vertexSource, const std::string& fragmentSource);
		static Scope<Shader> CreateScope(const std::string& name, const std::initializer_list<std::string>& sources);
		static Scope<Shader> CreateScope(const std::string& name, const std::string& filepath);
		static Scope<Shader> CreateScope(const std::string& filepath);
	};
}
