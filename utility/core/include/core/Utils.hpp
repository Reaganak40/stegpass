/***************************************************************/
/*  core/CLIParser.hpp                                         */
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
* Headers
***************************************************************/
#include "core/Export.hpp"

/*
	* RETURN CODES:
	* 0: Success
	* 1: Not enough arguments
	* 2: Invalid argument(s)
	* 3: File does not exist
	* 4: Could not recover password
	* -1: Unknown/Unexpected error
*/

namespace sp {

	/// <summary>
	/// The exit codes for the program.
	/// </summary>
	enum ExitCode {
		Success = 0,
		NotEnoughArguments = 1,
		InvalidArguments = 2,
		FileDoesNotExist = 3,
		CouldNotRecoverPassword = 4,
		UnknownError = -1
	};

	/// <summary>Converts a string to a hash</summary>
	/// <param name="hash">The out parameter for the hash </param>
	/// <param name="hashString">The string to convert to a hash </param>
	/// <returns>True if the hash was successfully converted, false otherwise </returns>
	CORE_API bool StringToHash(Hash256& hash, const char* hashString);

	/// <summary>
	/// Checks if a character is a hex digit
	/// </summary>
	/// <param name="c">The character to check </param>
	/// <returns>True if the character is a hex digit, false otherwise </returns>
	CORE_API bool IsHexDigit(char c);

	/// <summary>
	/// Checks if a file is open by another process
	/// </summary>
	/// <param name="filename">The file to check </param>
	/// <returns>True if the file is open by another process, false otherwise </returns>
	CORE_API bool IsFileOpenByAnotherProcess(const char* filename);
}