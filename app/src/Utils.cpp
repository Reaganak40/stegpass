#include "pch.h"
#include "Utils.hpp"
#include "Widgets.hpp"
#include "Logging.hpp"

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

