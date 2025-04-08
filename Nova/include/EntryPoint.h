#pragma once

#ifdef NV_PLATFORM_WINDOWS

extern NV::Application* NV::CreateApplication();

int main(int argc, char** argv)
{
    printf("Nova Engine\n");
    auto app = NV::CreateApplication();
    app->Run();
    delete app;
}

#endif // NV_PLATFORM_WINDOWS