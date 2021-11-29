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
		static constexpr uint8_t Console = 1;
		static constexpr uint8_t File = 2;
		static constexpr uint8_t MsgBox = 3;
		static constexpr uint8_t trace = 1;
		static constexpr uint8_t info = 2;
		static constexpr uint8_t warn = 3;
		static constexpr uint8_t error = 4;
		static constexpr uint8_t critical = 5;
	
	private:
		template<typename...Args> static void _Console(bool core, uint8_t s, const std::string& fmt, const Args&...args)
		{
			switch (s)
			{
				case trace:    (core ? s_CoreLogger : s_ClientLogger)->trace(fmt, args...); break;
				case info:     (core ? s_CoreLogger : s_ClientLogger)->info(fmt, args...); break;
				case warn:     (core ? s_CoreLogger : s_ClientLogger)->warn(fmt, args...); break;
				case error:    (core ? s_CoreLogger : s_ClientLogger)->error(fmt, args...); break;
				case critical: (core ? s_CoreLogger : s_ClientLogger)->critical(fmt, args...); break;
				default: (core ? s_CoreLogger : s_ClientLogger)->error("\t***\tInvalid Severity for Logger\t***");
			}
		}
		template<typename...Args> static void _File(bool core, uint8_t s, const std::string& fmt, const Args&...args)
		{
			switch (s)
			{
				case trace:    (core ? s_FileLogger : s_ClientFileLogger)->trace(fmt, args...); break;
				case info:     (core ? s_FileLogger : s_ClientFileLogger)->info(fmt, args...); break;
				case warn:     (core ? s_FileLogger : s_ClientFileLogger)->warn(fmt, args...); break;
				case error:    (core ? s_FileLogger : s_ClientFileLogger)->error(fmt, args...); break;
				case critical: (core ? s_FileLogger : s_ClientFileLogger)->critical(fmt, args...); break;
				default: (core ? s_FileLogger : s_ClientFileLogger)->error("\t***\tInvalid Severity for Logger\t***");
			}
		}
		static void _WindowBox(bool core, uint8_t s, const std::string& log)
		{
#ifdef KROSS_PLATFORM_WINDOWS
			std::string message = log.substr(log.find_last_of("]") + 1);
			std::string source = log.substr(0, log.find_last_of("]") + 1);
			auto MB_MESSAGETITLE = [&source](const std::string& mod) { return (source + " [" + mod + "] Code: ").c_str(); };

			switch (s)
			{
				case info:     MessageBoxA((GetActiveWindow()), message.c_str(), MB_MESSAGETITLE("[INFO] "), 0x00000040L); break;
				case warn:     MessageBoxA((GetActiveWindow()), message.c_str(), MB_MESSAGETITLE("[WARN] "), 0x00000030L); break;
				case error:    MessageBoxA((GetActiveWindow()), message.c_str(), MB_MESSAGETITLE("[ERROR] "), 0x00000010L); MessageBeep(1000); break;
				case critical: MessageBoxA((GetActiveWindow()), message.c_str(), MB_MESSAGETITLE("[FATAL] "), 0x00000010L); exit(1); break;
			}
#endif
		}
	public:
		static void Init();
		template<typename...Args>
		static inline void Core(uint8_t t, uint8_t s, const char* srcFunc, const char* fmt, const Args&...args)
		{
			std::string format;
			format += "[";
			format += srcFunc;
			format += "] | ";
			format += fmt;
			//if (t==Console) _Console(true, s, format, args...);
			//if (t==File) _File(true, s, format, args...);
			//if (t==MsgBox) _WindowBox(true, s, fmt);
			//s_CoreLogger->log({}, spdlog::level::trace, "{0}", format.size());
			if (t == Console || t == File || t == MsgBox) return;

			//s_CoreLogger->error("\t***\tInvalid Target for Logger\t***\n{0}", format);
			//s_FileLogger->error("\t***\tInvalid Target for Logger\t***\n{0}", format);
		}

		template<typename...Args>
		static inline void Client(uint8_t t, uint8_t s, const char* srcFunc, const char* fmt, const Args&...args)
		{
			//std::string format;
			//format += "[";
			//format += srcFunc;
			//format += "] | ";
			//format += fmt;
			//if (Console != 0) _Console(false, s, format.c_str(), FORWARD);
			//if (File != 0) _File(false, s, format.c_str(), FORWARD);
			//if (MsgBox != 0) _WindowBox(false, s, fmt);
			//if (t == Console || t == File || t == MsgBox) return;

			//s_ClientLogger->error("\t***\tInvalid Target for Logger\t***\n{0}", format);
			//s_ClientFileLogger->error("\t***\tInvalid Target for Logger\t***\n{0}", format);
		}

		static spdlog::logger* GetCore() { return s_CoreLogger; }
	private:
		//static spdlog::logger* s_FileLogger;
		static spdlog::logger* s_CoreLogger;
		//static spdlog::logger* s_ClientFileLogger;
		static spdlog::logger* s_ClientLogger;
	};

#ifndef LOGGER_IMPL
#define LOGGER_IMPL
//Ref<spdlog::logger> Logger::s_FileLogger;
//Ref<spdlog::logger> Logger::s_CoreLogger;
//Ref<spdlog::logger> Logger::s_ClientFileLogger;
//Ref<spdlog::logger> Logger::s_ClientLogger;
#endif //LOGGER_IMPL
}

#if(KROSS_FILE_LOG == true)
//#define KROSS_CORE_FILE_TRACE(fmt, ...) ::Kross::Logger::Core(Kross::Logger::File, Kross::Logger::trace, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_CORE_FILE_INFO(fmt, ...)  ::Kross::Logger::Core(Kross::Logger::File, Kross::Logger::info, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_CORE_FILE_WARN(fmt, ...)  ::Kross::Logger::Core(Kross::Logger::File, Kross::Logger::warn, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_CORE_FILE_ERROR(fmt, ...) ::Kross::Logger::Core(Kross::Logger::File, Kross::Logger::error, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_CORE_FILE_FATAL(fmt, ...) ::Kross::Logger::Core(Kross::Logger::MsgBox, Kross::Logger::critical, __FUNCTION__, fmt, __VA_ARGS__)
#else
#define KROSS_CORE_FILE_TRACE(fmt, ...)
#define KROSS_CORE_FILE_INFO(fmt, ...)
#define KROSS_CORE_FILE_WARN(fmt, ...)
#define KROSS_CORE_FILE_ERROR(fmt, ...)
#define KROSS_CORE_FILE_FATAL(fmt, ...)
#endif //KROSS_FILE_LOG
#define KROSS_MSGBOX_TRACE(fmt, ...)
#define KROSS_MSGBOX_INFO(fmt, ...)	
#define KROSS_MSGBOX_WARN(fmt, ...)	
#define KROSS_MSGBOX_ERROR(fmt, ...)

//#define KROSS_MSGBOX_TRACE(fmt, ...)	::Kross::Logger::Core(Kross::Logger::MsgBox, Kross::Logger::trace, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_MSGBOX_INFO(fmt, ...)		::Kross::Logger::Core(Kross::Logger::MsgBox, Kross::Logger::info, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_MSGBOX_WARN(fmt, ...)		::Kross::Logger::Core(Kross::Logger::MsgBox, Kross::Logger::warn, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_MSGBOX_ERROR(fmt, ...)	::Kross::Logger::Core(Kross::Logger::MsgBox, Kross::Logger::error, __FUNCTION__, fmt, __VA_ARGS__)

#define KROSS_CORE_TRACE(...)	::Kross::Logger::GetCore()->trace(__VA_ARGS__)
#define KROSS_CORE_INFO(...)	::Kross::Logger::GetCore()->info(__VA_ARGS__)
#define KROSS_CORE_WARN(...)	::Kross::Logger::GetCore()->warn(__VA_ARGS__)
#define KROSS_CORE_ERROR(...)	::Kross::Logger::GetCore()->error(__VA_ARGS__)
#define KROSS_CORE_FATAL(...)	::Kross::Logger::GetCore()->critical(__VA_ARGS__)

//#define KROSS_CORE_TRACE(fmt, ...)	::Kross::Logger::GetCore()->log(spdlog::level::trace,	(" ["+std::string(__FUNCTION__)+"] "+fmt).c_str(), __VA_ARGS__)
//#define KROSS_CORE_INFO(fmt, ...)	::Kross::Logger::GetCore()->log(spdlog::level::info,	(" ["+std::string(__FUNCTION__)+"] "+fmt).c_str(), __VA_ARGS__)
//#define KROSS_CORE_WARN(fmt, ...)	::Kross::Logger::GetCore()->log(spdlog::level::warn,	(" ["+std::string(__FUNCTION__)+"] "+fmt).c_str(), __VA_ARGS__)
//#define KROSS_CORE_ERROR(fmt, ...)	::Kross::Logger::GetCore()->log(spdlog::level::err,		(" ["+std::string(__FUNCTION__)+"] "+fmt).c_str(), __VA_ARGS__)
//#define KROSS_CORE_FATAL(fmt, ...)	::Kross::Logger::GetCore()->log(spdlog::level::critical,(" ["+std::string(__FUNCTION__)+"] "+fmt).c_str(), __VA_ARGS__)

//#define KROSS_CORE_TRACE(...)	::Kross::Logger::GetCore()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::trace, __VA_ARGS__)
//#define KROSS_CORE_INFO(...)	::Kross::Logger::GetCore()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::info, __VA_ARGS__)
//#define KROSS_CORE_WARN(...)	::Kross::Logger::GetCore()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::warn, __VA_ARGS__)
//#define KROSS_CORE_ERROR(...)	::Kross::Logger::GetCore()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::error, __VA_ARGS__)
//#define KROSS_CORE_FATAL(...)	::Kross::Logger::GetCore()->log(spdlog::source_loc{__FILE__, __LINE__, SPDLOG_FUNCTION}, spdlog::level::critical, __VA_ARGS__)

#define KROSS_CORE_TRACE_ONCE(...)
#define KROSS_CORE_INFO_ONCE(...) 
#define KROSS_CORE_WARN_ONCE(...) 
#define KROSS_CORE_ERROR_ONCE(...)

//#define KROSS_CORE_TRACE(fmt, ...)	::Kross::Logger::Core(Kross::Logger::Console, Kross::Logger::trace, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_CORE_INFO(fmt, ...)	::Kross::Logger::Core(Kross::Logger::Console, Kross::Logger::info, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_CORE_WARN(fmt, ...)	::Kross::Logger::Core(Kross::Logger::Console, Kross::Logger::warn, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_CORE_ERROR(fmt, ...)	::Kross::Logger::Core(Kross::Logger::Console, Kross::Logger::error, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_CORE_FATAL(fmt, ...)	::Kross::Logger::Core(Kross::Logger::MsgBox, Kross::Logger::critical, __FUNCTION__, fmt, __VA_ARGS__)
//#define KROSS_CORE_TRACE_ONCE(fmt, ...) { static bool called = false; if(!called) {	::Kross::Logger::Core(Kross::Logger::Console, Kross::Logger::trace, __FUNCTION__, fmt, __VA_ARGS__); called = true;}}
//#define KROSS_CORE_INFO_ONCE(fmt, ...)  { static bool called = false; if(!called) {	::Kross::Logger::Core(Kross::Logger::Console, Kross::Logger::info, __FUNCTION__, fmt, __VA_ARGS__); called = true;}}
//#define KROSS_CORE_WARN_ONCE(fmt, ...)  { static bool called = false; if(!called) {	::Kross::Logger::Core(Kross::Logger::Console, Kross::Logger::warn, __FUNCTION__, fmt, __VA_ARGS__); called = true;}}
//#define KROSS_CORE_ERROR_ONCE(fmt, ...) { static bool called = false; if(!called) {	::Kross::Logger::Core(Kross::Logger::Console, Kross::Logger::error, __FUNCTION__, fmt, __VA_ARGS__); called = true;}}
