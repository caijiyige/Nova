#include "Novar/Utils/PlatformUtils.h"

#include <windows.h>
#include <commdlg.h>
#include <glfw3.h>

#define GLFW_EXPOSE_NATIVE_WIN32
#include <glfw3native.h>
#include <shlobj.h>

namespace NV
{
    std::string FileDialog::OpenFile(const char* filter)
    {
        OPENFILENAME ofn;
        char szFile[260];
        HWND hwnd = glfwGetWin32Window(glfwGetCurrentContext());

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter  = filter;
        
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

        if (GetOpenFileName(&ofn) == TRUE)
            return std::string(ofn.lpstrFile);

        return std::string();
    }

    std::string FileDialog::SaveFile(const char* filter)
    {
        OPENFILENAME ofn;
        char szFile[260];
        HWND hwnd = glfwGetWin32Window(glfwGetCurrentContext());

        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hwnd;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = filter;
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_OVERWRITEPROMPT;

        if (GetSaveFileName(&ofn) == TRUE)
            return std::string(ofn.lpstrFile);

        return std::string();
    }

    std::string FileDialog::OpenFolder()
    {
        BROWSEINFO bi = { 0 };
        bi.lpszTitle = "Select a folder";
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

        LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
        if (pidl != NULL)
        {
            char path[MAX_PATH];
            if (SHGetPathFromIDList(pidl, path))
            {
                return std::string(path);
            }
        }
        return std::string();
    }

    std::string FileDialog::OpenProject()
    {
        BROWSEINFO bi = { 0 };
        bi.lpszTitle = "Select a project folder";
        bi.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

        LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
        if (pidl != NULL)
        {
            char path[MAX_PATH];
            if (SHGetPathFromIDList(pidl, path))
            {
                return std::string(path);
            }
        }
        return std::string();
    }
}