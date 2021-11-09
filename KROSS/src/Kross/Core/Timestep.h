#pragma once
#include <chrono>

namespace Kross {
	class Timestep
	{
	public:
		Timestep(float time = 0.0f)
			: m_fTime(time)
		{}

		operator float() const { return m_fTime; }

		inline const float GetSeconds() const { return m_fTime; }
		inline const float GetMilliseconds() const { return m_fTime * 1000.0f; }
	private:
		float m_fTime;
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
		static std::vector<step> RTP;
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
