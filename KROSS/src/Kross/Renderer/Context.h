#pragma once

namespace Kross {
	class Context
	{
	public:
		virtual void Init(unsigned int, unsigned int) = 0;
		virtual void SwapBuffers() = 0;
	};
}