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
		static float Float()
		{
			return (float)s_distribution(s_device) / (float)std::numeric_limits<uint32_t>::max();
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
