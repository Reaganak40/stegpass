#include "pch.h"
#include "core/Utils.hpp"

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
