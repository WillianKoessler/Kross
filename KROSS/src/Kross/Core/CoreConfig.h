#pragma once


#ifndef CORE_CONFIG
#define CORE_CONFIG


/*
	Configuration File for Core.h
*/

#ifdef GraphicsAPI
#undef GraphicsAPI
#endif
#ifdef KROSS_DEBUG
#undef KROSS_DEBUG
#endif
#ifdef KROSS_GL_DEBUG
#undef KROSS_GL_DEBUG
#endif
#ifdef KROSS_GLCALL_PROFILE
#undef KROSS_GLCALL_PROFILE
#endif
#ifdef KROSS_RENDERER_PROFILE
#undef KROSS_RENDERER_PROFILE
#endif
#ifdef KROSS_ENABLE_ASSERTS
#undef KROSS_ENABLE_ASSERTS
#endif
#ifdef KROSS_FILE_LOG
#undef KROSS_FILE_LOG
#endif
#ifdef KROSS_PROFILING
#undef KROSS_PROFILING
#endif
#ifdef KROSS_RUNTIME_PROFILING
#undef KROSS_RUNTIME_PROFILING
#endif
#ifdef KROSS_FILE_PROFILING
#undef KROSS_FILE_PROFILING
#endif
#ifdef KROSS_GLAD
#undef KROSS_GLAD
#endif
#ifdef KROSS_DLL
#undef KROSS_DLL
#endif


#define GraphicsAPI OpenGL
#define KROSS_DEBUG 1
#define KROSS_GL_DEBUG 1
#define KROSS_GLCALL_PROFILE 0
#define KROSS_RENDERER_PROFILE 0
#define KROSS_ENABLE_ASSERTS 0
#define KROSS_FILE_LOG 0
#define KROSS_PROFILING 0
#define KROSS_RUNTIME_PROFILING 0
#define KROSS_FILE_PROFILING 0
#define KROSS_GLAD 1
#define KROSS_DLL 0

#endif // !CORE_CONFIG
