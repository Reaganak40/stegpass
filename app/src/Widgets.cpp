#include "pch.h"
#include "Widgets.hpp"
#include "Logging.hpp"
#include "Utils.hpp"
#include "Window.hpp"
#include "Image.hpp"
#include "IconsForkAwesome.h"

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
* ADD PASSWORD FORM
***************************************************************/
struct PasswordFormData {
    sp::Image image;
    std::string image_error_message;
};
// data shared across implementation functions
static PasswordFormData password_form_data;

void sp::DrawAddPasswordForm()
{
    // Set the window position and size for the form
    static ImVec2 window_pos = ImVec2(10.0f, GetMenuBarSize().y + 10.0f);
    static ImVec2 window_size = ImVec2(WindowSpecs::WIDTH / 3 - (ImGui::GetStyle().WindowPadding.x * 1.7),
        WindowSpecs::HEIGHT - GetMenuBarSize().y - (ImGui::GetStyle().WindowPadding.y * 2));

    // fonts to be used in the form
    ImFont* password_font = FontManager::GetFont("PasswordDots", SP_DEFAULT_FONT_SIZE);
    ImFont* font_awesome = FontManager::GetFont("FontAwesome", SP_DEFAULT_FONT_SIZE);

    // password configurations
    static int pc_password_length = 16;
    static bool pc_random_length = false;

    // Set flags for window form
    ImGuiWindowFlags window_flags = 0;
    window_flags |= ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoResize;
    window_flags |= ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoCollapse;

    // Set the window position and size
    ImGui::SetNextWindowPos(window_pos);
    ImGui::SetNextWindowSize(window_size);

	// create new ImGui window
    ImGui::Begin(alias::ADD_PASSWORD_FORM, NULL, window_flags);

    // calculate the width for the password fields to fill the window
    static float field_width = 0.0f;
    if (field_width == 0.0f) {
        	field_width = window_size.x - ImGui::GetStyle().WindowPadding.x * 3;

            ImGui::PushFont(font_awesome);
            field_width -= (ImGui::CalcTextSize(ICON_FK_EYE).x + ImGui::GetStyle().FramePadding.x * 2);
            ImGui::PopFont();
    }
	
	// determine if password should be shown in plain text
    static bool show_password_fields = false;
    auto update_button_text = [&]() {
		return (show_password_fields ? std::string(ICON_FK_EYE) : std::string(ICON_FK_EYE_SLASH)) + "###show_password_button";
	};
    static std::string show_password_button_text = update_button_text();

    // enter password field
    static char password[SP_MAX_PASSWORD_LENGTH] = "";
    bool hide_password = !show_password_fields && strnlen_s(password, SP_MAX_PASSWORD_LENGTH) > 0;
  
    if (hide_password) ImGui::PushFont(password_font);
    ImGui::PushItemWidth(field_width);
    ImGui::InputTextWithHint("##enter_password", "Enter Password", 
        password, SP_MAX_PASSWORD_LENGTH);
    ImGui::PopItemWidth();
    if (hide_password)  ImGui::PopFont();

    // show password button
    ImGui::PushFont(font_awesome);
    ImGui::SameLine();
    if (ImGui::Button(show_password_button_text.c_str())) {
        show_password_fields = !show_password_fields;
        show_password_button_text = update_button_text();
	}
    ImGui::PopFont();

    // confirm password field
    static char confirm_password[SP_MAX_PASSWORD_LENGTH] = "";
    bool hide_confirm_password = !show_password_fields && strnlen_s(confirm_password, SP_MAX_PASSWORD_LENGTH) > 0;
    
    if (hide_confirm_password) ImGui::PushFont(password_font); 
    ImGui::PushItemWidth(field_width);
    ImGui::InputTextWithHint("##confirm_password", "Confirm Password",
        confirm_password, SP_MAX_PASSWORD_LENGTH);
    ImGui::PopItemWidth();
    if (hide_confirm_password)  ImGui::PopFont();

    // error message text
    static std::string error_label = "";
    ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), error_label.c_str());

    // auto-generate password button
    ImGui::NewLine();
    if (ImGui::Button("Auto-Generate")) {

        size_t password_length = pc_password_length;
        if (pc_random_length) {
            password_length = rand() % (SP_MAX_PASSWORD_LENGTH - SP_MIN_PASSWORD_LENGTH) + SP_MIN_PASSWORD_LENGTH;
        }
        std::string new_password = GeneratePassword(password_length);

        // copy the generated password to the password fields
        strncpy_s(password, new_password.c_str(), SP_MAX_PASSWORD_LENGTH);
        strncpy_s(confirm_password, new_password.c_str(), SP_MAX_PASSWORD_LENGTH);

		SP_LOG_TRACE("Auto-generate password with length: {}", password_length);
	}

    // clear password fields and reset error message
    auto clear_password_text = [&]() {
        memset(password, 0, SP_MAX_PASSWORD_LENGTH);
        memset(confirm_password, 0, SP_MAX_PASSWORD_LENGTH);
        error_label.clear();
    };

    // clear password fields button
    ImGui::SameLine();
    if (ImGui::Button("Clear Fields")) {
        clear_password_text();
    }

    // Create a collapsing header
    ImGui::SetCursorPos(ImVec2(ImGui::GetStyle().WindowPadding.x, ImGui::GetCursorPos().y + 5));
    static float random_length_checkbox_width = ImGui::CalcTextSize("Random Length").x + 
        ImGui::GetFrameHeight() + ImGui::GetStyle().ItemInnerSpacing.x + ImGui::GetStyle().WindowPadding.x;
    ImGuiTreeNodeFlags password_config_flags = ImGuiTreeNodeFlags_DefaultOpen;
    if (ImGui::CollapsingHeader("Password Configuration", password_config_flags)) {
        
        // Create the slider for password length
        ImGui::NewLine();
        ImGui::Text("Password Length");
        ImGuiSliderFlags slider_flags = ImGuiSliderFlags_AlwaysClamp;

        ImGui::BeginDisabled(pc_random_length);
        static float slider_width = ImGui::GetContentRegionAvail().x - random_length_checkbox_width;
        ImGui::PushItemWidth(slider_width);
        ImGui::SliderInt("##password_length_slider", &pc_password_length, 
            SP_MIN_PASSWORD_LENGTH, SP_MAX_PASSWORD_LENGTH - 1, "%d", slider_flags);
        ImGui::PopItemWidth();
        ImGui::EndDisabled();

        // Create the random length checkbox
        ImGui::SameLine();
        ImGui::Checkbox("Random Length", &pc_random_length);
    }

    // Center 'Add Password' button at bottom of window
    ImVec2 button_size = ImGui::CalcTextSize("Add Password");
    button_size.x += ImGui::GetStyle().FramePadding.x * 2;
    button_size.y += ImGui::GetStyle().FramePadding.y * 2;
    
    float button_x = (window_size.x - button_size.x) / 2.0f;
    float button_y = window_size.y - button_size.y - ImGui::GetStyle().WindowPadding.y;
    ImGui::SetCursorPos(ImVec2(button_x, button_y));

    auto validate_entries = [&] () {
        size_t password_length = strnlen_s(password, SP_MAX_PASSWORD_LENGTH);
		if (password_length == 0) {
			error_label = "Password cannot be empty";
            SP_LOG_TRACE("Password validation failed: Password cannot be empty.");
			return false;
		}

        if (password_length < SP_MIN_PASSWORD_LENGTH) {
			error_label = "Password must be at least " + std::to_string(SP_MIN_PASSWORD_LENGTH) + " characters.";
            SP_LOG_TRACE("Password validation failed: Password must be at least {} characters.", SP_MIN_PASSWORD_LENGTH);
			return false;
		}

		if (strnlen_s(confirm_password, SP_MAX_PASSWORD_LENGTH) == 0) {
			error_label = "Confirm Password cannot be empty";
            SP_LOG_TRACE("Password validation failed: Confirm Password cannot be empty.");
			return false;
		}

		if (strcmp(password, confirm_password) != 0) {
			error_label = "Passwords do not match";
            SP_LOG_TRACE("Password validation failed: Passwords do not match.");
			return false;
		}

        error_label.clear();

        if (password_form_data.image.textureID == SP_NO_IMAGE_LOADED) {
            password_form_data.image_error_message = "Please load an image to hide the password";
            SP_LOG_TRACE("Password validation failed: No image loaded.");
			return false;
		}

		return true;
	};

    // procedure when form is ready to be processed
    auto on_add_password = [&]() {

        // clear all form entries
        clear_password_text();
        password_form_data.image.Reset();
    };

	// process the form
	if (ImGui::Button("Add Password")) {
        // validate entries
        if (validate_entries()) {
            on_add_password();
            SP_LOG_TRACE("Password added");
        }
	}

	// End the ImGui window
	ImGui::End();

}

void sp::DrawImageViewer()
{
    // set window position and size (next to the 'Add Password' form)
    static ImVec2 window_pos = ImVec2(0, 0);
    static ImVec2 window_size = ImVec2(
        (WindowSpecs::WIDTH / 3 * 2) - (ImGui::GetStyle().WindowPadding.x * 1.7),
        WindowSpecs::HEIGHT - GetMenuBarSize().y - (ImGui::GetStyle().WindowPadding.y * 2));

    // get the position and size of the 'Add Password' form
    if (window_pos.x == 0 && window_pos.y == 0) {
        ImGui::Begin(alias::ADD_PASSWORD_FORM);
        ImVec2 add_password_form_pos = ImGui::GetWindowPos();
        ImVec2 add_password_form_size = ImGui::GetWindowSize();
        ImGui::End();

        window_pos.x = add_password_form_pos.x + add_password_form_size.x + ImGui::GetStyle().WindowPadding.x;
        window_pos.y = add_password_form_pos.y;
	}

	// set flags for window form
	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoCollapse;

	// set the window position and size
	ImGui::SetNextWindowPos(window_pos);
	ImGui::SetNextWindowSize(window_size);

	// create new ImGui window
	ImGui::Begin("Image Viewer", NULL, window_flags);
    
	// load the image
    std::string image_path = "";

    // user dropped a file
    if (DragNDrop::IsFileDropped()) {
        if (ImGui::IsWindowHovered()) {
            image_path = DragNDrop::GetDroppedFile();
        }
    }

    // user clicked to open file dialog
    else if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(0)) {
        image_path = OpenFileDialog("Open Image", "Image Files (*.bmp)\0*.bmp\0");
    }

    if (!image_path.empty()) {
        if (GetImageFormat(image_path) == ImageFormat::SP_IMAGE_FORMAT_UNSUPPORTED) {
            password_form_data.image_error_message = "Cannot load: Unsupported image format.";
        }
        else {
            password_form_data.image.LoadTextureFromFile(image_path.c_str());
            password_form_data.image_error_message.clear();
        }
    }

    if (password_form_data.image.textureID == SP_NO_IMAGE_LOADED) {

        // display a message to load an image (centered)
        ImVec2 text_size = ImGui::CalcTextSize("Click to load an image to view");
        float text_x = (window_size.x - text_size.x) / 2.0f;
        float text_y = (window_size.y - text_size.y) / 2.0f;
        ImGui::SetCursorPos(ImVec2(text_x, text_y));
        ImGui::Text("Click to load an image to view");

        if (!password_form_data.image_error_message.empty()) {
			// display error message
			ImVec2 error_size = ImGui::CalcTextSize(password_form_data.image_error_message.c_str());
			float error_x = (window_size.x - error_size.x) / 2.0f;
			float error_y = text_y + text_size.y + ImGui::GetStyle().ItemSpacing.y;
			ImGui::SetCursorPos(ImVec2(error_x, error_y));
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), password_form_data.image_error_message.c_str());
		}
    }
    else {

        // determine max context width and height
        float max_width = window_size.x - ImGui::GetStyle().WindowPadding.x * 2;
        float max_height = window_size.y - ImGui::GetStyle().WindowPadding.y * 2;
        ImVec2 image_size = password_form_data.image.GetSizeWithMaintainedAspectRatio(max_width, max_height);

        // center the image in the window
        ImVec2 image_pos = ImVec2((max_width - image_size.x) / 2.0f, (max_height - image_size.y) / 2.0f);
        ImGui::SetCursorPos(ImVec2(image_pos.x + ImGui::GetStyle().WindowPadding.x, image_pos.y + ImGui::GetStyle().WindowPadding.y));

        // display the image
        ImGui::Image((void*)(intptr_t)password_form_data.image.textureID, image_size);
    }

	// end the ImGui window
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

bool sp::FontManager::AddPasswordFont(const std::string& alias, const std::string& resource_path, float font_size)
{
    // check if the font already exists
    std::string key = alias + std::to_string(font_size);
    if (m_instance->m_fonts.find(key) != m_instance->m_fonts.end()) {
        SP_LOG_WARN("AddPasswordFont() - Font already exists: {}", alias);
        return false;
    }

    // check if resource path is valid
    if (!std::filesystem::exists(resource_path)) {
        SP_LOG_ERROR("AddPasswordFont() - Font resource not found: {}", resource_path);
        return false;
    }

    ImGuiIO& io = ImGui::GetIO();

    // define the glyph range for the password font (U+2022)
    static const ImWchar glyph_range[] = { 0x2022, 0x2022, 0 };
    ImFont* nFont = io.Fonts->AddFontFromFileTTF(resource_path.c_str(), font_size, nullptr, glyph_range);

    if (nFont == nullptr)
	{
		SP_LOG_ERROR("AddPasswordFont() - Failed to load font: {}", alias);
		return false;
	}

    nFont->FallbackChar = 0x2022;

    m_instance->m_fonts[key] = nFont;
	SP_LOG_TRACE("'{}' font added successfully.", alias);
	return true;
}

bool sp::FontManager::AddIconFont(const std::string& alias, const std::string& resource_path, float font_size)
{
    // check if the font already exists
	std::string key = alias + std::to_string(font_size);
	if (m_instance->m_fonts.find(key) != m_instance->m_fonts.end()) {
		SP_LOG_WARN("AddIconFont() - Font already exists: {}", alias);
		return false;
	}

	// check if resource path is valid
	if (!std::filesystem::exists(resource_path)) {
		SP_LOG_ERROR("AddIconFont() - Font resource not found: {}", resource_path);
		return false;
	}

    // load the icon font
	ImGuiIO& io = ImGui::GetIO();
    static const ImWchar glyph_range[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
    ImFontConfig config;
    config.MergeMode = true;
    config.PixelSnapH = true;

    ImFont* nFont = io.Fonts->AddFontFromFileTTF(resource_path.c_str(), font_size, &config, glyph_range);

	if (nFont == nullptr)
	{
		SP_LOG_ERROR("AddIconFont() - Failed to load font: {}", alias);
		return false;
	}

	m_instance->m_fonts[key] = nFont;
	SP_LOG_TRACE("'{}' font added successfully.", alias);
	return true;
}

/***************************************************************
* DRAGNDROP
***************************************************************/

sp::DragNDrop* sp::DragNDrop::m_instance = nullptr;

void sp::DragNDrop::Init()
{
    if (m_instance != nullptr) {
		return;
	}

    m_instance = new DragNDrop;
}

void sp::DragNDrop::Destroy()
{
    if (m_instance == nullptr) {
		return;
	}

	delete m_instance;
	m_instance = nullptr;
}

void sp::DragNDrop::OnFileDrop(GLFWwindow* window, int count, const char** paths)
{
    if (count == 0) {
        return;
    }

    if (count > 1) {
        SP_LOG_WARN("OnFileDrop() - Multiple files dropped, only the first file will be processed.");
    }

    m_instance->m_dropped_file = paths[0];
    SP_LOG_TRACE("OnFileDrop() - File dropped: {}", m_instance->m_dropped_file);
}

bool sp::DragNDrop::IsFileDropped()
{
    return !m_instance->m_dropped_file.empty();
}

std::string sp::DragNDrop::GetDroppedFile()
{
    return m_instance->m_dropped_file;
}

void sp::DragNDrop::FlagHandled()
{
    m_instance->m_dropped_file.clear();
}
