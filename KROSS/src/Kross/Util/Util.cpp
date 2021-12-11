#include "Kross_pch.h"
#include "Util.h"

namespace Kross {
	void append(char *dest, size_t size, const char *str)
	{
		if (!dest || !str) return;
		strcat_s(dest, size, str);

		//std::string buffer(dest);
		//std::string copy = buffer;
		//buffer += str;
		//size_t size = buffer.size();
		//char *result = new char[size];
		//memcpy(result, buffer.c_str(), size);

		//size_t d = dest!=nullptr ? strlen(dest) : 0;
		//size_t s = str!=nullptr ? strlen(str) : 0;
		//char *cbuffer = new char[d+s+1];
		//memset(cbuffer, 0, d + s + 1);
		//memcpy(cbuffer, dest, d);
		//memcpy(cbuffer + d, str, s);
		//cbuffer[d+s] = '\0';
		//char *temp = dest;
		//dest = cbuffer;
		//delete temp;
	}

	const char *ReadFile(const char *filepath)
	{
		std::ifstream inFile(filepath);
		if (!inFile.is_open()) KROSS_ERROR("Unable to Open file: {0}", filepath);
		std::stringstream stream;
		stream << inFile.rdbuf();
		inFile.close();
		std::string str = stream.str();
		size_t size = str.size();
		char *ptr = new char[size+1];
		memcpy(ptr, str.c_str(), size);
		ptr[size] = '\0';
		return ptr;
	}

	const char *FileName(const char *cpath)
	{
		std::string path(cpath);
		size_t slash = path.find_last_of("/\\");
		slash = (slash == std::string::npos) ? 0 : slash + 1;
		size_t dot = path.rfind('.');
		size_t count = (dot == std::string::npos) ? path.size() - slash : dot - slash;
		char *ptr = new char[count - slash];
		memcpy_s(ptr, count - slash, path.substr(slash, count).c_str(), count - slash);
		return ptr;
	}

	int FlipBits(int &flags, int mask)
	{
		flags ^= mask;
		return flags;
	}

	int ClearBits(int &flags, int mask)
	{
		flags &= ~mask;
		return flags;
	}

	int SetBits(int &flags, int mask)
	{
		flags |= mask;
		return flags;
	}
}