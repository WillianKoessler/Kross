#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	struct KAPI File
	{
		File() = default;
		File(const char *filepath);
		File(const File &other)
		{
			if (other.path) {
				pathSize = other.pathSize;
				path = new char[pathSize];
				memset(path, 0, pathSize);
				strcpy_s(path, pathSize, other.path);
			}
			if (other.name) {
				nameSize = other.nameSize;
				name = new char[nameSize];
				memset(name, 0, nameSize);
				strcpy_s(name, nameSize, other.name);
			}
			if (other.extension) {
				extensionSize = other.extensionSize;
				extension = new char[extensionSize];
				memset(extension, 0, extensionSize);
				strcpy_s(extension, extensionSize, other.extension);
			}
		}
		~File()
		{
			//if (path != nullptr) delete[] path;
			//if (name != nullptr) delete[] name;
			//if (extension != nullptr) delete[] extension;
		}
		operator bool() const { return path != nullptr; }

		char *path = nullptr;
		char *name = nullptr;
		char *extension = nullptr;
		size_t pathSize = 0, nameSize = 0, extensionSize = 0;
	};


	class KAPI FileDialog
	{
	public:
		static File SaveFile(const char *filter);
		static File OpenFile(const char *filter);
	};
}
