#include <Kross_pch.h>
#include "Components.h"

namespace Kross {
	static std::vector<std::string> usedNames;
	static int find(const char *tag)
	{
		std::string t(tag);
		for (int i = 0; i < (int)usedNames.size(); i++) 
			if (usedNames[i] == t) return i;
		return -1;
	}
	static void add(const char *tag)
	{
		usedNames.emplace_back(tag);
	}
	static void remove(const char *tag)
	{
		int i = find(tag);
		if (i > -1) usedNames.erase(usedNames.begin() + i);
	}
	static bool has(const char *tag)
	{
		return find(tag) > 0;
	}
	static std::string getAvail(const char *tag)
	{
		std::string now(tag);
		uint32_t count = 0;
		for (auto &stored : usedNames) {
			if (now == stored) count++;
			if (count > 0) {
				now = tag + ("(" + std::to_string(count) + ")");
				if (now == stored) count++;
			}
		}
		return now;
	}
	void TagComponent::Set(const char *newTag)
	{
		const char *target;
		if (std::string(newTag).empty()) target = "null";
		else target = newTag;

		if (has(tag))
			remove(tag);
		std::string avail = getAvail(target);
		memset(tag, 0, limit);
		memcpy(tag, avail.c_str(), limit);
		add(tag);
	}
	TagComponent::~TagComponent()
	{
		remove(tag);
	}
}