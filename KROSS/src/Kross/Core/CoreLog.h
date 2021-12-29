#pragma once

#include "Kross/Core/Core.h"

namespace Kross {
	class KAPI Logger
	{
	public:
		static constexpr uint8_t trace = 1;
		static constexpr uint8_t info = 2;
		static constexpr uint8_t warn = 3;
		static constexpr uint8_t error = 4;
		static constexpr uint8_t critical = 5;

	public:

		template<typename...Args>
		static void MsgBox(uint8_t s, const char *srcFunc, const char *fmt, const Args&...args)
		{
			std::string format;
			format += "[";
			format += srcFunc;
			format += "] | ";
			format += fmt;

#ifdef KROSS_PLATFORM_WINDOWS
			auto MB_MESSAGETITLE = [&srcFunc](const char *mod) { return std::string(srcFunc) + " [" + mod + "] Code: "; };
			switch (s)
			{
				case info:     s_CoreLogger->info(format, args...); MessageBoxA(ACTIVEWINDOW, format.c_str(), MB_MESSAGETITLE("INFO").c_str(), MB_ICONASTERISK); break;
				case warn:     s_CoreLogger->warn(format, args...); MessageBoxA(ACTIVEWINDOW, format.c_str(), MB_MESSAGETITLE("WARN").c_str(), MB_ICONEXCLAMATION); break;
				case error:    s_CoreLogger->error(format, args...); MessageBoxA(ACTIVEWINDOW, format.c_str(), MB_MESSAGETITLE("ERROR").c_str(), MB_ICONHAND); MessageBeep(1000); break;
				case critical: s_CoreLogger->critical(format, args...); MessageBoxA(ACTIVEWINDOW, format.c_str(), MB_MESSAGETITLE("FATAL").c_str(), MB_ICONHAND); exit(1); break;
			}
#endif
		}

		static void Init();
		static Ref<spdlog::logger> Core() { return s_CoreLogger; }
		static Ref<spdlog::logger> Client() { return s_CoreLogger; }
	private:
		static Ref<spdlog::logger> s_CoreLogger;
		static Ref<spdlog::logger> s_ClientLogger;
	};
}
