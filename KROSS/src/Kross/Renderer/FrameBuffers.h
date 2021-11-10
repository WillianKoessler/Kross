#pragma once
#include "Kross/Core/Core.h"

namespace Kross {
	class KAPI FrameBuffers
	{
	public:
		virtual ~FrameBuffers() = default;

		virtual void Bind() const = 0;
		virtual void unBind() const = 0;

		static Ref<FrameBuffers> CreateRef();
		static Scope<FrameBuffers> CreateScope();
	};
}
