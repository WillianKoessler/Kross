#pragma once

namespace Kross {
	class NonMovable
	{
	public:
		NonMovable(NonMovable&&) = delete;
		NonMovable& operator=(NonMovable&&) = delete;
	protected:
		NonMovable() = default;
	};
}