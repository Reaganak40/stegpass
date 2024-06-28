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

namespace sp {

	/// <summary>
	/// Draws the main menu bar for the application.
	/// </summary>
	void DrawMenuBar();

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
	private:
		static FontManager* m_instance;
		std::unordered_map<std::string, ImFont*> m_fonts;

		FontManager() = default;
		~FontManager() = default;
	};
}