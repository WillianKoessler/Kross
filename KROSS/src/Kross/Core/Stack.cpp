#include "Kross_pch.h"
#include "Stack.h"
#include "Kross/Renderer/Shaders.h"
#include "Kross/Renderer/Textures/Textures.h"
#include "Kross/Core/Layer.h"

#define __impl__STACK_TEMPLATE(x)\
const Ref<##x> Stack<##x>::null_value = nullptr;\
std::vector<Stack<##x>::Entry> Stack<##x>::stack;\
std::string Stack<##x>::Entry::table = "";\
template class Stack<##x>;

namespace Kross {
	__impl__STACK_TEMPLATE(Shader);
	//__impl__STACK_TEMPLATE(Layer);
	__impl__STACK_TEMPLATE(Texture::T2D);

	template<typename T> const bool Stack<T>::Add(const Ref<T>& resource)
	{
		const auto i = std::lower_bound(stack.begin(), stack.end(), resource->GetName());
		//const std::vector<Entry>::iterator i = location(resource->GetName());
		if (i != stack.end() && !(resource->GetName() < i->key))
		{
			KROSS_CORE_WARN("Object already exists in stack.", __FUNCTION__);
			//KROSS_CORE_WARN_ONCE("[Kross::Stack::Add(const Ref<T>&)] Object already exists in stack.");
			return false;
		}
		else if (resource.get())
		{
			stack.emplace(i, resource->GetName(), resource);
			return true;
		}
		else return false;
	}
	template<typename T> const bool Stack<T>::Del(const std::string& key)
	{
		const auto i = location(key);
		if (i != stack.end() && !(key < i->key))
		{
			stack.erase(i);
			return true;
		}
		KROSS_CORE_WARN("{0}\n	T= {1}\n Resource named '{2}' was not found, and because of it, could not be deleted.\nEntries are:\n{3}", __FUNCTION__, typeid(T).name(), key, Entry::GetTable());
		//KROSS_CORE_WARN_ONCE("{0} // T={1} // Ref<{1}> // Resource named '{2}' was not found, and because of it, could not be deleted", "[Kross::Stack::Del(const std::string&)]", typeid(T).name(), key);
		return false;
	}
	template<typename T> const Ref<T>& Stack<T>::_Get(const std::string& k, const std::string* filepath)
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
					KROSS_CORE_WARN("{0}\n	T={1}\n	Resource named '{2}' has not a valid filepath, therefore, it could not be loaded.\nEntries are: {3}", __FUNCTION__, typeid(T).name(), k, Entry::GetTable());
					calls.append(mycall);
				}
			}
		}
		else
		{
			if (calls.find(mycall) == std::string::npos)
			{
				KROSS_CORE_WARN("{0}\n	T={1}\n	KEY is a empty, and because of it, resource could not be loaded to Stack.\nEntries are: {2}", __FUNCSIG__, typeid(T).name(), Entry::GetTable());
				calls.append(mycall);
			}
		}
		return null_value;
	}
}
