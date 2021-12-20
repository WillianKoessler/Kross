#include "Kross_pch.h"
#include "Kross/Util/PlatformUtils.h"

#include <commdlg.h>
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include "Kross/Core/Application.h"

namespace Kross {
	File::File(const char *filepath)
	{
		if (filepath && strcmp(filepath, "")!=0) {
			pathSize = strlen(filepath) + 1;
			path = new char[pathSize];
			strcpy_s(path, pathSize, filepath);

			std::string sPath = path, sName, sExt;
			size_t slash = sPath.find_last_of("/\\");
			slash = slash != std::string::npos ? slash + 1 : 0;
			size_t dot = sPath.find_last_of('.');

			sName = sPath.substr(slash, dot == std::string::npos ? 0 : dot - slash);
			nameSize = sName.size() + 1;
			name = new char[nameSize];
			memset(name, 0, nameSize);
			if(nameSize > 1) strcpy_s(name, nameSize, sName.c_str());

			if (dot != std::string::npos) {
				sExt = sPath.substr(dot, std::string::npos);
				extensionSize = sExt.size() + 1;
				extension = new char[extensionSize];
				memset(extension, 0, extensionSize);
				if(extensionSize > 1) strcpy_s(extension, extensionSize, sExt.c_str());
			}
		} else {
			KROSS_ERROR("Attempt to create File object with invalid filepath");
		}
	}

	//void File::Mount(uint32_t size)
	//{
	//	if (!path) { KROSS_ERROR("Attempt to mount file from without filepath"); }
	//	name = new char[size];
	//	extension = new char[size];
	//	std::string sPath = path, sName, sExt;
	//	size_t slash = sPath.find_last_of("/\\") + 1;
	//	size_t dot = sPath.find_last_of('.');
	//	if (dot != std::string::npos) {
	//		sName = sPath.substr(slash, dot - slash);
	//		sName = sPath.substr(dot);
	//		strcpy_s(name, size, sName.c_str());
	//		strcpy_s(extension, size, sExt.c_str());
	//	} else {
	//		sName = sPath.substr(slash);
	//		strcpy_s(name, size, sName.c_str());
	//	}
	//}
	File FileDialog::SaveFile(const char *filter)
	{
		OPENFILENAMEA ofn;
		const size_t nSize = 128;
		CHAR filepath[nSize] = { 0 };
		File file;
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = filepath;
		ofn.nMaxFile = nSize;
		ofn.lpstrFilter = filter;
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn))
			file = File(filepath);
		return file;
	}

	File FileDialog::OpenFile(const char *filter)
	{
		OPENFILENAMEA ofn;
		const size_t nSize = 128;
		CHAR filepath[nSize] = { 0 };
		ZeroMemory(&ofn, sizeof(OPENFILENAMEA));
		ofn.lStructSize = sizeof(OPENFILENAMEA);
		ofn.hwndOwner = glfwGetWin32Window((GLFWwindow *)Application::Get().GetWindow().GetNativeWindow());
		ofn.lpstrFile = filepath;
		ofn.nMaxFile = nSize;
		ofn.lpstrFilter = filter;
		ofn.lpstrTitle = "Open File";
		ofn.nFilterIndex = 1;
		ofn.Flags = OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_NOCHANGEDIR;
		if (GetSaveFileNameA(&ofn)) 
			return File(filepath);
		return File{};
	}
}
