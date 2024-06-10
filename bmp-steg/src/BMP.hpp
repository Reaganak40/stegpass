#pragma once

#include <core/Obfuscator.hpp>
#include <string>
#include <vector>

/// <summary>
/// A class for reading and writing BMP files, and hiding messages in them.
/// </summary>
class BMP {
public:
    // BMP file header structure
    struct FileHeader {
        char bfType[2]; // "BM" (0x42, 0x4D)
        int bfSize; // File size in bytes
        short bfReserved1; // Reserved
        short bfReserved2; // Reserved
        int bfOffBits; // Offset to pixel array
    };

    // BMP info header structure (Windows)
    struct InfoHeader {
        int biSize; // Info header size in bytes
        int biWidth; // Image width in pixels
        int biHeight; // Image height in pixels
        short biPlanes; // Number of color planes
        short biBitCount; // Bits per pixel
        int biCompression; // Compression method
        int biSizeImage; // Image size in bytes
        int biXPelsPerMeter; // Horizontal resolution
        int biYPelsPerMeter; // Vertical resolution
        int biClrUsed; // Number of colors used
        int biClrImportant; // Important colors
    };

    // BMP structure for maintaining structure alignment (either before or after the pixel array)
    struct Gap {
		unsigned char* data;
		int size;
	};

    // BMP pixel array structure
    struct PixelArray {
        unsigned char* data;
        unsigned int pixelArraySize;

        unsigned int rowSize;
        unsigned int paddingSize;
        unsigned int numRows;
    };

    /// <summary>
    /// Constructor that reads a BMP file
    /// </summary>
    /// <param name="filename">The path to a BMP file</param>
    BMP(const std::string& filename);

    /// <summary>
    /// Prints information about the BMP file to standard output
    /// </summary>
    void Info() const;

    /// <summary>
    /// Hides a message in the BMP file, using the padding in the pixel array first, then Gap1 and Gap2
    /// if the message is too long.
    /// </summary>
    /// <param name="message">The message stored in the BMP file.</param>
    /// <returns>0 if the message failed to be added to the class' byte stream. 1 if only the padding of the pixel array was used. 2 if Gap1 was used, and 3 if Gap2 was also used.</returns>
    int HideMessage(const std::string& message);

    /// <summary>
    /// Saves the BMP file to a new file, with the hidden message.
    /// </summary>
    /// <param name="filename">The path to the new BMP file.</param>
    /// <returns>True if the file was saved successfully.</returns>
    bool Save(const std::string& filename) const;


    /// <summary>
    /// Extracts a message from the BMP file, starting with the padding in the pixel array, then Gap1 and Gap2.
    /// </summary>
    /// <returns>The message extracted from the BMP file. If no message is found, returns an empty string.</returns>
    std::string ExtractMessage() const;

    /// <summary>
    /// Sets the obfuscation key used for encrypting and decrypting hidden messages.
    /// </summary>
    /// <param name="key">The obfuscation key.</param>
    void SetEncryptionKey(const Hash256 key);

private:
    
    // The BMP file's byte buffer
    unsigned char* dataBytes;
    size_t dataSize;

    Hash256 encryptionKey;

    /// <summary>
    /// Gets the BMP file header from the byte buffer
    /// </summary>
    /// <param name="fileHeader">A reference to a structure that will be modified to hold the file header information.</param>
    /// <returns>True, if the assignment was successful.</returns>
    bool GetFileHeader(FileHeader& fileHeader) const;

    /// <summary>
    /// Gets the BMP info header from the byte buffer
    /// </summary>
    /// <param name="infoHeader">A reference to a structure that will be modified to hold the info header information.</param>
    /// <returns>True, if the assignment was successful.</returns>
    bool GetInfoHeader(InfoHeader& infoHeader) const;

    /// <summary>
    /// Gets the BMP pixel array from the byte buffer
    /// </summary>
    /// <param name="pixelArray">A reference to the structure that will be modified to point to the pixel array.</param>
    /// <returns>True, if the assignment was successful.</returns>
    bool GetPixelArray(PixelArray& pixelArray) const;

    /// <summary>
    /// Gets the BMP pixel array from the byte buffer
    /// </summary>
    /// <param name="pixelArray">A reference to the structure that will changed to point to the gap bytes before the pixel array.</param>
    /// <returns>True, if the assignment was successful. Fails if this is a compact BMP and no gap 1 exists.</returns>
    bool GetGap1(Gap& gap1) const;

    /// <summary>
    /// Gets the BMP pixel array from the byte buffer
    /// </summary>
    /// <param name="gap2">A reference to the structure that will changed to point to the gap bytes after the pixel array.</param>
    /// <returns>True if there is no gap after the pixel array.</returns>
    bool GetGap2(Gap& gap2) const;

    /// <summary>
    /// Attempts to recover bytes from the pixel array.
    /// </summary>
    /// <param name="pixelArray"> The pixel array structure to read from.</param>
    /// <param name="messageBytes">The vector to store the message bytes.</param>
    /// <returns>The number of bytes read from the pixel array.</returns>
    uint8_t ReadFromPixelArray(std::vector<uint8_t>& messageBytes) const;

    /// <summary>
    /// Attempts to recover bytes from Gap1.
    /// </summary>
    /// <param name="messageBytes">The vector to store the message bytes.</param>
    /// <returns>The number of bytes read from Gap1.</returns>
    uint8_t ReadFromGap1(std::vector<uint8_t>& messageBytes) const;

    /// <summary>
    /// Attempts to recover bytes from Gap2.
    /// </summary>
    /// <param name="messageBytes">The vector to store the message bytes.</param>
    /// <returns>The number of bytes read from Gap2.</returns>
    uint8_t ReadFromGap2(std::vector<uint8_t>& messageBytes) const;
};