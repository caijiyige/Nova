#pragma once

#include <string>


namespace NV
{
    class FileDialog
    {
    public:
        static std::string OpenFile(const char* filter);
        static std::string SaveFile(const char* filter);
        static std::string OpenFolder();
        static std::string OpenProject();
    };

}