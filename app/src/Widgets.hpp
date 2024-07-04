/***************************************************************/
/*  app/Widgets.hpp                                            */
/* *************************************************************/
/*                 This file is a part of:                     */
/*             -- StegPass: Password Manager --                */
/*         https://github.com/Reaganak40/stegpass              */
/***************************************************************/
/*            Copyright(c) 2024 Reagan Kelley                  */
/*                                                             */
/*  Permission  is  hereby  granted, free  of charge, to  any  */
/*  person obtaining a copy of this  software and  associated  */
/*  documentation  files(the  "Software"), to  deal  in   the  */
/*  Software without restriction,including without limitation  */
/*  the   rights   to  use,  copy,  modify,  merge,  publish,  */
/*  distribute,  sublicense,  and  /or  sell  copies  of  the  */
/*  Software,  and to permit persons to whom the  Software is  */
/*  furnished to do so, subject to the following conditions:   */
/*                                                             */
/*  The  above  copyright  notice  and this permission notice  */
/*  shall  be  included in all copies or substantial portions  */
/*  of the Software.                                           */
/*                                                             */
/*  THE  SOFTWARE  IS PROVIDED  "AS IS",  WITHOUT WARRANTY OF  */
/*  ANY KIND,  EXPRESS OR IMPLIED, INCLUDING  BUT NOT LIMITED  */
/*  TO THE  WARRANTIES  OF  MERCHANTABILITY,  FITNESS  FOR  A  */
/*  PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT  SHALL  */
/*  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,  */
/*  DAMAGES OR OTHER  LIABILITY,  WHETHER  IN  AN  ACTION  OF  */
/*  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT  OF  OR IN  */
/*  CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS  */
/*  IN THE SOFTWARE.                                           */
/***************************************************************/

#pragma once

/// <summary>
/// Contains the aliases for the widgets in the application.
/// </summary>
namespace alias {

	constexpr const char* MENU_BAR = "menu_bar";
	constexpr const char* ADD_PASSWORD_FORM = "add_password_form";
}

namespace sp {

	/// <summary>
	/// Draws the main menu bar for the application.
	/// </summary>
	void DrawMenuBar();

	/// <summary>
	/// Draws the 'Add Password' form, which allows the user to add a new password.
	/// </summary>
	void DrawAddPasswordForm();

	/// <summary>
	/// Draws the window to load and display the image that will be used to hide the passwords.
	/// </summary>
	void DrawImageViewer();

	/// <summary>
	/// Manages the fonts for the application.
	/// </summary>
	class FontManager
	{
	public:
		/// <summary>
		/// Initializes the font manager.
		/// </summary>
		static void Init();

		/// <summary>
		/// Releases the font manager.
		/// </summary>
		static void Destroy();
		
		/// <summary>
		/// Adds a font to the font manager.
		/// </summary>
		/// <param name="alias">The alias for the font, to fetch it later. </param>
		/// <param name="resource_path">The path to the font resource.</param>
		/// <param name="font_size">The size of the font. </param>
		/// <returns>True if the font was added successfully, false otherwise.</returns>
		static [[nodiscard]] bool AddFont(const std::string& alias, const std::string& resource_path, float font_size);

		/// <summary>
		/// Fetches a font from the font manager.
		/// </summary>
		/// <param name="alias">The alias of the font to fetch. </param>
		/// <param name="font_size">The size of the font.</param>
		/// <returns>The font if it was found, nullptr otherwise.</returns>
		static [[nodiscard]] ImFont* GetFont(const std::string& alias, float font_size);

		/// <summary>
		/// Creates a password font, only loading the '*' glyph from the resource path.
		/// </summary>
		/// <param name="alias">The alias for the font. </param>
		/// <param name="resource_path">The path to the font resource. </param>
		/// <param name="font_size">The size of the font. </param>
		/// <returns>True if the font was added successfully, false otherwise. </returns>
		static [[nodiscard]] bool AddPasswordFont(const std::string& alias, const std::string& resource_path, float font_size);
		

		/// <summary>
		/// Adds an icon font to the font manager.
		/// </summary>
		/// <param name="alias">The alias for the font. </param>
		/// <param name="resource_path">The path to the font resource. </param>
		/// <param name="font_size">The size of the font. </param>
		/// <returns>True if the font was added successfully, false otherwise. </returns>
		static [[nodiscard]] bool AddIconFont(const std::string& alias, const std::string& resource_path, float font_size);
	
	private:
		static FontManager* m_instance;
		std::unordered_map<std::string, ImFont*> m_fonts;

		FontManager() = default;
		~FontManager() = default;
	};

	/// <summary>
	/// Handles the drag and drop functionality for the application.
	/// </summary>
	class DragNDrop
	{
	public:
		/// <summary>
		/// Initializes the drag and drop functionality.
		/// </summary>
		static void Init();

		/// <summary>
		/// Releases the drag and drop functionality.
		/// </summary>
		static void Destroy();

		/// <summary>
		/// Handles the file drop event.
		/// </summary>
		/// <param name="window">The window that received the drop event.</param>
		/// <param name="count">The number of files dropped. </param>
		/// <param name="paths">The paths to the files dropped. </param>
		static void OnFileDrop(GLFWwindow* window, int count, const char** paths);

		/// <summary>
		/// Checks if a file has been dropped this frame.
		/// </summary>
		/// <returns>True if a file has been dropped, false otherwise.</returns>
		[[nodiscard]] static bool IsFileDropped();

		/// <summary>
		/// Gets the file that was dropped.
		/// </summary>
		/// <returns>The file that was dropped.</returns>
		[[nodiscard]] static std::string GetDroppedFile();

		/// <summary>
		/// Sets the file dropped flag to false.
		/// </summary>
		static void FlagHandled();
	private:
		static DragNDrop* m_instance;
		std::string m_dropped_file;

		DragNDrop() = default;
		~DragNDrop() = default;
	};
}