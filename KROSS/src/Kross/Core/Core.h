#pragma once

#include "CoreConfig.h"

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

#ifndef _INFO_
	#define _INFO_ 0
#endif
#ifndef _WARN_
	#define _WARN_ 1
#endif
#ifndef _ERROR_
	#define _ERROR_ 2
#endif
#ifndef _FATAL_
	#define _FATAL_ 3
#endif

//#if defined(KROSS_PLATFORM_WINDOWS) 
//#else
//	#error Kross only supports Windows... for now.
//#endif

#ifdef KROSS_ENABLE_ASSERTS
	#define KROSS_ASSERT(x, ...) { if(!(x)) { KROSS_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	#define KROSS_CORE_ASSERT(x, ...) { if(!(x)) { KROSS_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
	#define KROSS_ASSERT(x, ...)
	#define KROSS_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)
#define KROSS_BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)

#include <memory>

namespace Kross {

	template<class T>
	using Ref = std::shared_ptr<T>;

	template<class T, class ... Args>
	constexpr Ref<T> makeRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

	template<class T>
	using Scope = std::unique_ptr<T>;

	template<class T, class ... Args>
	constexpr Scope<T> makeScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
}