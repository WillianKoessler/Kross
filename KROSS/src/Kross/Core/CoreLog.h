#pragma once

#include "Kross/Core/Core.h"

#include "spdlog/include/spdlog/spdlog.h"
#include "spdlog/include/spdlog/fmt/ostr.h"
#include "spdlog/include/spdlog/sinks/stdout_color_sinks.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ostream_sink.h"

#include <iostream>

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
		static void MsgBox(uint8_t s, const char* srcFunc, const char* fmt, const Args&...args)
		{
			std::string format;
			format += "[";
			format += srcFunc;
			format += "] | ";
			format += fmt;

#ifdef KROSS_PLATFORM_WINDOWS
			auto MB_MESSAGETITLE = [&srcFunc](const char* mod) { return std::string(srcFunc) + " [" + mod + "] Code: "; };
			switch (s)
			{
				case info:     s_CoreLogger->info(format, args...); MessageBoxA(ACTIVEWINDOW, format.c_str(), MB_MESSAGETITLE("[INFO] ").c_str(), MB_ICONASTERISK); break;
				case warn:     s_CoreLogger->warn(format, args...); MessageBoxA(ACTIVEWINDOW, format.c_str(), MB_MESSAGETITLE("[WARN] ").c_str(), MB_ICONEXCLAMATION); break;
				case error:    s_CoreLogger->error(format, args...); MessageBoxA(ACTIVEWINDOW, format.c_str(), MB_MESSAGETITLE("[ERROR] ").c_str(), MB_ICONHAND); MessageBeep(1000); break;
				case critical: s_CoreLogger->critical(format, args...); MessageBoxA(ACTIVEWINDOW, format.c_str(), MB_MESSAGETITLE("[FATAL] ").c_str(), MB_ICONHAND); exit(1); break;
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

#define KROSS_MSGBOX_TRACE(fmt, ...)	::Kross::Logger::MsgBox(Kross::Logger::trace, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_MSGBOX_INFO(fmt, ...)		::Kross::Logger::MsgBox(Kross::Logger::info, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_MSGBOX_WARN(fmt, ...)		::Kross::Logger::MsgBox(Kross::Logger::warn, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_MSGBOX_ERROR(fmt, ...)	::Kross::Logger::MsgBox(Kross::Logger::error, __FUNCTION__, fmt, __VA_ARGS__)

#define KROSS_CORE_TRACE(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::trace, __VA_ARGS__)
#define KROSS_CORE_INFO(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::info, __VA_ARGS__)
#define KROSS_CORE_WARN(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::warn, __VA_ARGS__)
#define KROSS_CORE_ERROR(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::err, __VA_ARGS__)
#define KROSS_CORE_FATAL(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::critical, __VA_ARGS__)

#define KROSS_CORE_TRACE_ONCE(...) { static bool called = false; if(!called) { KROSS_CORE_TRACE(__VA_ARGS__); called = true; } }
#define KROSS_CORE_INFO_ONCE(...)  { static bool called = false; if(!called) { KROSS_CORE_INFO(__VA_ARGS__); called = true; } }
#define KROSS_CORE_WARN_ONCE(...)  { static bool called = false; if(!called) { KROSS_CORE_WARN(__VA_ARGS__); called = true; } }
#define KROSS_CORE_ERROR_ONCE(...) { static bool called = false; if(!called) { KROSS_CORE_ERROR(__VA_ARGS__); called = true; } }

#define KROSS_TRACE(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Client(), spdlog::level::trace, __VA_ARGS__)
#define KROSS_INFO(...)		SPDLOG_LOGGER_CALL(Kross::Logger::Client(), spdlog::level::info, __VA_ARGS__)
#define KROSS_WARN(...)		SPDLOG_LOGGER_CALL(Kross::Logger::Client(), spdlog::level::warn, __VA_ARGS__)
#define KROSS_ERROR(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Client(), spdlog::level::err, __VA_ARGS__)
#define KROSS_FATAL(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Client(), spdlog::level::critical, __VA_ARGS__)

#define KROSS_TRACE_ONCE(...) { static bool called = false; if(!called) { KROSS_TRACE(__VA_ARGS__); called = true; } }
#define KROSS_INFO_ONCE(...)  { static bool called = false; if(!called) { KROSS_INFO(__VA_ARGS__); called = true; } }
#define KROSS_WARN_ONCE(...)  { static bool called = false; if(!called) { KROSS_WARN(__VA_ARGS__); called = true; } }
#define KROSS_ERROR_ONCE(...) { static bool called = false; if(!called) { KROSS_ERROR(__VA_ARGS__); called = true; } }
