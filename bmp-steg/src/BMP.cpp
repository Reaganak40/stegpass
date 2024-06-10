#include "BMP.hpp"

#include "core/Utils.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

std::string ConvertToString(uint8_t data[], size_t data_size)
{
    std::string result;
    for (size_t i = 0; i < data_size; i++) {
        result += data[i];
    }
    return result;
}

void ConvertToByteStream(const std::string& data, uint8_t* out)
{
    for (size_t i = 0; i < data.size(); i++) {
        out[i] = data[i];
    }
}

BMP::BMP(const std::string& filename) 
    : dataBytes(nullptr), encryptionKey { 0 }
{
    // Read the entire file into memory
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file) {
        throw std::runtime_error("Failed to open file");
    }

    dataSize = file.tellg();
    file.seekg(0, std::ios::beg);

    dataBytes = new unsigned char[dataSize];
    if (!file.read((char*)dataBytes, dataSize)) {
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
    std::vector<uint8_t> encryptedMessage(message.begin(), message.end());
    encryptedMessage.push_back('\0'); // Add null terminator
    sp::Obfuscator::Crypt(encryptedMessage.data(), encryptedMessage.size(), encryptionKey);

    // Add metadata (message length) to the beginning of the encrypted message
    unsigned int messageSize = static_cast<unsigned int>(message.size()) + 1; // Include null terminator
    encryptedMessage.insert(encryptedMessage.begin(), messageSize);
    messageSize++; // Include the metadata byte

    // Hide the encrypted message in the BMP pixel array
    uint8_t* messagePtr = encryptedMessage.data();
    unsigned char* pixelArrayPtr = pixelArray.data + pixelArray.rowSize - pixelArray.paddingSize;
    for (unsigned int i = 0; i < pixelArray.numRows; ++i) {
        for (unsigned int j = 0; j < pixelArray.paddingSize && messageSize > 0; ++j) {
            *pixelArrayPtr = *reinterpret_cast<unsigned char*>(messagePtr);
            
            ++pixelArrayPtr;
            ++messagePtr;
            --messageSize;
        }
        pixelArrayPtr += pixelArray.rowSize - pixelArray.paddingSize;
    }

    // If the message is too long, try to hide it in Gap1
    Gap gap1;
    if (messageSize > 0 && GetGap1(gap1)) {
        unsigned char* gap1Ptr = gap1.data;
        for (unsigned int i = 0; i < gap1.size && messageSize > 0; ++i) {
            *gap1Ptr = *reinterpret_cast<unsigned char*>(messagePtr);

            ++gap1Ptr;
            ++messagePtr;
            --messageSize;
        }
    }

    // If the message is still too long, try to hide it in Gap2
    Gap gap2;
    if (messageSize > 0 && GetGap2(gap2)) {
        // If Gap2 is not big enough, reallocate memory for the byte stream
        if (gap2.size < messageSize) {
            dataSize = gap2.data - dataBytes + messageSize;
            unsigned char* newDataBytes = new unsigned char[dataSize];
            std::copy(dataBytes, gap2.data, newDataBytes);
            delete[] dataBytes;
            dataBytes = newDataBytes;
            gap2.data = dataBytes + (gap2.data - dataBytes);
            gap2.size = messageSize;
        }

        // Hide the rest of the message in Gap2
        unsigned char* gap2Ptr = gap2.data;
        for (; messageSize > 0; --messageSize) {
            *gap2Ptr = *reinterpret_cast<unsigned char*>(messagePtr);

			++gap2Ptr;
			++messagePtr;
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
    file.write((char*)dataBytes, dataSize);
    file.close();

    return true;
}

std::string BMP::ExtractMessage() const {
    
    std::vector<uint8_t> messageBytes;

    auto IsComplete = [&]() {
        return messageBytes.size() > 0 && messageBytes[0] == messageBytes.size() - 1;
        };

    auto GetDecodedMessage = [&]() {
		char* decoded_message = sp::ValidateMessage(messageBytes.data(), messageBytes.size(), encryptionKey);
        if (decoded_message == nullptr) {
			return std::string("");
		}

		std::string message(decoded_message);
		delete[] decoded_message;
		return message;
	};

    // read from pixel array first, if possible.
    if (!ReadFromPixelArray(messageBytes) && messageBytes.size() > 0 && messageBytes[0] == 0) {
		return "";
	}

    // check if the message is complete after reading from the pixel array
    if (IsComplete()) {
        return GetDecodedMessage();
    }

    // read from Gap1 if the message is not complete
    if (!ReadFromGap1(messageBytes) && messageBytes.size() > 0 && messageBytes[0] == 0) {
        return "";
    }

    // check if the message is complete after reading from Gap1
    if (IsComplete()) {
        return GetDecodedMessage();
    }

    // read from Gap2 if the message is not complete
    if (!ReadFromGap2(messageBytes) && messageBytes.size() > 0 && messageBytes[0] == 0) {
		return "";
	}

	// check if the message is complete after reading from Gap2
    if (IsComplete()) {
		return GetDecodedMessage();
	}

    // if the message is not complete after reading from Gap2, message is invalid
	return "";
}

void BMP::SetEncryptionKey(const Hash256 key)
{
    for (int i = 0; i < 32; i++) {
        encryptionKey[i] = key[i];
    }
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

    // sanity checks
    if (fileHeader.bfType[0] != 'B' || fileHeader.bfType[1] != 'M') {
        throw std::runtime_error("Invalid BMP file, header does not match");
	}

    if (fileHeader.bfOffBits < 54) {
		throw std::runtime_error("Invalid BMP file, bfOffBits is too small");
    }

    if (fileHeader.bfOffBits > dataSize) {
        throw std::runtime_error("Invalid BMP file, bfOffBits out of bounds");
    }

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

uint8_t BMP::ReadFromPixelArray(std::vector<uint8_t>& messageBytes) const
{
    PixelArray pixelArray;
    if (!GetPixelArray(pixelArray)) {
        throw std::runtime_error("Failed to get pixel array");
    }

    // not padding space to read from
    if (pixelArray.paddingSize == 0) {
        return 0;
    }

    uint8_t messageLength;
    uint8_t bytesRead = 0;
    unsigned char* pixelArrayPtr = pixelArray.data + pixelArray.rowSize - pixelArray.paddingSize;

    // read the message length from the metadata
    if (messageBytes.size() == 0) {
        messageBytes.push_back(*pixelArrayPtr);
        pixelArrayPtr++;
    }
    messageLength = messageBytes[0];

    // sanity check: messageLength is too big
    if (messageLength > dataSize) {
        messageBytes[0] = 0;
        return 0;
    }

    unsigned int current_row = 0;
    unsigned int padding_index = 1;

    for (; current_row < pixelArray.numRows; ++current_row) {
        for (; padding_index < pixelArray.paddingSize && bytesRead < messageLength; padding_index++) {
			messageBytes.push_back(*pixelArrayPtr);
			
            bytesRead++;
            pixelArrayPtr++;
		}

		pixelArrayPtr += pixelArray.rowSize - pixelArray.paddingSize;
        padding_index = 0;
	}

    return bytesRead;
}

uint8_t BMP::ReadFromGap1(std::vector<uint8_t>& messageBytes) const
{
    Gap gap1;
    size_t start_size = messageBytes.size();

    // check if gap 1 exists
    if (!GetGap1(gap1)) {
		return 0;
	}

    // if message length is not yet found, try to read from gap 1
    if (messageBytes.size() == 0) {
		messageBytes.push_back(*gap1.data);

        // sanity check: messageLength is too big
        if (messageBytes[0] > dataSize) {
			messageBytes[0] = 0;
			return 0;
		}

        gap1.data++;
        gap1.size--;
	}

    size_t remaining_bytes = messageBytes[0] - messageBytes.size() + 1; // metadata size does not include itself 
    size_t bytes_able_to_read = std::min(remaining_bytes, static_cast<size_t>(gap1.size));
    messageBytes.insert(messageBytes.end(), gap1.data, gap1.data + bytes_able_to_read);

    return static_cast<uint8_t>(messageBytes.size() - start_size);
}

uint8_t BMP::ReadFromGap2(std::vector<uint8_t>& messageBytes) const
{
    Gap gap2;
	size_t start_size = messageBytes.size();

	// check if gap 2 exists
    if (!GetGap2(gap2)) {
		return 0;
	}

	// if message length is not yet found, try to read from gap 2
    if (messageBytes.size() == 0) {
		messageBytes.push_back(*gap2.data);

		// sanity check: messageLength is too big
        if (messageBytes[0] > dataSize) {
			messageBytes[0] = 0;
			return 0;
		}

		gap2.data++;
		gap2.size--;
	}

	size_t remaining_bytes = messageBytes[0] - messageBytes.size() - 1; // metadata size does not include itself 
	size_t bytes_able_to_read = std::min(remaining_bytes, static_cast<size_t>(gap2.size));
	messageBytes.insert(messageBytes.end(), gap2.data, gap2.data + bytes_able_to_read);

    return static_cast<uint8_t>(messageBytes.size() - start_size);
}
