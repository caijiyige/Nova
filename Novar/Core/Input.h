#pragma once

#include "Novar/Core/Core.h"
#include "Novar/Core/KeyCodes.h"
#include "Novar/Core/MouseButtonCodes.h"

namespace NV
{
    class Input{

        public:
        static bool IsKeyPressed(KeyCode keycode) ;
        static bool IsMouseButtonPressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX() ;
        static float GetMouseY() ;
    };
    
}