#pragma once
#include "Novar/Debug/Instrumentor.h"
#ifdef NV_PLATFORM_WINDOWS

extern NV::Application* NV::CreateApplication();

int main(int argc, char** argv)
{
    NV::Log::Init();
    NV_PROFILE_BEGIN_SESSION("Startup", "NovaProfile-Startup.json");
    auto app = NV::CreateApplication();
    NV_PROFILE_END_SESSION();
    NV_PROFILE_BEGIN_SESSION("Runtime", "NovaProfile-Runtime.json");
    app->Run();
    NV_PROFILE_END_SESSION();
    NV_PROFILE_BEGIN_SESSION("ShutDown", "NovaProfile-ShutDown.json");
    delete app;
    NV_PROFILE_END_SESSION();
}

#endif // NV_PLATFORM_WINDOWS