/***************************************************************/
/*  core/MessageBlock.hpp                                      */
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

// The magic number for the SP message block
#define SP_MAGIC_NUMBER 0x5350 

// The size of the SP header in bytes
#define SP_HEADER_SIZE 6

// The index of the message length in the byte stream
#define SP_MESSAGE_LENGTH_INDEX 5

/*************************************************************************
*
*  The message byte stream should always match this format:
*
*  |      -- SP MESSAGE BLOCK HEADER --         |    -- DATA --     |
*  | Magic Number | SP Version | Message Length | Encrypted Message |
*  | 2 bytes      | 3 bytes    | 1 byte         | n bytes           |
* 
* 
*  Stegonograhy utilities should use this class in the following way:
* 
*  FOR HIDING MESSAGES:
* 
*  1. Create a MessageBlock object with the encryption key.
*  2. Set the message to be encrypted (in plaintext).
*  3. Get the contiguous byte stream and to hide in the image.
*  4. Use a reversable algorithm to hide the byte stream in the image, 
      non-contiguously.
*
*  FOR EXTRACTING MESSAGES:
*  1. Using the reverse algorithm, start reading bytes in the order
*     by which they were written to disk.
*  2. After reading the header (first 6 bytes), use AssignHeader to
*     validate the header.
*  3. If the header is valid, use GetMessageLength to read forward
*     the number of bytes specified.
*  4. Use DecryptMessage to decrypt the message.
*
**************************************************************************/

namespace sp {

/// <summary>
/// The contiguous representation of an encrypted message. Used to build byte streams for writing, 
/// or to parse byte streams for reading, validating along the way.
/// </summary>
class CORE_API MessageBlock {
public:
    
    /// <summary>
    /// Initializes a new instance of the MessageBlock class.
    /// <param name="encryption_key">The encryption key to use for the message. Not copied, ensure the key does not go out of scope.</param>
    /// </summary>
    MessageBlock(const Hash256& encryption_key);
    ~MessageBlock();

    /// <summary>
    /// Set the unencrypted message to be encrypted and stored.
    /// </summary>
    /// <param name="message">The null-terminated message to be encrypted and stored.</param>
    void SetMessage(const char* message);

    /// <summary>
    /// Gets the contiguous representation of the SP message block, including the magic number and version and encrypted byte stream.
    /// </summary>
    /// <param name="byte_size">The size of the byte stream.</param>
    /// <returns>The byte stream of the message block, ensure to keep the MessageBlock in-scope while using this.</returns>
    uint8_t* GetBytes(size_t& byte_size) const;

    /// <summary>
    /// Attempts to translate the byte stream into a message block header.
    /// </summary>
    /// <param name="bytes">The byte stream to translate.</param>
    /// <returns>True if the header was successfully assigned (a valid header), false otherwise.</returns>
    bool AssignHeader(const uint8_t* bytes);

    /// <summary>
    /// Checks if the byte stream has a valid SP header.
    /// </summary>
    /// <param name="bytes">The byte stream to validate.</param>
    /// <returns>True if the header is valid, false otherwise.</returns>
    static bool ValidateHeader(const uint8_t* bytes);

    /// <summary>
    /// Gets the SP-major version that the message block was created with.
    /// </summary>
    /// <returns>The SP-major version, which will be the current version unless AssignHeader is used.</returns>
    uint8_t GetVersionMajor() const;

    /// <summary>
    /// Gets the SP-minor version that the message block was created with.
    /// </summary>
    /// <returns>The SP-minor version, which will be the current version unless AssignHeader is used.</returns>
    uint8_t GetVersionMinor() const;

    /// <summary>
    /// Gets the SP-patch version that the message block was created with.
    /// </summary>
    /// <returns>The SP-patch version, which will be the current version unless AssignHeader is used.</returns>
    uint8_t GetVersionPatch() const;

    /// <summary>
    /// Checks if the SP version is valid, either now or in the past.
    /// </summary>
    /// <returns>True if the SP version is valid, false otherwise.</returns>
    bool IsVersionValid() const;

    /// <summary>
    /// Checks if the magic number is valid.
    /// </summary>
    /// <returns>True if the magic number is valid, false otherwise.</returns>
    bool IsMagicNumberValid() const;

    /// <summary>
    /// Gets the message length of the encrypted message.
    /// </summary>
    /// <returns>If not message is set or no header is assigned, returns 0.</returns>
    uint8_t GetMessageLength() const;


    /// <summary>
    /// Takes a byte stream that was previously encrypted by the Obfuscator and attempts to restore it. The message
    /// bytes should include the header and the bytes following corresponding to the message length.
    /// </summary>
    /// <param name="message_byte_stream">The byte stream to validate. Should include the header and message. (assumes header is valid).</param>
    /// <returns> A pointer to the validated message. If the message is invalid, nullptr is returned. It is up to you to free it.</returns>
    char* DecryptMessage(uint8_t* message_byte_stream);

    /// <summary>
    /// Given a byte stream, returns the number of bytes left to read to either complete the message or header.
    /// </summary>
    /// <param name="bytes">The byte stream to read from.</param>
    /// <param name="byte_size">The size of the byte stream.</param>
    /// <returns>The number of bytes left to read.</returns>
    static size_t BytesLeftToRead(const uint8_t* bytes, size_t byte_size);

private:
    struct Impl;
    struct Impl* m_impl;

    static bool ValidateVersion(uint8_t version_major, uint8_t version_minor, uint8_t version_patch);
};

}