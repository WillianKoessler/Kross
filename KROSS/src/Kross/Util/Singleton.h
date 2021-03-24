#pragma once

#include "NonCopyable.h"
#include "NonMovable.h"

namespace Kross {
	class Singleton : public NonCopyable, public NonMovable
	{
	public:
		virtual Singleton& get() = 0;
	};
}