#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Stack.h"

#include <string>
#include <algorithm>
#include <vector>

namespace Kross {

	template<typename T>
	class Stack_Impl : public Stack
	{
	public:

	};

	// Kross::Shader || Kross::Texture::T2D  || Kross::Mesh::Base
	template<class T>
	class _Stack
	{
	public:
		class Entry
		{
#define __decl__op__(r, op, type, other)		r operator op(type& o) { return (key op other);}
		public:
			Entry(const std::string& key, const Ref<T>& res, const std::string& path = "NULL")
				:
				key(key),
				path(path),
				resource(res)
			{
				table += "\n	" + key + " | " + path;
			}
			~Entry() { resource.reset(); };
			
			std::string key, path;
			Ref<T> resource;

			static std::string table;


			static const std::string& GetTable() { return table; }


			__decl__op__(const bool, <=, const Entry, o.key);
			__decl__op__(const bool, >=, const Entry, o.key);
			__decl__op__(const bool, < , const Entry, o.key);
			__decl__op__(const bool, > , const Entry, o.key);
			__decl__op__(const bool, ==, const Entry, o.key);
			__decl__op__(const bool, !=, const Entry, o.key);
			__decl__op__(const bool, <=, const std::string, o);
			__decl__op__(const bool, >=, const std::string, o);
			__decl__op__(const bool, < , const std::string, o);
			__decl__op__(const bool, > , const std::string, o);
			__decl__op__(const bool, ==, const std::string, o);
			__decl__op__(const bool, !=, const std::string, o);
			Ref<T> operator ->()
			{
				return resource;
			}
			operator std::string()
			{
				return key;
			}
			operator const Ref<T>&()
			{
				return resource;
			}
			operator Ref<T>& ()
			{
				return resource;
			}
			Ref<T>& operator *()
			{
				return resource;
			}
		};

	public:
		static const Stack<T>& instance()
		{
			static Stack<T> instance;
			return instance;
		}

		static const bool Add(const Ref<T>& resource);
		static const bool Del(const std::string& key);

		static const Ref<T> Get(const std::string& key) { return _Get(key, nullptr); }
		static const Ref<T> Get(const std::string& key, const std::string& filepath) { return _Get(key, &filepath); }

		static void Log() { KROSS_CORE_TRACE("[Kross::Stack<{1}>] TABLE{0}", Entry::GetTable(), typeid(T).name()); }

		static void clear() { stack.clear(); }
		static const auto begin() { return stack.begin(); }
		static const auto end() { return stack.end(); }

		//Stack(Stack<T>&& other) = delete;
		//void operator =(Stack<T>&& other) = delete;

	private:
		~Stack() { KROSS_CORE_INFO("[Kross::Stack<{0}>] Destroyed", typeid(T).name()); clear(); }

		Stack() { KROSS_CORE_INFO("[Kross::Stack<{0}>] Created", typeid(T).name()); }

		static Ref<T> _Get(const std::string& k, const std::string* filepath);

		static auto location(const std::string& key)
		{
			return std::lower_bound(stack.begin(), stack.end(), key);
		}
		static std::vector<Entry> stack;
	};
}
