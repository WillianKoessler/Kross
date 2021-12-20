#include <Kross_pch.h>
#include "Components.h"

namespace Kross {

	static std::string getAvail(std::string &&tag, std::vector<std::string> &pool)
	{
		std::string now = tag;
		uint32_t count = 0;
		for (auto &stored : pool) {
			if (now == stored) count++;
			if (count > 0) now = tag + "(" + std::to_string(count) + ")";
		}
		auto place = std::lower_bound(pool.begin(), pool.end(), now);
		pool.emplace(place, now);
		return now;
	}
	void TagComponent::Set(const char *newTag)
	{
		auto find = [&](std::string &tag) { return std::lower_bound(pool->begin(), pool->end(), tag); };
		auto add = [&](std::string &tag) { pool->emplace(find(tag), tag); };
		auto remove = [&](std::string &tag) { auto i = find(tag); if (i != pool->end() && tag == (*i)) pool->erase(i); };
		std::string myTag = tag;
		if (newTag != nullptr)
		{
			std::string target = newTag;
			if (myTag == target) return;

			if (target.empty()) {
				remove(myTag);
				target = "(null)";
			} else {
				auto stored = std::lower_bound(pool->begin(), pool->end(), target);
				if (stored != pool->end() && myTag == (*stored)) {
					if (target.length() > limit - 10) target.resize(limit - 10);
					(*stored) = target;
				} else {
					remove(myTag);
					if (pool) target = getAvail(std::move(target), *pool);
					else {
						KROSS_ERROR("Invalid name pool");
						return;
					}
				}
			}
			memset(tag, 0, limit);
			strcpy_s(tag, target.c_str());
		} else {
			remove(myTag);
			memset(tag, 0, limit);
		}
	}

	TagComponent::TagComponent(const TagComponent &other)
		: pool(other.pool)
	{
		Set(other.tag);
	}

	TagComponent::TagComponent(const char *newTag, std::vector<std::string> *newPool)
		: pool(newPool)
	{
		memset(tag, 0, limit);
		if (pool) {
			std::string available = getAvail(newTag, *pool);
			strcpy_s(tag, available.c_str());
		} else KROSS_ERROR("Invalid name pool");
	}
}
