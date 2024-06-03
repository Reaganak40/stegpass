#include "BMP.hpp"
#include "Obfuscator.hpp"

#include <fstream>
#include <iostream>
#include <vector>

BMP::BMP(const std::string& filename) 
    : dataBytes(nullptr), encryptionKey(0x12345678)
{
    // Read the entire file into memory
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    dataBytes = new unsigned char[size];
    if (!file.read((char*)dataBytes, size)) {
        throw std::runtime_error("Failed to read file");
    }

    file.close();
}

void BMP::Info() const {
    FileHeader fileHeader;
    if (!GetFileHeader(fileHeader)) {
        throw std::runtime_error("Failed to get file header");
    }

    InfoHeader infoHeader;
    if (!GetInfoHeader(infoHeader)) {
        throw std::runtime_error("Failed to get info header");
    }

    std::cout << "File Header:" << std::endl;
    std::cout << "  bfType: " << fileHeader.bfType[0] << fileHeader.bfType[1] << std::endl;
    std::cout << "  bfSize: " << fileHeader.bfSize << std::endl;
    std::cout << "  bfOffBits: " << fileHeader.bfOffBits << std::endl;

    std::cout << "Info Header:" << std::endl;
    std::cout << "  biSize: " << infoHeader.biSize << std::endl;
    std::cout << "  biWidth: " << infoHeader.biWidth << std::endl;
    std::cout << "  biHeight: " << infoHeader.biHeight << std::endl;
    std::cout << "  biBitCount: " << infoHeader.biBitCount << std::endl;
}

int BMP::HideMessage(const std::string& message) {

    PixelArray pixelArray;
    if (!GetPixelArray(pixelArray)) {
        throw std::runtime_error("Failed to get pixel array");
    }

    // Encrypt the message using the Obfuscator class
    std::vector<uint8_t> messageBytes(message.begin(), message.end());
    std::vector<uint8_t> encryptedMessage = Obfuscator::Crypt(messageBytes, encryptionKey);

    // Add metadata (message length) to the beginning of the encrypted message
    uint8_t messageLength = static_cast<uint8_t>(message.size());
    encryptedMessage.insert(encryptedMessage.begin(), messageLength);

    // Hide the encrypted message in the BMP file
    unsigned char* messagePtr = encryptedMessage.data();
    unsigned char* pixelArrayPtr = pixelArray.data + pixelArray.rowSize - pixelArray.paddingSize;
    unsigned int messageSize = message.size() + 1; // Include null terminator
    for (unsigned int i = 0; i < pixelArray.numRows; ++i) {
        for (unsigned int j = 0; j < pixelArray.paddingSize && messageSize > 0; ++j) {
            *pixelArrayPtr++ = *messagePtr++;
            --messageSize;
        }
        pixelArrayPtr += pixelArray.rowSize - pixelArray.paddingSize;
    }

    // If the message is too long, try to hide it in Gap1
    Gap gap1;
    if (messageSize > 0 && GetGap1(gap1)) {
        unsigned char* gap1Ptr = gap1.data;
        for (unsigned int i = 0; i < gap1.size && messageSize > 0; ++i) {
            *gap1Ptr++ = *messagePtr;
            messagePtr++;
            --messageSize;
        }
    }

    // If the message is still too long, try to hide it in Gap2
    Gap gap2;
    if (messageSize > 0 && GetGap2(gap2)) {
        // If Gap2 is not big enough, reallocate memory for the byte stream
        if (gap2.size < messageSize) {
            unsigned char* newDataBytes = new unsigned char[gap2.data - dataBytes + messageSize];
            std::copy(dataBytes, gap2.data, newDataBytes);
            delete[] dataBytes;
            dataBytes = newDataBytes;
            gap2.data = dataBytes + (gap2.data - dataBytes);
            gap2.size = messageSize;
        }

        unsigned char* gap2Ptr = gap2.data;
        for (unsigned int i = 0; i < messageSize; ++i) {
            *gap2Ptr++ = *messagePtr++;
        }
    }

    // Return the status of hiding the message
    if (messageSize > 0) {
        return 0; // Failed to hide the message
    }
    else if (GetGap2(gap2) && gap2.size > 0) {
        return 3; // Hid the message in Gap2
    }
    else if (GetGap1(gap1) && gap1.size > 0) {
        return 2; // Hid the message in Gap1
    }
    else {
        return 1; // Hid the message in the padding of the pixel array
    }
}

bool BMP::Save(const std::string& filename) const {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        return false;
    }

    FileHeader fileHeader;
    if (!GetFileHeader(fileHeader)) {
        return false;
    }

    file.write((char*)dataBytes, fileHeader.bfSize);

    return true;
}

std::string BMP::ExtractMessage() const {
    PixelArray pixelArray;
    if (!GetPixelArray(pixelArray)) {
        throw std::runtime_error("Failed to get pixel array");
    }

    std::vector<uint8_t> messageBytes;

    unsigned char* pixelArrayPtr = pixelArray.data + pixelArray.rowSize - pixelArray.paddingSize;

    // Read the message length from the metadata
    uint8_t messageLength;
    if (pixelArray.paddingSize == 0) {
        Gap gap1;
        if (GetGap1(gap1)) {
            unsigned char* gap1Ptr = gap1.data;
            messageLength = *gap1Ptr++;
            messageBytes.push_back(messageLength);
            // Read the rest of the message based on the message length
            uint8_t bytesRead = 0;
            while (bytesRead < messageLength && gap1.size > 0) {
                messageBytes.push_back(*gap1Ptr++);
                bytesRead++;
                gap1.size--;
            }
        }
        else {
            Gap gap2;
            if (GetGap2(gap2)) {
                unsigned char* gap2Ptr = gap2.data;
                messageLength = *gap2Ptr++;
                messageBytes.push_back(messageLength);
                // Read the rest of the message based on the message length
                uint8_t bytesRead = 0;
                while (bytesRead < messageLength && gap2.size > 0) {
                    messageBytes.push_back(*gap2Ptr++);
                    bytesRead++;
                    gap2.size--;
                }
            }
        }
    }
    else {
        messageLength = *(pixelArrayPtr) + 1;
        // Read the rest of the message based on the message length
        uint8_t bytesRead = 0;
        for (unsigned int i = 0; i < pixelArray.numRows; ++i) {
            for (unsigned int j = 0; j < pixelArray.paddingSize && bytesRead < messageLength; ++j) {
                messageBytes.push_back(*pixelArrayPtr++);
                bytesRead++;
            }
            pixelArrayPtr += pixelArray.rowSize - pixelArray.paddingSize;
        }
    }

    // If the message is too long, read from Gap1 and Gap2
    if (messageBytes.size() < messageLength + 1) {
        Gap gap1;
        if (GetGap1(gap1)) {
            unsigned char* gap1Ptr = gap1.data;
            while (messageBytes.size() < messageLength + 1 && gap1.size > 0) {
                messageBytes.push_back(*gap1Ptr++);
                gap1.size--;
            }
        }

        Gap gap2;
        if (messageBytes.size() < messageLength + 1 && GetGap2(gap2)) {
            unsigned char* gap2Ptr = gap2.data;
            while (messageBytes.size() < messageLength + 1 && gap2.size > 0) {
                messageBytes.push_back(*gap2Ptr++);
                gap2.size--;
            }
        }
    }

    return ValidateMessage(messageBytes);
}

void BMP::SetEncryptionKey(uint32_t key)
{
    encryptionKey = key;
}

bool BMP::GetFileHeader(FileHeader& fileHeader) const {
    if (dataBytes == nullptr) {
        return false;
    }

    fileHeader.bfType[0] = dataBytes[0];
    fileHeader.bfType[1] = dataBytes[1];
    fileHeader.bfSize = *(int*)(dataBytes + 2);
    fileHeader.bfReserved1 = *(short*)(dataBytes + 6);
    fileHeader.bfReserved2 = *(short*)(dataBytes + 8);
    fileHeader.bfOffBits = *(int*)(dataBytes + 10);

    return true;
}

bool BMP::GetInfoHeader(InfoHeader& infoHeader) const {
    if (dataBytes == nullptr) {
        return false;
    }

    FileHeader fileHeader;
    if (!GetFileHeader(fileHeader)) {
        return false;
    }

    infoHeader.biSize = *(int*)(dataBytes + 14);
    infoHeader.biWidth = *(int*)(dataBytes + 18);
    infoHeader.biHeight = *(int*)(dataBytes + 22);
    infoHeader.biPlanes = *(short*)(dataBytes + 26);
    infoHeader.biBitCount = *(short*)(dataBytes + 28);
    infoHeader.biCompression = *(int*)(dataBytes + 30);
    infoHeader.biSizeImage = *(int*)(dataBytes + 34);
    infoHeader.biXPelsPerMeter = *(int*)(dataBytes + 38);
    infoHeader.biYPelsPerMeter = *(int*)(dataBytes + 42);
    infoHeader.biClrUsed = *(int*)(dataBytes + 46);
    infoHeader.biClrImportant = *(int*)(dataBytes + 50);

    return true;
}

bool BMP::GetPixelArray(PixelArray& pixelArray) const {
    if (dataBytes == nullptr) {
        return false;
    }

    FileHeader fileHeader;
    if (!GetFileHeader(fileHeader)) {
        return false;
    }

    InfoHeader infoHeader;
    if (!GetInfoHeader(infoHeader)) {
        return false;
    }

    pixelArray.data = dataBytes + fileHeader.bfOffBits;
    pixelArray.pixelArraySize = infoHeader.biSizeImage;
    pixelArray.rowSize = (infoHeader.biWidth * infoHeader.biBitCount + 31) / 32 * 4;
    pixelArray.paddingSize = pixelArray.rowSize - infoHeader.biWidth * infoHeader.biBitCount / 8;
    pixelArray.numRows = infoHeader.biHeight;

    return true;
}

bool BMP::GetGap1(Gap& gap1) const {
    if (dataBytes == nullptr) {
        return false;
    }

    FileHeader fileHeader;
    if (!GetFileHeader(fileHeader)) {
        return false;
    }

    InfoHeader infoHeader;
    if (!GetInfoHeader(infoHeader)) {
        return false;
    }

    gap1.data = dataBytes + sizeof(FileHeader) + sizeof(InfoHeader);
    gap1.size = fileHeader.bfOffBits - sizeof(FileHeader) - sizeof(InfoHeader);

    return gap1.size > 0;
}

bool BMP::GetGap2(Gap& gap2) const {
    if (dataBytes == nullptr) {
        return false;
    }

    FileHeader fileHeader;
    if (!GetFileHeader(fileHeader)) {
        return false;
    }

    InfoHeader infoHeader;
    if (!GetInfoHeader(infoHeader)) {
        return false;
    }

    gap2.data = dataBytes + fileHeader.bfOffBits + infoHeader.biSizeImage;
    gap2.size = fileHeader.bfSize - fileHeader.bfOffBits - infoHeader.biSizeImage;

    return true;
}

std::string BMP::ValidateMessage(const std::vector<uint8_t>& messageBytes) const {
    if (messageBytes.empty()) {
        return "";
    }

    // Get the message length from the metadata
    uint8_t messageLength = messageBytes[0];

    // Check if the message length is valid
    if (messageLength >= messageBytes.size()) {
        return "";
    }

    // Decrypt the message using the Obfuscator class
    std::vector<uint8_t> messageBytesWithoutMetadata(messageBytes.begin() + 1, messageBytes.begin() + messageLength + 1);
    std::vector<uint8_t> decryptedMessage = Obfuscator::Crypt(messageBytesWithoutMetadata, encryptionKey);

    // Validate the decrypted message
    for (char c : decryptedMessage) {
        if (c < 32 || c > 126) {
            return "";
        }
    }

    return Obfuscator::ConvertToString(decryptedMessage);
}
