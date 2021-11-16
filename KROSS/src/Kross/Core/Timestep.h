#pragma once
#include <chrono>

namespace Kross {
	class Timestep
	{
	public:
		Timestep()
			: m_scTime(std::chrono::high_resolution_clock::now().time_since_epoch()),
			m_lfTime(static_cast<double>(std::chrono::time_point_cast<std::chrono::microseconds>(m_scTime).time_since_epoch().count()))
		{}

		operator double() const { return m_lfTime; }
		static inline const double now() { return static_cast<double>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now().time_since_epoch()).count()); }
		const double GetLap() { static double last_pass = m_lfTime; m_lfTime = now(); auto r = m_lfTime - last_pass; last_pass = m_lfTime; return r * 0.0001f; }
		inline const double GetSeconds() const { return m_lfTime * 0.0001f; }
		inline const double GetMilliseconds() const { return m_lfTime; }
	private:
		std::chrono::time_point<std::chrono::steady_clock> m_scTime;
		double m_lfTime;
	};

	class Timer
	{
	public:
		struct step {
			const char* cName;
			float fTime;
		};
	public:
		Timer(const char* name, bool rtp_ = false);
		~Timer();

		void Stop();

		static void read(void func(step&));
		static void read_c(void func(step&));
		static void RTP_clear();
		static float RTP_get(const char* name);
	private:
		static void RTP_add(const step& step);
	public:
		const char* cName;
		float fTime;
	private:
		static std::vector<step> RTP; // Real Time Profiler
		std::chrono::time_point<std::chrono::steady_clock> startPoint;
		bool bStoped;
		bool rtp;
		static bool RTP_cleared;
	};

	class DeltaTime
	{
		std::chrono::time_point<std::chrono::steady_clock> m_fLastIterationTime;
	public:
		DeltaTime();
		float lap();
	};
}
