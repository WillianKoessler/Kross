#pragma once

#include "Kross/Core/Core.h"
#include <string>
#include <algorithm>
#include <vector>

namespace Kross {

	// Kross::Shader || Kross::Texture::T2D  || Kross::Mesh::Base
	template<typename T>
	class Stack
	{
	public:
		class Entry
		{
#define __decl__op__(r, op, type, other)		r operator op(type& o) { return (key op other);}
		public:
			Entry(const char *ckey, const Ref<T> &res, const char *cpath = "NULL")
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
			static const std::string &GetTable() { return table; }

			__decl__op__(const bool, <= , const Entry, o.key);
			__decl__op__(const bool, >= , const Entry, o.key);
			__decl__op__(const bool, < , const Entry, o.key);
			__decl__op__(const bool, > , const Entry, o.key);
			__decl__op__(const bool, == , const Entry, o.key);
			__decl__op__(const bool, != , const Entry, o.key);
			__decl__op__(const bool, <= , const std::string, o);
			__decl__op__(const bool, >= , const std::string, o);
			__decl__op__(const bool, < , const std::string, o);
			__decl__op__(const bool, > , const std::string, o);
			__decl__op__(const bool, == , const std::string, o);
			__decl__op__(const bool, != , const std::string, o);

			Ref<T> operator ->() { return resource; }
			operator std::string() { return key; }
			operator const Ref<T> &() { return resource; }
			operator Ref<T> &() { return resource; }
			Ref<T> &operator *() { return resource; }
		};

	public:
		static const bool Add(const Ref<T> &resource)
		{
			if (resource.get() == nullptr || resource == nullptr) {
				KROSS_CORE_WARN("Trying to emplace EMPTY resource on Stack.");
				return false;
			}

			const auto i = location(resource->GetName());
			if (valid(resource->GetName(), i)) {
				KROSS_CORE_WARN("Resource already exists in stack.");
				return false;
			} else {
				stack.emplace(i, resource->GetName(), resource);
				KROSS_CORE_TRACE("Resource named '{0}' added to the Stack.", resource->GetName());
				return true;
			}
		}
		static const bool Del(const char *key)
		{
			const auto i = location(key);
			if (valid(key, i))
			{
				stack.erase(i);
				KROSS_CORE_TRACE("Resource named '{0}' deleted from the Stack.", key);
				return true;
			}
			KROSS_CORE_WARN("{0}\n	T= {1}\n Resource named '{2}' was not found, and because of it, could not be deleted.\nEntries are:\n{3}", typeid(T).name(), key, Entry::GetTable());
			return false;
		}
		static const Ref<T> Get(const char *key) { return _Get(key, nullptr); }
		static const Ref<T> Get(const char *key, const char *filepath) { return _Get(key, filepath); }
		static void Log() { KROSS_CORE_TRACE("TABLE{0}", Entry::GetTable()); }
		static void clear() { stack.clear(); KROSS_CORE_INFO("Stack Cleared."); }
		static size_t size() { return stack.size(); }

	private:
		static Ref<T> _Get(const char *k, const char *filepath)
		{
			KROSS_CORE_TRACE("{0}", k);
			if (k && strcmp(k, "") != 0)
			{
				const auto i = location(k);
				if (valid(k, i)) return i->resource;
				else if (filepath && strcmp(filepath, "") != 0) return stack.emplace(i, k, T::CreateRef(k, filepath), filepath)->resource;
				else KROSS_CORE_WARN("\n\tT={0}\n\tResource named '{1}' has a not valid filepath, therefore, it could not be loaded.\n\tEntries are: {2}", typeid(T).name(), k, Entry::GetTable());
			} else KROSS_CORE_WARN("\n\tT={0}\n\tKEY is a empty, and because of it, resource could not be loaded to Stack.\n\tEntries are: {1}", typeid(T).name(), Entry::GetTable());
			return nullptr;
		}
		static auto location(const char *key) { return std::lower_bound(stack.begin(), stack.end(), key); }
		template<typename iter> static bool valid(const char* key, const iter &i)
		{
			KROSS_TRACE("{0}", typeid(iter).name());
			bool valid = false;
			if (i != stack.end())
				if(key != nullptr)
					if (i->key.compare(key) != 0)
					{ valid = true; KROSS_TRACE("{0}", key); }
			return valid;
		}
		static std::vector<Entry> stack;
	};
}
