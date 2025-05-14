#pragma once

#include "Novar/Core/PlatformDetection.h"

#include <memory>

#ifdef NV_DEBUG
	#if defined(NV_PLATFORM_WINDOWS)
		#define NV_DEBUGBREAK() __debugbreak()
	#elif defined(NV_PLATFORM_LINUX)
		#include <signal.h>
		#define NV_DEBUGBREAK() raise(SIGTRAP)
	#else
		#error "Platform doesn't support debugbreak yet!"
	#endif
	#define NV_ENABLE_ASSERTS
#else
	#define NV_DEBUGBREAK()
#endif

#define NV_EXPAND_MACRO(x) x
#define NV_STRINGIFY_MACRO(x) #x

#define BIT(x) (1 << x)

#define NV_BIND_EVENT_FN(fn) [this](auto&&... args) -> decltype(auto) { return this->fn(std::forward<decltype(args)>(args)...); }

namespace NV {

	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}

}

#include "Novar/Core/Log.h"






#ifdef NV_ENBALE_ASSERTS
    #define NV_ASSERT(x, ...) { if(!(x)) { NV_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define NV_CORE_ASSERT(x, ...) { if(!(x)) { NV_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define NV_ASSERT(x, ...)
    #define NV_CORE_ASSERT(x, ...)
#endif