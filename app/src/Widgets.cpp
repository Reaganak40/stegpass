#include "pch.h"
#include "Widgets.hpp"
#include "Logging.hpp"

void sp::DrawMenuBar()
{
    // Create the main menu bar
    if (ImGui::BeginMainMenuBar())
    {
        // Create a "File" menu
        if (ImGui::BeginMenu("File"))
        {
            // Add menu items to the "File" menu
            if (ImGui::MenuItem("Open", "Ctrl+O")) { /* Handle open action */ }
            if (ImGui::MenuItem("Save", "Ctrl+S")) { /* Handle save action */ }
            if (ImGui::MenuItem("Exit", "Alt+F4")) { /* Handle exit action */ }

            // End the "File" menu
            ImGui::EndMenu();
        }

        // Create an "Edit" menu
        if (ImGui::BeginMenu("Edit"))
        {
            // Add menu items to the "Edit" menu
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) { /* Handle undo action */ }
            if (ImGui::MenuItem("Redo", "Ctrl+Y", false, false)) { /* Handle redo action */ } // Disabled item
            if (ImGui::MenuItem("Cut", "Ctrl+X")) { /* Handle cut action */ }
            if (ImGui::MenuItem("Copy", "Ctrl+C")) { /* Handle copy action */ }
            if (ImGui::MenuItem("Paste", "Ctrl+V")) { /* Handle paste action */ }

            // End the "Edit" menu
            ImGui::EndMenu();
        }

        // End the main menu bar
        ImGui::EndMainMenuBar();
    }
}

/***************************************************************
* FONT MANAGER
***************************************************************/
sp::FontManager* sp::FontManager::m_instance = nullptr;

void sp::FontManager::Init()
{
    if (m_instance != nullptr) {
        return;
    }

    m_instance = new FontManager;
}

void sp::FontManager::Destroy()
{
    if (m_instance == nullptr) {
		return;
	}

    delete m_instance;
	m_instance = nullptr;
}

bool sp::FontManager::AddFont(const std::string& alias, const std::string& resource_path, float font_size)
{
    // check if the font already exists
    std::string key = alias + std::to_string(font_size);
    if (m_instance->m_fonts.find(key) != m_instance->m_fonts.end()) {
        SP_LOG_WARN("AddFont() - Font already exists: {}", alias);
		return false;
	}

    // check if resource path is valid
    if (!std::filesystem::exists(resource_path)) {
        SP_LOG_ERROR("AddFont() - Font resource not found: {}", resource_path);
        return false;
    }


    ImGuiIO& io = ImGui::GetIO();
    ImFont* nFont = io.Fonts->AddFontFromFileTTF(resource_path.c_str(), font_size);
    if (nFont == nullptr)
	{
        SP_LOG_ERROR("AddFont() - Failed to load font: {}", alias);
		return false;
	}

    m_instance->m_fonts[key] = nFont;
    return true;
}

ImFont* sp::FontManager::GetFont(const std::string& alias, float font_size)
{
    std::string key = alias + std::to_string(font_size);
	if (m_instance->m_fonts.find(key) == m_instance->m_fonts.end()) {
		return nullptr;
	}

	return m_instance->m_fonts[key];
}
