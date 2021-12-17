#pragma once

#define CONSOLE_HANDLE
#define ACTIVEWINDOW

#ifdef _WIN32
	#ifdef _WIN64
		#ifndef KROSS_PLATFORM_WINDOWS
			#define KROSS_PLATFORM_WINDOWS 1
		#endif
	#else
		#error "x86 Builds are not supported!"
	#endif

	#undef CONSOLE_HANDLE
	#define CONSOLE_HANDLE (GetStdHandle(STD_OUTPUT_HANDLE))

	#undef ACTIVEWINDOW
	#define ACTIVEWINDOW (GetActiveWindow())

	#if defined(KAPI)
		#undef KAPI
	#endif
	#if KROSS_DLL
		#if KROSS_BUILD_DLL
			#define KAPI __declspec(dllexport)
		#else
			#define KAPI __declspec(dllimport)
		#endif
	#else
		#define KAPI
	#endif
#elif defined(__APPLE__) || defined (__MACH__)
	#include <TargetConditionals.h>
	#if TARGET_IPHONE_SIMULATOR == 1
		#error "IOS simulator is not supported!"
	#elif TARGET_OS_IPHONE == 1
		#ifndef KROSS_PLATFORM_IOS
			#define KROSS_PLATFORM_IOS 1
		#endif
		#error "IOS is not supported!"
	#elif TARGET_OS_MAC == 1
		#ifndef KROSS_PLATFORM_MACOS
			#define KROSS_PLATFORM_MACOS 1
		#endif
		#error "MacOS is not supported!"
	#else
		#error "Unknown Apple platform"
	#endif
#elif defined(__ANDROID__)
	#ifndef KROSS_PLATFORM_ANDROID
		#define KROSS_PLATFORM_ANDROID 1
	#endif
	#error "Android is not supported"
#elif defined(__linux__)
	#ifndef KROSS_PLATFORM_LINUX
		#define KROSS_PLATFORM_LINUX 1
	#endif
	#error "Linux is not supported"
#else
	#error "Unknown Platform."
#endif

#if KROSS_PROFILING
	#if KROSS_RUNTIME_PROFILING
		#define KROSS_RTP_FUNC()						::Kross::Timer timer(__FUNCSIG__, true)
		#define KROSS_RTP_SCOPE(name)					::Kross::Timer timer##name(#name, true)
	#else
		#define KROSS_RTP_FUNC()
		#define KROSS_RTP_SCOPE(name)
	#endif
	#if KROSS_FILE_PROFILING
		#define KROSS_PROFILE_OVERRIDE(name, filepath)	::Kross::Profiler::Get().OverrideSession(name, filepath)
		#define KROSS_PROFILE_BEGIN(name, filepath)		::Kross::Profiler::Get().BeginSession(name, filepath)
		#define KROSS_PROFILE_END						::Kross::Profiler::Get().EndSession()

		#define KROSS_PROFILE_SCOPE						::Kross::Timer timer
		#define KROSS_PROFILE_FUNC()					::Kross::Timer timer(__FUNCSIG__, false)
	#else
		#define KROSS_PROFILE_OVERRIDE(name, filepath)
		#define KROSS_PROFILE_BEGIN(name, filepath)
		#define KROSS_PROFILE_END
		#define KROSS_PROFILE_SCOPE
		#define KROSS_PROFILE_FUNC()
	#endif
#else
	#define KROSS_PROFILE_OVERRIDE(name, filepath)
	#define KROSS_PROFILE_BEGIN(name, filepath)
	#define KROSS_PROFILE_END
	#define KROSS_PROFILE_SCOPE
	#define KROSS_PROFILE_FUNC()
	#define KROSS_RTP_FUNC()
	#define KROSS_RTP_SCOPE(name)
#endif

#ifdef KROSS_DEBUG
#if defined(KROSS_PLATFORM_WINDOWS)
#define KROSS_DEBUGBREAK() __debugbreak()
#elif defined(KROSS_PLATFORM_LINUX)
#include <signal.h>
#define KROSS_DEBUGBREAK() raise(SIGTRAP)
#else
#error "Platform doesn't support debugbreak yet!"
#endif
#define KROSS_ENABLE_ASSERTS
#elif KROSS_FATAL_BREAK
#define KROSS_DEBUGBREAK() assert(false);
#else
#define KROSS_DEBUGBREAK()
#endif

#define KROSS_EXPAND_MACRO(x) x
#define KROSS_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)
#define KROSS_BIND_EVENT_FN(func) [this](auto&&...args)->decltype(auto){ return this->func(std::forward<decltype(args)>(args)...); }

#include <memory>

namespace Kross {

	template<class T> using Ref = std::shared_ptr<T>;
	template<class T, class ... Args>
	constexpr Ref<T> makeRef(Args&& ... args) {
		printf("\nRef Created: %s\n", typeid(T).name());
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<class T> using Scope = std::unique_ptr<T>;
	template<class T, class ... Args>
	constexpr Scope<T> makeScope(Args&& ... args) { return std::make_unique<T>(std::forward<Args>(args)...); }
}

#include "spdlog/include/spdlog/spdlog.h"
#include "Kross/Core/CoreLog.h"

#define KROSS_MSGBOX_TRACE(fmt, ...)	::Kross::Logger::MsgBox(Kross::Logger::trace, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_MSGBOX_INFO(fmt, ...)		::Kross::Logger::MsgBox(Kross::Logger::info, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_MSGBOX_WARN(fmt, ...)		::Kross::Logger::MsgBox(Kross::Logger::warn, __FUNCTION__, fmt, __VA_ARGS__)
#define KROSS_MSGBOX_ERROR(fmt, ...)	::Kross::Logger::MsgBox(Kross::Logger::error, __FUNCTION__, fmt, __VA_ARGS__)

#if KROSS_BUILD
#define KROSS_TRACE(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::trace, __VA_ARGS__)
#define KROSS_INFO(...)		SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::info, __VA_ARGS__)
#define KROSS_WARN(...)		SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::warn, __VA_ARGS__)
#define KROSS_ERROR(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::err, __VA_ARGS__)
#define KROSS_FATAL(...)	{SPDLOG_LOGGER_CALL(Kross::Logger::Core(), spdlog::level::critical, __VA_ARGS__); ::exit(1);}
#else
#define KROSS_TRACE(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Client(), spdlog::level::trace, __VA_ARGS__)
#define KROSS_INFO(...)		SPDLOG_LOGGER_CALL(Kross::Logger::Client(), spdlog::level::info, __VA_ARGS__)
#define KROSS_WARN(...)		SPDLOG_LOGGER_CALL(Kross::Logger::Client(), spdlog::level::warn, __VA_ARGS__)
#define KROSS_ERROR(...)	SPDLOG_LOGGER_CALL(Kross::Logger::Client(), spdlog::level::err, __VA_ARGS__)
#define KROSS_FATAL(...)	{SPDLOG_LOGGER_CALL(Kross::Logger::Client(), spdlog::level::critical, __VA_ARGS__); ::exit(1);}
#endif

#define KROSS_TRACE_ONCE(...) { static bool called = false; if(!called) { KROSS_TRACE(__VA_ARGS__); called = true; } }
#define KROSS_INFO_ONCE(...)  { static bool called = false; if(!called) { KROSS_INFO(__VA_ARGS__); called = true; } }
#define KROSS_WARN_ONCE(...)  { static bool called = false; if(!called) { KROSS_WARN(__VA_ARGS__); called = true; } }
#define KROSS_ERROR_ONCE(...) { static bool called = false; if(!called) { KROSS_ERROR(__VA_ARGS__); called = true; } }




#include <filesystem>

#ifdef KROSS_ENABLE_ASSERTS

// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define KROSS_INTERNAL_ASSERT_IMPL_FATAL(check, msg, ...) { if(!(check)) { KROSS_ERROR(msg, __VA_ARGS__); KROSS_DEBUGBREAK(); } }
#define KROSS_INTERNAL_ASSERT_WITH_MSG_FATAL(check, ...) KROSS_INTERNAL_ASSERT_IMPL_FATAL(check, "Assertion failed: {0}", __VA_ARGS__)
#define KROSS_INTERNAL_ASSERT_NO_MSG_FATAL(check) KROSS_INTERNAL_ASSERT_IMPL_FATAL(check, "Assertion '{0}' failed at {1}:{2}", KROSS_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define KROSS_INTERNAL_ASSERT_GET_MACRO_NAME_FATAL(arg1, arg2, macro, ...) macro
#define KROSS_INTERNAL_ASSERT_GET_MACRO_FATAL(...) KROSS_EXPAND_MACRO( KROSS_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, KROSS_INTERNAL_ASSERT_WITH_MSG_FATAL, KROSS_INTERNAL_ASSERT_NO_MSG_FATAL) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define KROSS_HARD_ASSERT(...) KROSS_EXPAND_MACRO( KROSS_INTERNAL_ASSERT_GET_MACRO_FATAL(__VA_ARGS__)(__VA_ARGS__) )


// Alteratively we could use the same "default" message for both "WITH_MSG" and "NO_MSG" and
// provide support for custom formatting by concatenating the formatting string instead of having the format inside the default message
#define KROSS_INTERNAL_ASSERT_IMPL(check, msg, ...) { if(!(check)) { KROSS_ERROR(msg, __VA_ARGS__); KROSS_DEBUGBREAK(); } }
#define KROSS_INTERNAL_ASSERT_WITH_MSG(check, ...) KROSS_INTERNAL_ASSERT_IMPL(check, "Assertion failed: {0}", __VA_ARGS__)
#define KROSS_INTERNAL_ASSERT_NO_MSG(check) KROSS_INTERNAL_ASSERT_IMPL(check, "Assertion '{0}' failed at {1}:{2}", KROSS_STRINGIFY_MACRO(check), std::filesystem::path(__FILE__).filename().string(), __LINE__)

#define KROSS_INTERNAL_ASSERT_GET_MACRO_NAME(arg1, arg2, macro, ...) macro
#define KROSS_INTERNAL_ASSERT_GET_MACRO(...) KROSS_EXPAND_MACRO( KROSS_INTERNAL_ASSERT_GET_MACRO_NAME(__VA_ARGS__, KROSS_INTERNAL_ASSERT_WITH_MSG, KROSS_INTERNAL_ASSERT_NO_MSG) )

// Currently accepts at least the condition and one additional parameter (the message) being optional
#define KROSS_SOFT_ASSERT(...) KROSS_EXPAND_MACRO( KROSS_INTERNAL_ASSERT_GET_MACRO(__VA_ARGS__)(__VA_ARGS__) )

#if KROSS_DEBUG
#define KROSS_ASSERT(...) KROSS_SOFT_ASSERT(__VA_ARGS__)
#else
#define KROSS_ASSERT(...) KROSS_HARD_ASSERT(__VA_ARGS__)
#endif

#else
#define KROSS_ASSERT(...)
#endif

namespace Kross {
	template<typename genType>
	genType phi()
	{
		KROSS_ASSERT(std::numeric_limits<genType>::is_iec559, "'phi' only accepts floating-point inputs");
		return static_cast<genType>(1.61903398874989484920721002966692491);
	}

	template<typename...error_message_args>
	bool Validate(bool *wasValid, bool condition, const char *fmt, error_message_args&&...args)
	{
		if (!wasValid) {
			KROSS_ERROR("Validation failed. (bool *wasValid == nullptr)");
			return false;
		}
		if (!condition) {
			if (*wasValid) {
				KROSS_WARN(fmt, std::forward<error_message_args>(args)...);
				*wasValid = false;
			}
			return false;
		}
		*wasValid = true;
		return true;
	}
}

