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

private:
	struct Impl;
	Impl* m_pimpl;
};