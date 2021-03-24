#include "Kross_pch.h"
#include "FileUtil.h"

std::string Kross::ReadFile(const std::string& filepath)
{
	std::ifstream inFile(filepath);
	if (!inFile.is_open())
		throw std::runtime_error("RUNTIME ERROR -> Unable to open file: " + filepath);

	std::stringstream stream;
	stream << inFile.rdbuf();
	return stream.str();
}
