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

/***************************************************************
* Obfuscator Class
***************************************************************/

namespace sp {
/// <summary>
/// Container class for encryption and decryption methods.
/// </summary>
class CORE_API Obfuscator {
public:

    /// <summary>
    /// Encrypt or decrypt a byte array using the Blum Blum Shub algorithm with XOR.
    /// </summary>
    /// <param name="data">The data to encrypt or decrypt.</param>
    /// <param name="data_size">The size of the data array.</param>
    /// <param name="encryption_key">A passkey to provide deterministic encryption.</param>
    /// <returns>An encrypted/decrypted key stream.</returns>
    static void Crypt(uint8_t data[], size_t data_size, const Hash256 encryption_key);

    /// <summary>
    /// Generate a psuedo-random byte array using the Blum Blum Shub algorithm.
    /// </summary>
    /// <param name="seed">The seed value to start the algorithm.</param>
    /// <param name="length">The length of the byte array to generate.</param>
    /// <param name="bytes">The byte array to store the generated bytes.</param>
    static void BBS(const Hash256 seed, size_t length, uint8_t bytes[]);

};
}