#pragma once
#include <vector>
#include <cstdint>
#include <string>

/// <summary>
/// Container class for obfuscation methods using the Blum Blum Shub algorithm.
/// </summary>
class Obfuscator {
public:
    /// <summary>
    /// Encrypt or decrypt a byte array using the Blum Blum Shub algorithm with XOR.
    /// </summary>
    /// <param name="data">The data to encrypt or decrypt.</param>
    /// <param name="initialValue">A passkey to provide deterministic encryption.</param>
    /// <returns>An encrypted/decrypted key stream.</returns>
    static std::vector<uint8_t> Crypt(const std::vector<uint8_t>& data, uint32_t initialValue);

    /// <summary>
    /// Converts a vector of uint8_t to a string.
    /// </summary>
    /// <param name="data">The vector of uint8_t to convert.</param>
    /// <returns>A string representation of the input vector.</returns>
    static std::string ConvertToString(const std::vector<uint8_t>& data);

private:
    static std::vector<uint8_t> BBS(uint64_t value, size_t length);
};