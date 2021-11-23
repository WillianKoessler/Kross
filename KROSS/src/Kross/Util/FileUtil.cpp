#include "Kross_pch.h"
#include "FileUtil.h"

#include <string>

const char* Kross::ReadFile(const char* cfilepath)
{
	std::string filepath(cfilepath);
	std::ifstream inFile(filepath);
	if (!inFile.is_open())
		throw std::runtime_error("RUNTIME ERROR -> Unable to open file: " + filepath);

	std::stringstream stream;
	stream << inFile.rdbuf();
	return stream.str().c_str();
}


inline const char* Kross::FileName(const char* cpath)
{
	std::string path(cpath);
	size_t slash = path.find_last_of("/\\");
	slash = (slash == std::string::npos) ? 0 : slash + 1;
	size_t dot = path.rfind('.');
	size_t count = (dot == std::string::npos) ? path.size() - slash : dot - slash;
	return (path.substr(slash, count)).c_str();
}

