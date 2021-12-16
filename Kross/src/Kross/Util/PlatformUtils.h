#pragma once

namespace Kross {
	class FileDialog
	{
	public:
		static const char *SaveFile(const char *filter);
		static const char *OpenFile(const char *filter);
	};
}
