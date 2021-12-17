#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	struct KAPI File { const char *name, *path, *extension; };
	class KAPI FileDialog
	{
	public:
		static File SaveFile(const char *filter);
		static File OpenFile(const char *filter);
	};
}
