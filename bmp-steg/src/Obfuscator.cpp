#include "Obfuscator.hpp"

static const uint64_t M = 0xE2089EA5;

std::vector<uint8_t> Obfuscator::Crypt(const std::vector<uint8_t>& data, uint32_t initialValue) {
    std::vector<uint8_t> result;
    std::vector<uint8_t> bbsStream = BBS(initialValue, data.size());
    for (size_t i = 0; i < data.size(); i++) {
        result.push_back(data[i] ^ bbsStream[i]);
    }
    return result;
}

std::vector<uint8_t> Obfuscator::BBS(uint64_t value, size_t length) {
    std::vector<uint8_t> bytes(length);
    for (size_t i = 0; i < length; i++) {
        value = (value * value) % M;
        bytes[i] = static_cast<uint8_t>(value & 0xFF);
    }
    return bytes;
}

std::string Obfuscator::ConvertToString(const std::vector<uint8_t>& data) {
    std::string result;
    for (uint8_t byte : data) {
        result += static_cast<char>(byte);
    }
    return result;
}