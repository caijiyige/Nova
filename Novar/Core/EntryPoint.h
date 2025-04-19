#pragma once

#ifdef NV_PLATFORM_WINDOWS

extern NV::Application* NV::CreateApplication();

int main(int argc, char** argv)
{
    NV::Log::Init();
    NV_CORE_TRACE("Initialized Log");
    NV_TRACE("Initialized Log");


    printf("Nova Engine\n");
    auto app = NV::CreateApplication();
    app->Run();
    delete app;
}

#endif // NV_PLATFORM_WINDOWS