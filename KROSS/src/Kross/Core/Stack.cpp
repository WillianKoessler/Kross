#include "Kross_pch.h"
#include "Stack.h"
#include "Kross/Renderer/Shaders.h"
#include "Kross/Renderer/Textures/Textures.h"
#include "Kross/Core/Layer.h"

#define __impl__STACK_TEMPLATE(x)\
std::vector<Stack<##x>::Entry> Stack<##x>::stack;\
Stack<##x>* Stack<##x>::self = nullptr;\
std::string Stack<##x>::Entry::table = "";\
template class Stack<##x>;

namespace Kross {
	__impl__STACK_TEMPLATE(Shader);
	__impl__STACK_TEMPLATE(Texture::T2D);

	template<typename T> const Ref<T> Stack<T>::Add(const Ref<T>& resource)
	{
		const auto i = std::lower_bound(stack.begin(), stack.end(), resource->GetName());
		if (i != stack.end() && !(resource->GetName() < i->key))
		{
			KROSS_CORE_WARN("[ {0} ] |||| Object already exists in stack.", __FUNCTION__);
		}
		else if (resource.get())
		{
			stack.emplace(i, resource->GetName(), resource);
		}
		return resource;
	}
	template<typename T> const bool Stack<T>::Del(const std::string& key)
	{
		const auto i = location(key);
		if (i != stack.end() && !(key < i->key))
		{
			stack.erase(i);
			return true;
		}
		KROSS_CORE_WARN("[ {0} ] ||||\n	T= {1}\n Resource named '{2}' was not found, and because of it, could not be deleted.\nEntries are:\n{3}", __FUNCTION__, typeid(T).name(), key, Entry::GetTable());
		return false;
	}
	template<typename T> Ref<T> Stack<T>::_Get(const std::string& k, const std::string* filepath)
	{
		static std::string calls = "";
		const char* mycall = (" " + k + " " + (filepath ? *filepath : "null")).c_str();

		if (k != "")
		{
			const auto i = location(k);
			if (i != stack.end() && !(k < i->key)) return i->resource;
			else if (filepath)
			{
					return *stack.emplace(i, k, T::CreateRef(*filepath, k), *filepath);
			}
			else
			{
				if (calls.find(mycall) == std::string::npos)
				{
					KROSS_CORE_WARN("[ {0} ] ||||\n	T={1}\n	Resource named '{2}' has not a valid filepath, therefore, it could not be loaded.\nEntries are: {3}", __FUNCTION__, typeid(T).name(), k, Entry::GetTable());
					calls.append(mycall);
				}
			}
		}
		else
		{
			if (calls.find(mycall) == std::string::npos)
			{
				KROSS_CORE_WARN("[ {0} ] ||||\n	T={1}\n	KEY is a empty, and because of it, resource could not be loaded to Stack.\nEntries are: {2}", __FUNCTION__, typeid(T).name(), Entry::GetTable());
				calls.append(mycall);
			}
		}
		return nullptr;
	}
}
