#include "pch.h"
#include "core/MessageBlock.hpp"
#include "core/Obfuscator.hpp"
#include "core/Utils.hpp"

template<typename T, std::size_t N>
class ConstexprSet {
public:
	constexpr ConstexprSet(const std::array<T, N>& elements) : elements_(elements) {
		std::sort(elements_.begin(), elements_.end());
	}

	constexpr bool contains(const T& value) const {
		return std::binary_search(elements_.begin(), elements_.end(), value);
	}

private:
	std::array<T, N> elements_;
};

constexpr std::array<std::tuple<int, int, int>, 1> sp_version_history = {
	std::make_tuple(SP_VERSION_MAJOR, SP_VERSION_MINOR, SP_VERSION_PATCH),
};
constexpr ConstexprSet<std::tuple<int, int, int>, sp_version_history.size()> sp_version_set(sp_version_history);


struct sp::MessageBlock::Impl
{
	const Hash256* encryption_key = nullptr;
	std::vector<uint8_t> byte_stream;
};


sp::MessageBlock::MessageBlock(const Hash256& encryption_key)
	: m_impl(new Impl())
{
	m_impl->encryption_key = &encryption_key;
	
	// reserve space for the magic number, version, and message length
	m_impl->byte_stream.reserve(6);
	m_impl->byte_stream.push_back((SP_MAGIC_NUMBER >> 8) & 0xFF);
	m_impl->byte_stream.push_back(SP_MAGIC_NUMBER & 0xFF);
	m_impl->byte_stream.push_back(SP_VERSION_MAJOR);
	m_impl->byte_stream.push_back(SP_VERSION_MINOR);
	m_impl->byte_stream.push_back(SP_VERSION_PATCH);
	m_impl->byte_stream.push_back(0);
}

sp::MessageBlock::~MessageBlock()
{
	delete m_impl;
}

void sp::MessageBlock::SetMessage(const char* message)
{
	// include the null terminator in the message length
	m_impl->byte_stream[SP_MESSAGE_LENGTH_INDEX] = static_cast<uint8_t>(strlen(message) + 1);
	
	m_impl->byte_stream.reserve(6 + m_impl->byte_stream[SP_MESSAGE_LENGTH_INDEX]);
	m_impl->byte_stream.resize(6);

	for (size_t i = 0; i < m_impl->byte_stream[SP_MESSAGE_LENGTH_INDEX]; i++)
	{
		m_impl->byte_stream.push_back(message[i]);
	}

	Obfuscator::Crypt(m_impl->byte_stream.data() + 6, m_impl->byte_stream[SP_MESSAGE_LENGTH_INDEX], *m_impl->encryption_key);
}

uint8_t* sp::MessageBlock::GetBytes(size_t& byte_size) const
{
	byte_size = m_impl->byte_stream.size();
	return m_impl->byte_stream.data();
}

bool sp::MessageBlock::AssignHeader(const uint8_t* bytes)
{
	bool result = true;
	if (bytes[0] != ((SP_MAGIC_NUMBER >> 8) & 0xFF) || bytes[1] != (SP_MAGIC_NUMBER & 0xFF))
	{
		result = false;
	}

	// overwrite the magic number to use for this message block (should be SP)
	m_impl->byte_stream[0] = bytes[0];
	m_impl->byte_stream[1] = bytes[1];


	if (!ValidateVersion(bytes[2], bytes[3], bytes[4]))
	{
		result = false;
	}

	// overwrite the version to use for this message block
	m_impl->byte_stream[2] = bytes[2];
	m_impl->byte_stream[3] = bytes[3];
	m_impl->byte_stream[4] = bytes[4];

	m_impl->byte_stream[SP_MESSAGE_LENGTH_INDEX] = bytes[SP_MESSAGE_LENGTH_INDEX];
	return result;
}

bool sp::MessageBlock::ValidateHeader(const uint8_t* bytes)
{
	if (bytes[0] != ((SP_MAGIC_NUMBER >> 8) & 0xFF) || bytes[1] != (SP_MAGIC_NUMBER & 0xFF))
	{
		return false;
	}

	if (!ValidateVersion(bytes[2], bytes[3], bytes[4]))
	{
		return false;
	}

	return true;
}

uint8_t sp::MessageBlock::GetVersionMajor() const
{
	return m_impl->byte_stream[2];
}

uint8_t sp::MessageBlock::GetVersionMinor() const
{
	return m_impl->byte_stream[3];
}

uint8_t sp::MessageBlock::GetVersionPatch() const
{
	return m_impl->byte_stream[4];
}

bool sp::MessageBlock::IsVersionValid() const
{
	return ValidateVersion(GetVersionMajor(), GetVersionMinor(), GetVersionPatch());
}

bool sp::MessageBlock::IsMagicNumberValid() const
{
	return m_impl->byte_stream[0] == ((SP_MAGIC_NUMBER >> 8) & 0xFF) && m_impl->byte_stream[1] == (SP_MAGIC_NUMBER & 0xFF);
}

uint8_t sp::MessageBlock::GetMessageLength() const
{
	return m_impl->byte_stream[SP_MESSAGE_LENGTH_INDEX];
}

char* sp::MessageBlock::DecryptMessage(uint8_t* message_byte_stream)
{
	if (message_byte_stream == nullptr) {
		return nullptr;
	}
	
	// we don't check header, assume it is valid...

	// Get the message length from the metadata
	uint8_t messageLength = message_byte_stream[SP_MESSAGE_LENGTH_INDEX];

	// Decrypt the message using the Obfuscator class
	sp::Obfuscator::Crypt(message_byte_stream + SP_MESSAGE_LENGTH_INDEX + 1, messageLength, *m_impl->encryption_key);


	// ensure no null bytes in the message
	for (size_t i = 0; i < messageLength; i++) {
		if (message_byte_stream[SP_MESSAGE_LENGTH_INDEX + i] == '\0') {
			return nullptr;
		}
	}

	// ensure the message is null-terminated
	if (message_byte_stream[SP_MESSAGE_LENGTH_INDEX + messageLength] != '\0') {
		return nullptr;
	}

	// convert to string
	char* message = new char[messageLength];
	memcpy(message, message_byte_stream + SP_MESSAGE_LENGTH_INDEX + 1, messageLength);
	return message;
}

size_t sp::MessageBlock::BytesLeftToRead(const uint8_t* bytes, size_t byte_size)
{
	if (byte_size < SP_HEADER_SIZE) {
		return SP_HEADER_SIZE - byte_size;
	}

	uint8_t message_length = bytes[SP_MESSAGE_LENGTH_INDEX];
	return (SP_HEADER_SIZE + message_length) - byte_size;
}

bool sp::MessageBlock::ValidateVersion(uint8_t version_major, uint8_t version_minor, uint8_t version_patch)
{
	return sp_version_set.contains(std::make_tuple(version_major, version_minor, version_patch));
}
