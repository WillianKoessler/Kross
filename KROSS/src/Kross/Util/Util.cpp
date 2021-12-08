#include "Kross_pch.h"
#include "Util.h"

const char* Kross::append(const char* str1, const char* str2)
{
	return nullptr;
	//std::string buffer(str1);
	//buffer += str2;
	//char *result = new char[buffer.size()];
	//memcpy(result, buffer.data(), buffer.size());
	//delete[] str1, str2; //does not work.
	//return result;

	//size_t d = dest!=nullptr ? strlen(dest) : 0;
	//size_t t = type!=nullptr ? strlen(type) : 0;
	//size_t s = str!=nullptr ? strlen(str) : 0;
	//char *buffer = new char[d+t+s+1];	
	//memcpy(buffer, dest, d);
	//memcpy(buffer + d, type, t);
	//memcpy(buffer + d + t, str, s);
	//buffer[d+t+s] = '\0';
	//dest = buffer;
	//return dest;
}

const char* Kross::ReadFile(const char* filepath)
{
	static std::vector<std::string> entries;
	std::ifstream inFile(filepath);
	if (!inFile.is_open())
		KROSS_ERROR("Unable to Open file: {0}", filepath);
	std::stringstream stream;
	stream << inFile.rdbuf();
	inFile.close();
	entries.emplace_back(stream.str());
	return entries.back().c_str();
}

const char *Kross::FileName(const char *cpath)
{
	static std::vector<std::string> entries;
	std::string path(cpath);
	size_t slash = path.find_last_of("/\\");
	slash = (slash == std::string::npos) ? 0 : slash + 1;
	size_t dot = path.rfind('.');
	size_t count = (dot == std::string::npos) ? path.size() - slash : dot - slash;
	entries.emplace_back(path.substr(slash, count));
	return entries.back().c_str();
}
