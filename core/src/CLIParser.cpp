#include "pch.h"
#include "core/CLIParser.hpp"
#include "core/Utils.hpp"

void toLowerCase(std::string& str) {
	std::transform(str.begin(), str.end(), str.begin(),
		[](unsigned char c) { return std::tolower(c); });
}

struct sp::CLIParser::Impl
{
	std::vector<std::string> supported_filetypes;
	Hash256 hash;
	bool has_hash = false;
	int action = 0;
	std::string target_file;
};

sp::CLIParser::CLIParser(int argc, const char** supported_filetypes)
	: m_pimpl(new CLIParser::Impl)
{
	for (int i = 0; i < argc; i++)
	{
		std::string filetype = std::string(supported_filetypes[i]);
		toLowerCase(filetype);
		m_pimpl->supported_filetypes.push_back(filetype);
	}

	// the default hash is the hash if the user does not provide one later.
	sp::StringToHash(m_pimpl->hash, "7add733c75fc5f1ac1435a03f88dacf07bda3e5eba6f8c2520ec916843ed0a3e");
}

sp::CLIParser::~CLIParser()
{
	delete m_pimpl;
}

const char* sp::CLIParser::GetSupportedFileType(int index) const
{
	if (index < 0 || index >= m_pimpl->supported_filetypes.size())
	{
		return "";
	}

	return m_pimpl->supported_filetypes[index].c_str();
}

int sp::CLIParser::Parse(int argc, const char* argv[], bool print_to_stderr)
{
	// Check if the number of arguments is less than 3
	if (argc < 3) {
		// If true, display the usage and return 3
		if (print_to_stderr)
		{
			std::cerr << "Usage: .\\" << argv[0] << ".exe -[action] [path_to_image_file] [optional_parameters...]" << std::endl;
			std::cerr << "Actions: -s (save password) [password], -g (get password)" << std::endl;
			std::cerr << "Optional parameters: -h [256bit hash] (encryption key)" << std::endl;
		}
		return 1;
	}

	// Check if the action is valid and save it
	if (strcmp(argv[1], "-s") != 0 && strcmp(argv[1], "-g") != 0) {
			// If not, display the usage and return 2
			if (print_to_stderr)
			{
				std::cerr << "Invalid action: " << argv[1] << std::endl;
				std::cerr << "Usage: .\\" << argv[0] << ".exe -[action] [path_to_image_file] [optional_parameters...]" << std::endl;
				std::cerr << "Actions: -s (save password) [password], -g (get password)" << std::endl;
				std::cerr << "Optional parameters: -h [256bit hash] (encryption key)" << std::endl;
			}
			return 2;
	}

	if (strcmp(argv[1], "-s") == 0)
	{
		m_pimpl->action = SP_CLI_ACTION_HIDE;
	}
	else
	{
		m_pimpl->action = SP_CLI_ACTION_EXTRACT;
	}

	// Verify the filepath to image is supported
	m_pimpl->target_file = argv[2];
	if (!VerifyFileExtension(m_pimpl->target_file.c_str()))
	{
		if (print_to_stderr)
		{
			std::string file_extension = std::string(m_pimpl->target_file);
			file_extension = file_extension.substr(file_extension.find_last_of("."));
			toLowerCase(file_extension);

			std::cerr << "Invalid file extension: " << m_pimpl->target_file << std::endl;
			std::cerr << "Supported file types: ";
			for (int i = 0; i < m_pimpl->supported_filetypes.size(); i++)
			{
				std::cerr << m_pimpl->supported_filetypes[i] << " ";
			}
			std::cerr << std::endl;
		}
		return 2;
	}

	return 0;
}

bool sp::CLIParser::GetHash(Hash256& out_hash) const
{
if (!m_pimpl->has_hash)
	{
		return false;
	}

	for (int i = 0; i < 32; i++)
	{
		out_hash[i] = m_pimpl->hash[i];
	}
	
	return true;
}

int sp::CLIParser::Action() const
{
	return m_pimpl->action;
}

const char* sp::CLIParser::GetTargetFile() const
{
	return nullptr;
}

bool sp::CLIParser::VerifyFileExtension(const char* filename) const
{
	std::string file_extension = std::string(filename);
	file_extension = file_extension.substr(file_extension.find_last_of("."));
	toLowerCase(file_extension);

	for (int i = 0; i < m_pimpl->supported_filetypes.size(); i++)
	{
		if (file_extension == m_pimpl->supported_filetypes[i])
		{
			return true;
		}
	}

	return false;
}
