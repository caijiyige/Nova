#include "Window.h"

#include "Novar/PlatForm/Windows/WindowsWindow.h"


namespace NV{
    std::unique_ptr<Window> Window::Create(const WindowProps& props)
    {
        return std::make_unique<WindowsWindow>(props);
    } 
}
