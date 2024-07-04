/***************************************************************/
/*  app/Image.hpp                                              */
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

/***************************************************************
* Functions for loading and handling images.
***************************************************************/

// indicates that no image is loaded for an associated texture ID
#define SP_NO_IMAGE_LOADED 0

namespace sp {

	enum ImageFormat {
		SP_IMAGE_FORMAT_UNSUPPORTED,
		SP_IMAGE_FORMAT_BMP,
	};

	/// <summary>
	/// Get the image format of a file.
	/// </summary>
	/// <param name="filename">The file to get the image format of. </param>
	/// <returns>The image format of the file. </returns>
	[[nodiscard]] ImageFormat GetImageFormat(const std::string& filename);

	struct Image {
		GLuint textureID = SP_NO_IMAGE_LOADED; // the texture ID for this image
		int width = 0;   // the width of the image in pixels
		int height = 0;  // the height of the image pixels
		std::string filename = ""; // the filename of the image
		
		/// <summary>
		/// Load a texture from a file.
		/// </summary>
		/// <param name="filename">The file to load the texture from.</param>
		/// <returns>True if the texture was loaded successfully, false otherwise </returns> 
		bool LoadTextureFromFile(const char* filename);

		/// <summary>
		/// Get the size of the image with the aspect ratio maintained.
		/// </summary>
		/// <param name="maxWidth">The maximum width of the image.</param>
		/// <param name="maxHeight">The maximum height of the image.</param>
		/// <returns>The size of the image with the aspect ratio maintained. </returns>
		[[nodiscard]] ImVec2 GetSizeWithMaintainedAspectRatio(float maxWidth, float maxHeight) const;

		/// <summary>
		/// Reset the image data.
		/// </summary>
		void Reset();
	};
}