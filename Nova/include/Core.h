#pragma once

#ifdef NV_EXPORT
    #define NOVA_API __declspec(dllexport)
#else
    #define NOVA_API __declspec(dllimport)
#endif