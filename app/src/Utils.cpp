#include "pch.h"
#include "Utils.hpp"
#include "Widgets.hpp"
#include "Logging.hpp"

#ifdef SP_PLATFORM_WINDOWS
#include <Windows.h>
#include <commdlg.h>

// Function to convert std::string to std::wstring
std::wstring StringToWString(const std::string& s) {
    int len;
    int slength = static_cast<int>(s.length()) + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
    std::wstring wstr(len, L'\0');
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, &wstr[0], len);
    return wstr;
}

// Function to convert std::wstring to std::string
std::string WStringToString(const std::wstring& s) {
	int len;
	int slength = static_cast<int>(s.length()) + 1;
	len = WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, 0, 0, 0, 0);
	std::string str(len, '\0');
	WideCharToMultiByte(CP_ACP, 0, s.c_str(), slength, &str[0], len, 0, 0);
	return str;
}

#endif

ImVec2 sp::GetWindowPositionByID(const char* windowID)
{
    ImVec2 position(0.0f, 0.0f);

    // Set the current window context to the window with the specified ID
    if (ImGui::Begin(windowID))
    {
        // Get the position of the current window
        position = ImGui::GetWindowPos();
    }
    ImGui::End();

    return position;
}

ImVec2 sp::GetMenuBarSize()
{
    ImVec2 size(0.0f, 0.0f);

    ImGui::BeginMainMenuBar();
	size = ImGui::GetWindowSize();
    ImGui::EndMainMenuBar();

	return size;
}

std::string sp::GeneratePassword(size_t length)
{
    std::string password;
	password.reserve(length);

	// Generate a random password
	for (size_t i = 0; i < length; i++)
	{
		password.push_back((char)(rand() % 94 + 33));
	}

	return password;
}

std::string sp::OpenFileDialog(const std::string& title, const std::string& filter)
{
#ifdef SP_PLATFORM_WINDOWS
    
    std::wstring titleW = StringToWString(title);
    std::wstring filterW = StringToWString(filter);

    OPENFILENAME ofn;           // common dialog box structure
    wchar_t szFile[260] = { 0 };// buffer for file name
    HWND hwnd = NULL;           // owner window
    HANDLE hf;                  // file handle

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = hwnd;
    ofn.lpstrFile = szFile;
    ofn.lpstrFile[0] = '\0';
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = filterW.c_str();
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Display the Open dialog box
    if (GetOpenFileName(&ofn) == TRUE) {
        return WStringToString(ofn.lpstrFile);
    }
    return "";

#else
    #error "Unsupported platform for file dialog."
#endif
}

