#include "Kross_pch.h"
#include "Kross/Util/PlatformUtils.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Kross/Core/Application.h"

namespace Kross {
	File FileDialog::SaveFile(const char *filter)
	{
		OPENFILENAMEA ofn;
		const size_t nSize = 128;
		File file{ new char[nSize], new char[nSize] };
		memset(const_cast<char*>(file.name), 0, nSize);
		memset(const_cast<char*>(file.path), 0, nSize);
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = (LPSTR)file.path;
		ofn.lpstrFileTitle = (LPSTR)file.name;
		ofn.nMaxFile = nSize;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
		GetSaveFileNameA(&ofn);
		return file;
	}

	File FileDialog::OpenFile(const char *filter)
	{
		OPENFILENAMEA ofn;
		const size_t nSize = 128;
		char *filepath = new char[nSize];
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = filepath;
		ofn.nMaxFile = nSize;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetOpenFileNameA(&ofn)) {
			std::string path = filepath, name = "", ext = "";
			File file{ new char[nSize], filepath, new char[nSize] };
			memset(const_cast<char *>(file.name), 0, nSize);
			memset(const_cast<char *>(file.extension), 0, nSize);
			size_t slash = path.find_last_of("/\\") + 1;
			size_t dot = path.find_last_of('.');
			if (dot != std::string::npos) {
				name = path.substr(slash, dot - slash);
				ext = path.substr(dot);
				strcpy_s(const_cast<char *>(file.name), nSize, name.c_str());
				strcpy_s(const_cast<char *>(file.extension), nSize, ext.c_str());
			} else {
				name = path.substr(slash);
				strcpy_s(const_cast<char *>(file.name), nSize, name.c_str());
			}
			KROSS_TRACE("Opening file:\n\tNamed : '{0}'\n\tPath : '{1}'\n\tExtension : '{2}'", file.name, file.path, file.extension);
			return file;
		}
		return File{};
	}
}
