#include <Kross_pch.h>
#include "Components.h"

namespace Kross {
	static std::vector<std::string> pool;
	static auto find(std::string &tag)
	{
		auto i = std::lower_bound(pool.begin(), pool.end(), tag);
		return i;
	}
	static void add(std::string &tag)
	{
		auto place = find(tag);
		pool.emplace(place, tag);
	}
	static void remove(std::string &tag)
	{
		auto i = find(tag);
		if (i != pool.end() && tag == (*i))
			pool.erase(i);
	}
	static bool has(std::string &tag)
	{
		return find(tag) != pool.end();
	}
	static std::string getAvail(std::string &&tag)
	{
		std::string now = tag;
		uint32_t count = 0;
		for (auto &stored : pool) {
			if (now == stored) count++;
			if (count > 0) now = tag + "(" + std::to_string(count) + ")";
		}
		add(now);
		return now;
	}
	void TagComponent::Set(const char *newTag)
	{
		std::string myTag = tag;
		if (newTag != nullptr)
		{
			std::string target = newTag;
			if (myTag == target) return;

			if (target.empty()) {
				remove(myTag);
				target = "(null)";
			} else {
				auto stored = find(target);
				if (stored != pool.end() && myTag == (*stored)) {
					if (target.length() > limit - 10) target.resize(limit - 10);
					(*stored) = target;
				} else {
					remove(myTag);
					target = getAvail(std::move(target));
				}
			}
			memset(tag, 0, limit);
			strcpy_s(tag, target.c_str());
		} else {
			remove(myTag);
			memset(tag, 0, limit);
		}
	}
	void TagComponent::ShowAll()
	{
		int i = 0;
		for (auto &str : pool)
			KROSS_TRACE(" pool[{0}] = {1}", i++, str);
	}

	TagComponent::TagComponent(const char *newTag)
	{
		memset(tag, 0, limit);
		std::string available = getAvail(newTag);
		strcpy_s(tag, available.c_str());
	}
}
