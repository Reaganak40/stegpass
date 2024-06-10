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

namespace sp {
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
	/// Takes a byte stream that was previously encrypted by the Obfuscator and attempts to restore it. The message
	/// bytes should include the metadata for its first byte, indicating the original length of the message, including
	/// the null-terminated character at the end. If the message does not match the metadata or does not end in a null
	/// terminator, the function will return nullptr.
	/// </summary>
	/// <param name="messageBytes">The byte stream to validate. This will be altered in the function.</param>
	/// <param name="messageSize">The size of the byte stream.</param>
	/// <param name="encryptionKey">The key to use for decryption.</param>
	/// <returns> A pointer to the validated message. If the message is invalid, nullptr is returned. It is up to you to free it.</returns>
	CORE_API char* ValidateMessage(uint8_t* messageBytes, size_t messageSize, const Hash256 encryptionKey);
}