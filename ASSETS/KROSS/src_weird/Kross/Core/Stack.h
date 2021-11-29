#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	template<typename T>
	class KAPI Stack
	{
	public:
		class Entry
		{
		public:
			Entry(const std::string& key, const Ref<T>& res, const std::string& path = "NULL");
		};
	};
}