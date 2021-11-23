#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Util/Singleton.h"

// TODO: BIG CHANGES HERE (remove std::string && std::vector)

namespace Kross {

	// Kross::Shader || Kross::Texture::T2D  || Kross::Mesh::Base
	template<class T>
	class KAPI Stack
	{
	public:
		class Entry
		{
#define __decl__op__(r, op, type, other)		r operator op(type& o) { return (key op other);}
			const char* stradd(const char* src1, const char* src2)
			{
				uint32_t size1 = strlen(src1);
				uint32_t size2 = strlen(src2);
				char* dest = malloc(size1 + size2);
				memmove(dest, src1, size1);
				memmove(dest + size1, src2, size2);
				return dest;
			}
			static const char* table;
			const char* key, path;
			Ref<T> resource;
		public:
			Entry(const char* key, const Ref<T>& res, const char* path = "NULL")
				:
				key(key),
				path(path),
				resource(res)
			{
				table = stradd(table, path); //table += "\n	" + key + " | " + path;
			}
			~Entry() { resource.reset(); };
			static const char* GetTable() { return table; }
			__decl__op__(const bool, <=, const Entry, o.key);
			__decl__op__(const bool, >=, const Entry, o.key);
			__decl__op__(const bool, < , const Entry, o.key);
			__decl__op__(const bool, > , const Entry, o.key);
			__decl__op__(const bool, ==, const Entry, o.key);
			__decl__op__(const bool, !=, const Entry, o.key);
			__decl__op__(const bool, <=, const char*, o);
			__decl__op__(const bool, >=, const char*, o);
			__decl__op__(const bool, < , const char*, o);
			__decl__op__(const bool, > , const char*, o);
			__decl__op__(const bool, ==, const char*, o);
			__decl__op__(const bool, !=, const char*, o);
			Ref<T> operator ->() { return resource; }
			operator const char*() { return key; }
			operator const Ref<T>&() { return resource; }
			operator Ref<T>& () { return resource; }
			Ref<T>& operator *() { return resource; }
		};

	public:
		static const Stack<T>& instance()
		{
			static Stack<T> instance;
			return instance;
		}

		static const bool Add(const Ref<T>& resource);
		static const bool Del(const char* key);

		static const Ref<T> Get(const char* key) { return _Get(key, nullptr); }
		static const Ref<T> Get(const char* key, const char* filepath) { return _Get(key, filepath); }

		static void Log() { KROSS_CORE_TRACE("[Kross::Stack<{1}>] TABLE{0}", Entry::GetTable(), typeid(T).name()); }

		static void clear() { stack.clear(); }
		static const auto begin() { return stack.begin(); }
		static const auto end() { return stack.end(); }

		Stack(Stack<T>&& other) = delete;
		void operator =(Stack<T>&& other) = delete;

	private:
		~Stack() { KROSS_CORE_INFO("[Kross::Stack<{0}>] Destroyed", typeid(T).name()); clear(); }

		Stack() { KROSS_CORE_INFO("[Kross::Stack<{0}>] Created", typeid(T).name()); }

		static Ref<T> _Get(const char* k, const char* filepath);

		static auto location(const char* key)
		{
			return std::lower_bound(stack.begin(), stack.end(), key);
		}
		static std::vector<Entry> stack;
	};
}
