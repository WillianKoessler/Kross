#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	template<typename T>
	class KAPI Stack
	{
	public:
		Stack();
		virtual ~Stack() {};

		virtual const bool Add(const Ref<T>& resource) = 0;
		virtual const bool Del(const std::string& key) = 0;

		virtual const Ref<T> Get(const std::string& key) = 0;
		virtual const Ref<T> Get(const std::string& key, const std::string& filepath) = 0;

		virtual void Log() = 0;

		virtual void clear() = 0;
		virtual size_t size() const = 0;

		static Stack<T>& instance();
		//virtual const std::_Vector_iterator<std::_Vector_val<std::_Simple_types<T>>> begin() = 0;
		//virtual const std::_Vector_iterator<std::_Vector_val<std::_Simple_types<T>>> end() = 0;
	};
}
