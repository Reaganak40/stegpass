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

#define SP_CLI_ACTION_HIDE 0
#define SP_CLI_ACTION_EXTRACT 1

namespace sp {

class CORE_API CLIParser
{
public:

	/// <summary>
	/// Constructor for the CLIParser class
	/// </summary>
	/// <param name="argc">The number of supported filetypes</param>
	/// <param name="supported_filetypes">The extensions to support (dot included)</param>
	CLIParser(int argc, const char** supported_filetypes);
	~CLIParser();

	/// <summary>
	/// Gets the supported filetype at the given index of the supported filetypes list.
	/// </summary>
	/// <param name="index">The index of the supported filetypes list</param>
	/// <returns>The supported filetype at the given index, or empty string if does not exist.</returns>
	const char* GetSupportedFileType(int index) const;

	/// <summary>
	/// Parses the command line arguments and returns the proper exit code.
	/// </summary>
	/// <param name="argc">The number of arguments passed from main-argc.</param>
	/// <param name="argv">The arguments passed from main-argv.</param>
	/// <param name="print_to_stderr">Whether or not to print the help message to stderr.</param>
	/// <returns>0 indicates successful parsing. 1 indicates an empty set of arguments and the usage example
	/// is sent to the user. 2 indicates an invalid argument was found.</returns>
	int Parse(int argc, const char* argv[], bool print_to_stderr = true);

	/// <summary>
	/// Gets the 256-bit hash to be used for encryption/decryption.
	/// </summary>
	/// <param name="out_hash">An out parameter to store the hash.</param>
	/// <returns>True if the hash was provided by the parsed arguments, or this is a default hash.</returns>
	bool GetHash(Hash256& out_hash) const;

	/// <summary>
	/// Gets the desired action to be performed.
	/// </summary>
	/// <returns>0 indicates hiding data, 1 indicates extracting data.</returns>
	int Action() const;

	/// <summary>
	/// Gets the target file to be used for data hiding/extraction.
	/// </summary>
	/// <returns>The target file to be used for data hiding/extraction.</returns>
	const char* GetTargetFile() const;

	/// <summary>
	/// Utility function to check if the target file exists
	/// </summary>
	/// <returns>True if the target file exists, false otherwise.</returns>
	bool TargetFileExists() const;

	/// <summary>
	/// Gets the password to be used for data hiding.
	/// </summary>
	/// <returns>The password to be used for data hiding.</returns>
	const char* GetPassword() const;

	/// <summary>
	/// Sets the root path for the CLIParser to use, when dealing with relative paths.
	/// </summary>
	/// <param name="root_path">The root path to set.</param>
	void SetRootPath(const char* root_path);

private:
	struct Impl;
	Impl* m_pimpl;

	bool VerifyEnoughArgs(int argc, const char* argv[], bool print_to_stderr) const;
	bool GetActionFromArgs(int index, const char* argv[], bool print_to_stderr);
	bool GetTargetFileFromArgs(int index, const char* argv[], bool print_to_stderr);
	bool GetPasswordFromArgs(int index, int argc, const char* argv[], bool print_to_stderr);
	bool VerifyFileExtension(const char* filename) const;
	bool GetHashFromArgs(int index, int argc, const char* argv[], bool print_to_stderr);
};

}