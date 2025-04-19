#pragma once



#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/fmt/ostr.h"
#include <spdlog/sinks/basic_file_sink.h>

namespace NV{
    class NOVA_API Log
    {
        public:
        static void Init();
        inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }    
        inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

        private:

        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;

    };

   




}

    // Core log macros
#define NV_CORE_TRACE(...)    ::NV::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define NV_CORE_INFO(...)     ::NV::Log::GetCoreLogger()->info(__VA_ARGS__)
#define NV_CORE_WARN(...)     ::NV::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define NV_CORE_ERROR(...)    ::NV::Log::GetCoreLogger()->error(__VA_ARGS__)


// Client log macros
#define NV_TRACE(...)         ::NV::Log::GetClientLogger()->trace(__VA_ARGS__)
#define NV_INFO(...)          ::NV::Log::GetClientLogger()->info(__VA_ARGS__)
#define NV_WARN(...)          ::NV::Log::GetClientLogger()->warn(__VA_ARGS__)
#define NV_ERROR(...)         ::NV::Log::GetClientLogger()->error(__VA_ARGS__)
