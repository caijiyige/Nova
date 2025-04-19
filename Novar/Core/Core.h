#pragma once

#ifdef NV_EXPORT
    #define NOVA_API __declspec(dllexport)
#else
    #define NOVA_API __declspec(dllimport)
#endif



// Event categories
#define BIT(x) (1 << x)

#define BIND_EVENT_FN(x) std::bind(&x, this, std::placeholders::_1)



#ifdef NV_ENBALE_ASSERTS
    #define NV_ASSERT(x, ...) { if(!(x)) { NV_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
    #define NV_CORE_ASSERT(x, ...) { if(!(x)) { NV_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else
    #define NV_ASSERT(x, ...)
    #define NV_CORE_ASSERT(x, ...)
#endif