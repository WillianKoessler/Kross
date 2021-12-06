#pragma once

#include <random>
#include "Kross/Core/Core.h"

namespace Kross {
	class KAPI Random
	{
	public:
		static void Init()
		{
			s_device.seed(std::_Random_device());
		}
		template<typename Number>
		static Number rand()
		{
			return ((Number)s_distribution(s_device) / (Number)std::numeric_limits<Number>::max());
		}
		static uint64_t hash(uint64_t state)
		{
			state ^= 2747636419u;
			state *= 2654435769u;
			state ^= state >> 16;
			state *= 2654435769u;
			state ^= state >> 16;
			state *= 2654435769u;
			return state;
		}
	private:
		static std::mt19937 s_device;
		static std::uniform_int_distribution<std::mt19937::result_type> s_distribution;
	};
}

#if !(RANDOM_INSTANCE)
#define RANDOM_INSTANCE 1
std::mt19937 Kross::Random::s_device;
std::uniform_int_distribution<std::mt19937::result_type> Kross::Random::s_distribution;
#endif
