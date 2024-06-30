#include "pch.h"
#include "Widgets.hpp"
#include "Logging.hpp"
#include "Utils.hpp"

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

void sp::DrawAddPasswordForm()
{
    static ImVec2 window_pos = ImVec2(10.0f, GetMenuBarSize().y + 10.0f);
    static ImVec2 window_size = ImVec2(400, 200);
    ImFont* password_font = FontManager::GetFont("PasswordDots", 12.0f);

    // Set flags for window form
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    // Set the window position and size
    ImGui::SetNextWindowPos(window_pos);
    ImGui::SetNextWindowSize(window_size);

	// Create ImGui window
    ImGui::Begin(alias::ADD_PASSWORD_FORM, NULL, window_flags);
	
	// Add input fields
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_Password;

    static char password[SP_MAX_PASSWORD_LENGTH] = "";
    bool hide_password = strnlen_s(password, SP_MAX_PASSWORD_LENGTH) > 0;
  
    if (hide_password)  ImGui::PushFont(password_font);
    ImGui::InputTextWithHint("##enter_password", "Enter Password", 
        password, SP_MAX_PASSWORD_LENGTH, input_text_flags);
    if (hide_password)  ImGui::PopFont();

    static char confirm_password[SP_MAX_PASSWORD_LENGTH] = "";
    bool hide_confirm_password = strnlen_s(confirm_password, SP_MAX_PASSWORD_LENGTH) > 0;

    if (hide_confirm_password)  ImGui::PushFont(password_font);
    ImGui::InputTextWithHint("##confirm_password", "Confirm Password",
        confirm_password, SP_MAX_PASSWORD_LENGTH, input_text_flags);
    if (hide_confirm_password)  ImGui::PopFont();

    // Center button at bottom of window
    ImVec2 button_size = ImGui::CalcTextSize("Add Password");
    button_size.x += ImGui::GetStyle().FramePadding.x * 2;
    button_size.y += ImGui::GetStyle().FramePadding.y * 2;
    
    float button_x = (window_size.x - button_size.x) / 2.0f;
    float button_y = window_size.y - button_size.y - ImGui::GetStyle().WindowPadding.y;
    ImGui::SetCursorPos(ImVec2(button_x, button_y));

	// Process the form
	if (ImGui::Button("Add Password")) {
        SP_LOG_TRACE("Password added.");

        // Clear the input fields
        memset(password, 0, SP_MAX_PASSWORD_LENGTH);
        memset(confirm_password, 0, SP_MAX_PASSWORD_LENGTH);
	}

	// End the ImGui window
	ImGui::End();

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

    SP_LOG_TRACE("'{}' font added successfully.", alias);
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
