#include "Window.h"

#include "Novar/PlatForm/Windows/WindowsWindow.h"


namespace NV{
    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    } 
}
