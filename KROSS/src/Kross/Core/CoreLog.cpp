#include <Kross_pch.h>
#include "Kross/Core/CoreLog.h"

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ostream_sink.h"

namespace Kross {
	//KAPI spdlog::logger* Logger::s_FileLogger;
	KAPI spdlog::logger* Logger::s_CoreLogger;
	//KAPI spdlog::logger* Logger::s_ClientFileLogger;
	KAPI spdlog::logger* Logger::s_ClientLogger;

	void Logger::Init()
	{
		try
		{
			std::vector<spdlog::sink_ptr> sinks(3);
			const uint8_t core = 0;
			sinks[0] = (makeRef<spdlog::sinks::stdout_color_sink_mt>());

			sinks[0]->set_pattern("%^[%T] %n: %v%$");
			sinks[1] = (makeRef<spdlog::sinks::basic_file_sink_mt>("Kross.log", true));
			sinks[1]->set_pattern("[%T] [%l] %n: %v");
			{
				auto log = makeRef<spdlog::logger>("KROSS", sinks.begin(), sinks.end());
				s_CoreLogger = log.get();
				spdlog::register_logger(log);
				s_CoreLogger->set_level(spdlog::level::trace);
				s_CoreLogger->flush_on(spdlog::level::trace);
			}
			sinks[2] = makeRef<spdlog::sinks::basic_file_sink_mt>("Client.log", true);
			sinks[2]->set_pattern("[%T] [%l] %n: %v");

			{
				auto log = makeRef<spdlog::logger>("APP", sinks.begin(), sinks.end());
				s_ClientLogger = log.get();
				spdlog::register_logger(log);
				s_ClientLogger->set_level(spdlog::level::trace);
				s_ClientLogger->flush_on(spdlog::level::trace);
			}
			s_CoreLogger->critical("Welcome to Kross Engine");
			s_CoreLogger->info("Initialized Logger");
		} catch (const spdlog::spdlog_ex& ex)
		{
			std::cout << "Logger init failed: " << ex.what() << std::endl;
		}
	}
	/*
	template<typename... Args>
	static std::string log_to_str(const std::string& msg, const Args &... args)
	{
		std::ostringstream oss;
		auto oss_sink = std::make_shared<spdlog::sinks::ostream_sink_mt>(oss);
		spdlog::logger oss_logger("pattern_tester", oss_sink);
		oss_logger.set_level(spdlog::level::info);

		oss_logger.set_formatter(std::unique_ptr<spdlog::formatter>(new spdlog::pattern_formatter(args...)));

		oss_logger.info(msg);
		return oss.str();
	}
	template<typename ...Args>
	inline void Kross::Logger::Core(Target t, Severity s, const char* srcFunc, const char* fmt, Args && ...args)
	{
		std::string format;
		format += "[";
		format += srcFunc;
		format += "] | ";
		format += fmt;
		if (Target::Console) Console(true, s, format, FORWARD);
		if (Target::File) File(true, s, format, FORWARD);
		if (Target::MsgBox) WindowBox(true, s, format, FORWARD);
		if (t == Target::Console || t == Target::File || t == Target::MsgBox) return;

		s_CoreLogger->error("\t***\tInvalid Target for Logger\t***\n{0}", format);
		s_FileLogger->error("\t***\tInvalid Target for Logger\t***\n{0}", format);
	}
	template<typename...Args>
	inline void Kross::Logger::Client(Target t, Severity s, const char* srcFunc, const char* fmt, Args&&...args)
	{
		std::string format;
		format += "[";
		format += srcFunc;
		format += "] | ";
		format += fmt;
		if (Target::Console) Console(false, s, format, FORWARD);
		if (Target::File) File(false, s, format, FORWARD);
		if (Target::MsgBox) WindowBox(false, s, format, FORWARD);
		if (t == Target::Console || t == Target::File || t == Target::MsgBox) return;

		s_ClientLogger->error("\t***\tInvalid Target for Logger\t***\n{0}", format);
		s_ClientFileLogger->error("\t***\tInvalid Target for Logger\t***\n{0}", format);
	}
	template<typename ...Args>
	inline void Logger::Console(bool core, Severity s, const char* fmt, Args && ...args)
	{
		switch (s)
		{
			case Severity::trace:    (core ? s_CoreLogger : s_ClientLogger)->trace(fmt, FORWARD); break;
			case Severity::info:     (core ? s_CoreLogger : s_ClientLogger)->info(fmt, FORWARD); break;
			case Severity::warn:     (core ? s_CoreLogger : s_ClientLogger)->warn(fmt, FORWARD); break;
			case Severity::error:    (core ? s_CoreLogger : s_ClientLogger)->error(fmt, FORWARD); break;
			case Severity::critical: (core ? s_CoreLogger : s_ClientLogger)->critical(fmt, FORWARD); break;
			default: (core ? s_CoreLogger : s_ClientLogger)->error("\t***\tInvalid Severity for Logger\t***");
		}
	}
	template<typename ...Args>
	inline void Logger::File(bool core, Severity s, const char* fmt, Args && ...args)
	{
		switch (s)
		{
			case Severity::trace:    (core ? s_FileLogger : s_ClientFileLogger)->trace(fmt, FORWARD); break;
			case Severity::info:     (core ? s_FileLogger : s_ClientFileLogger)->info(fmt, FORWARD); break;
			case Severity::warn:     (core ? s_FileLogger : s_ClientFileLogger)->warn(fmt, FORWARD); break;
			case Severity::error:    (core ? s_FileLogger : s_ClientFileLogger)->error(fmt, FORWARD); break;
			case Severity::critical: (core ? s_FileLogger : s_ClientFileLogger)->critical(fmt, FORWARD); break;
			default: (core ? s_FileLogger : s_ClientFileLogger)->error("\t***\tInvalid Severity for Logger\t***");
		}
	}
	template<typename ...Args>
	inline void Kross::Logger::WindowBox(bool core, Severity s, const char* fmt, Args && ...args)
	{
#ifdef KROSS_PLATFORM_WINDOWS
		std::string log = log_to_str(std::string(" [MB] ") + fmt, FORWARD);

#if UNICODE
		std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
		std::wstring message = convert.from_bytes(log.substr(log.find_last_of("]|") + 1));
		std::wstring source = convert.from_bytes(log.substr(0, log.find_last_of("]|") + 1));
#define MB_MESSAGETITLE(mod) std::wstring(#mod+source).c_str()
#else
		std::string message = log.substr(log.find_last_of("]") + 1);
		std::string source = log.substr(0, log.find_last_of("]") + 1);
#define MB_MESSAGETITLE(mod, cod) (std::string(source+" ["#mod"] Code: ") + std::to_string(cod)).c_str()
#endif

		switch (s)
		{
			case Severity::info:     MessageBox(ACTIVEWINDOW, message.c_str(), MB_MESSAGETITLE("[INFO] "), MB_ICONINFORMATION); break;
			case Severity::warn:     MessageBox(ACTIVEWINDOW, message.c_str(), MB_MESSAGETITLE("[WARN] "), MB_ICONEXCLAMATION); break;
			case Severity::error:    MessageBox(ACTIVEWINDOW, message.c_str(), MB_MESSAGETITLE("[ERROR] "), MB_ICONERROR); MessageBeep(1000); break;
			case Severity::critical: MessageBox(ACTIVEWINDOW, message.c_str(), MB_MESSAGETITLE("[FATAL] "), MB_ICONERROR); exit(1); break;
		}
#endif
	}
//*/
}

