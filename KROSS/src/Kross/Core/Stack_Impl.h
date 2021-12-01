#pragma once

#include "Kross/Core/Core.h"
#include "Kross/Core/Stack.h"
#include <string>
#include <algorithm>
#include <vector>

namespace Kross {

	// Kross::Shader || Kross::Texture::T2D  || Kross::Mesh::Base
	template<typename T>
	class Stack_Impl : public Stack<T>
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
			Ref<T> operator ->()
			{
				return resource;
			}
			operator std::string()
			{
				return key;
			}
			operator const Ref<T>& ()
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
		static Stack_Impl<T>& instance()
		{
			static Stack_Impl<T> instance;
			return instance;
		}

		virtual const bool Add(const Ref<T>& resource) override
		{
			const auto i = std::lower_bound(stack.begin(), stack.end(), resource->GetName());
			if (i != stack.end() && !(resource->GetName() < i->key))
			{
				KROSS_CORE_WARN("Object already exists in stack.");
				return false;
			} else if (resource.get())
			{
				stack.emplace(i, resource->GetName(), resource);
				return true;
			} else
			{
				KROSS_CORE_WARN("EMPTY object emplaced on Stack.");
				return false;
			}
		}
		virtual const bool Del(const std::string& key) override
		{
			const auto i = location(key);
			if (i != stack.end() && !(key < i->key))
			{
				stack.erase(i);
				return true;
			}
			KROSS_CORE_WARN("{0}\n	T= {1}\n Resource named '{2}' was not found, and because of it, could not be deleted.\nEntries are:\n{3}", typeid(T).name(), key, Entry::GetTable());
			return false;
		}
		virtual const Ref<T> Get(const std::string& key) override { return _Get(key, nullptr); }
		virtual const Ref<T> Get(const std::string& key, const std::string& filepath) override { return _Get(key, &filepath); }
		virtual void Log() override { KROSS_CORE_TRACE(" | [Kross::Stack<{1}>] TABLE{0}", Entry::GetTable(), typeid(T).name()); }
		virtual void clear() override { stack.clear(); }
		virtual size_t size() const override { return stack.size(); }
		//virtual const Entry* begin() override { return stack.begin(); }
		//virtual const Entry* end() override { return stack.end(); }

		Stack_Impl(const Stack_Impl& other) = delete;
		Stack_Impl(Stack_Impl&& other) = delete;
		void operator =(Stack_Impl&& other) = delete;
	private:
		Stack_Impl() { KROSS_CORE_INFO(" | [Kross::Stack<{0}>] Created", typeid(T).name()); }
		virtual ~Stack_Impl() override { clear(); KROSS_CORE_INFO(" | [Kross::Stack<{0}>] Destroyed", typeid(T).name()); }

		static Ref<T> _Get(const std::string& k, const std::string* filepath)
		{
			static std::string calls = "";
			const char* mycall = (" " + k + " " + (filepath ? *filepath : "null")).c_str();

			if (!k.empty())
			{
				const auto i = location(k);
				if (i != stack.end() && !(k < i->key)) return i->resource;
				else if (filepath)
				{
					return *stack.emplace(i, k, T::CreateRef(k, *filepath), *filepath);
				} else
				{
					if (calls.find(mycall) == std::string::npos)
					{
						KROSS_CORE_WARN("\n\tT={0}\n\tResource named '{1}' has not a valid filepath, therefore, it could not be loaded.\n\tEntries are: {2}", typeid(T).name(), k, Entry::GetTable());
						calls.append(mycall);
					}
				}
			} else
			{
				if (calls.find(mycall) == std::string::npos)
				{
					KROSS_CORE_WARN("\n\tT={0}\n\tKEY is a empty, and because of it, resource could not be loaded to Stack.\n\tEntries are: {1}", typeid(T).name(), Entry::GetTable());
					calls.append(mycall);
				}
			}
			return nullptr;
		}
		static auto location(const std::string& key)
		{
			return std::lower_bound(stack.begin(), stack.end(), key);
		}

		static std::vector<Entry> stack;
	};
}
