#include <Kross_pch.h>
#include "Components.h"

namespace Kross {
	static std::vector<std::string> usedNames;
	static int find(const std::string &tag)
	{
		for (int i = 0; i < (int)usedNames.size(); i++)
			if (usedNames[i] == tag) return i;
		return -1;
	}
	static void add(const std::string &tag)
	{
		usedNames.emplace_back(tag);
	}
	static void remove(const char *tag)
	{
		int i = find(tag);
		//KROSS_TRACE("Remove '{0}'", *(usedNames.begin() + i));
		if (i > -1) usedNames.erase(usedNames.begin() + i);
	}
	static bool has(const char *tag)
	{
		return find(tag) >= 0;
	}
	static std::string getAvail(const std::string &tag)
	{
		std::string now = tag;
		uint32_t count = 0;
		for (auto &stored : usedNames) {
			if (now == stored) count++;
			if (count > 0) now = tag + "(" + std::to_string(count) + ")";
		}
		add(now);
		return now;
	}
	void TagComponent::Set(const char *newTag)
	{
		std::string target = newTag;
		if (target.empty()) target = "(null)";

		if (has(tag)) remove(tag);
		memset(tag, 0, limit);
		memcpy(tag, getAvail(target).c_str(), limit);
	}
	TagComponent::~TagComponent()
	{
		remove(tag);
	}
}