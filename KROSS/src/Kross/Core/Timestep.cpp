#include <Kross_pch.h>
#include "Timestep.h"

namespace Kross {
	std::vector<Timer::step> Timer::RTP = std::vector<step>();
	bool Timer::RTP_cleared = false;

	Timer::Timer(const char* name, bool rtp_)
		: cName(name),
		fTime(0.0f),
		startPoint(std::chrono::high_resolution_clock::now()),
		bStoped(false),
		rtp(rtp_)
	{
		RTP_cleared = false;
	}

	Timer::~Timer()
	{
		if (!bStoped)
			Stop();
	}

	void Timer::Stop()
	{
		using namespace std::chrono;
		time_point<steady_clock> endPoint = high_resolution_clock::now();
		long long start = time_point_cast<microseconds>(startPoint).time_since_epoch().count();
		long long end = time_point_cast<microseconds>(endPoint).time_since_epoch().count();
		fTime = float(end - start) * 0.001f;
		bStoped = true;

		//KROSS_CORE_TRACE("[Timer: {1}] Duration: {0}ms", fTime, cName);
		if (rtp)
			RTP_add({ cName, fTime });
		else
			Profiler::Get().Write({ cName, start, end });
	}

	void Timer::read(void func(step&))
	{
		for (auto& i = Kross::Timer::RTP.rbegin(); i != Kross::Timer::RTP.rend(); ++i)
		{
			auto& t = *i;
			func(t);
		}
		if (!RTP_cleared)
		{
			KROSS_CORE_WARN("[Kross::Timer] Reading from a not cleared Profiling Buffer.\nTry use Kross::Timer::read_c instead.");
		}
	}

	void Timer::read_c(void func(step&))
	{
		for (auto& i = Kross::Timer::RTP.rbegin(); i != Kross::Timer::RTP.rend(); ++i)
		{
			auto& t = *i;
			func(t);
		}
		RTP_clear();
	}

	void Timer::RTP_clear()
	{
		RTP.clear();
		RTP_cleared = true;
	}

	void Timer::RTP_add(const step& step)
	{
		//KROSS_CORE_TRACE("RunTime Profiler: add {0}", step.cName);
		RTP.emplace_back(step);
	}

	float Timer::RTP_get(const char* name)
	{
		for (auto& t : RTP)
			if (name == t.cName)
				return t.fTime;
		KROSS_CORE_WARN("Runtime Profiler: element '{0}' not found.", name);
		return -1;
	}

}