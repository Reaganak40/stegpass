#include "pch.h"
#include "core/Obfuscator.hpp"

static const uint64_t M = 0xE2089EA5;

void sp::Obfuscator::Crypt(uint8_t data[], size_t data_size, const Hash256 encryption_key)
{
	uint8_t* key_stream = new uint8_t[data_size];
	BBS(encryption_key, data_size, key_stream);

	for (size_t i = 0; i < data_size; i++)
	{
		data[i] ^= key_stream[i];
	}

	delete[] key_stream;
}

void sp::Obfuscator::BBS(const Hash256 seed, size_t length, uint8_t bytes[])
{
	std::array<uint64_t, 4> state;
	uint8_t state_index = 0;

	for (size_t i = 0; i < 4; i++)
	{
		state[i] = *reinterpret_cast<const uint64_t*>(seed + (i * 8));
	}

	for (size_t i = 0; i < length; i++)
	{
		uint64_t value = state[state_index] * state[state_index] % M;
		bytes[i] = static_cast<uint8_t>(value & 0xFF);

		state[state_index] = value;
		state_index = (state_index + 1) % 4;
	}
}
