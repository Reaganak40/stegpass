#include "pch.h"
#include "core/Utils.hpp"
#include "core/Obfuscator.hpp"

bool sp::StringToHash(Hash256& hash, const char* c_hashString)
{
	std::string hashString(c_hashString);

	if (hashString.size() != 64)
	{
		return false;
	}

	for (size_t i = 0; i < 32; ++i) {
		char c1 = hashString[i * 2];
		char c2 = hashString[i * 2 + 1];

		if (!IsHexDigit(c1) || !IsHexDigit(c2))
		{
			return false;
		}

		std::string byteString = hashString.substr(i * 2, 2);
		hash[i] = static_cast<uint8_t>(std::stoul(byteString, nullptr, 16));
	}

	return true;
}

bool sp::IsHexDigit(char c)
{
	return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F');
}

char* sp::ValidateMessage(uint8_t* messageBytes, size_t messageSize, const Hash256 encryptionKey)
{
	if (messageBytes == nullptr || messageSize <= 1) {
		return nullptr;
	}

	// Get the message length from the metadata
	uint8_t messageLength = messageBytes[0];
	if (messageLength < messageSize - 1) {
		return nullptr;
	}


	// Decrypt the message using the Obfuscator class
	sp::Obfuscator::Crypt(messageBytes + 1, messageLength, encryptionKey);

	// ensure the message is null-terminated
	if (messageBytes[messageLength] != '\0') {
		return nullptr;
	}

	// convert to string
	char* message = new char[messageLength];
	memcpy(message, messageBytes + 1, messageLength);

	// check if all characters are ascii-friendly
	for (size_t i = 0; i < messageLength - 1; i++) {
		if (message[i] < 32 || message[i] > 126) {
			delete[] message;
			return nullptr;
		}
	}

	return message;
}
