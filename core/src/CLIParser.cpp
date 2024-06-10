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
	Hash256 hash = { 0 };
	bool has_hash = false;
	int action = 0;
	std::string target_file = "";
	std::string password = "";
	std::string root_path = "";
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
	if (!VerifyEnoughArgs(argc, argv, print_to_stderr)) {
		return 1;
	}

	// Get the action
	if (!GetActionFromArgs(1, argv, print_to_stderr)) {
		return 2;
	}

	// Get the target file
	if (!GetTargetFileFromArgs(2, argv, print_to_stderr)) {
		return 2;
	}

	// Get the password (if applicable)
	int current_arg = 3;
	if (m_pimpl->action == SP_CLI_ACTION_HIDE)
	{
		if (!GetPasswordFromArgs(current_arg, argc, argv, print_to_stderr)) {
			return 2;
		}
		current_arg++;
	}

	// Check for hash argument
	if (argc > current_arg)
	{
		if (!GetHashFromArgs(current_arg, argc, argv, print_to_stderr)) {
			return 2;
		}

		current_arg += 2;
	}

	// check if there are any extra arguments
	if (argc > current_arg)
	{
		if (print_to_stderr)
		{
			std::cerr << "Invalid argument: " << argv[current_arg] << std::endl;
			std::cerr << "Usage: .\\" << argv[0] << ".exe -[action] [path_to_image_file] [optional_parameters...]" << std::endl;
			std::cerr << "Actions: -s (save password) [password], -g (get password)" << std::endl;
			std::cerr << "Optional parameters: -h [256bit hash] (encryption key)" << std::endl;
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
	return m_pimpl->target_file.c_str();
}

bool sp::CLIParser::TargetFileExists() const
{
	std::ifstream file(m_pimpl->target_file);
	return file.good();
}

const char* sp::CLIParser::GetPassword() const
{
	return m_pimpl->password.c_str();
}

void sp::CLIParser::SetRootPath(const char* root_path)
{
	m_pimpl->root_path = root_path;

	if (m_pimpl->root_path[m_pimpl->root_path.size() - 1] != '\\')
	{
		m_pimpl->root_path += "\\";
	}
}

bool sp::CLIParser::VerifyEnoughArgs(int argc, const char* argv[], bool print_to_stderr) const
{
	if (argc < 3)
	{
		if (print_to_stderr)
		{
			std::cerr << "Usage: .\\" << argv[0] << ".exe -[action] [path_to_image_file] [optional_parameters...]" << std::endl;
			std::cerr << "Actions: -s (save password) [password], -g (get password)" << std::endl;
			std::cerr << "Optional parameters: -h [256bit hash] (encryption key)" << std::endl;
		}
		return false;
	}

	return true;
}

bool sp::CLIParser::GetActionFromArgs(int index, const char* argv[], bool print_to_stderr)
{
	if (strcmp(argv[index], "-s") != 0 && strcmp(argv[index], "-g") != 0)
	{
		if (print_to_stderr)
		{
			std::cerr << "Invalid action: " << argv[index] << std::endl;
			std::cerr << "Usage: .\\" << argv[0] << ".exe -[action] [path_to_image_file] [optional_parameters...]" << std::endl;
			std::cerr << "Actions: -s (save password) [password], -g (get password)" << std::endl;
			std::cerr << "Optional parameters: -h [256bit hash] (encryption key)" << std::endl;
		}
		return false;
	}

	m_pimpl->action = strcmp(argv[index], "-s") == 0 ? SP_CLI_ACTION_HIDE : SP_CLI_ACTION_EXTRACT;
	return true;
}

bool sp::CLIParser::GetTargetFileFromArgs(int index, const char* argv[], bool print_to_stderr)
{
	// Verify the filepath to image is supported
	m_pimpl->target_file = argv[index];

	// replace all forward slashes with backslashes
	std::replace(m_pimpl->target_file.begin(), m_pimpl->target_file.end(), '/', '\\');

	if (!VerifyFileExtension(m_pimpl->target_file.c_str()))
	{
		if (print_to_stderr)
		{
			std::string file_extension = std::string(m_pimpl->target_file);

			if (file_extension.find_last_of(".") == std::string::npos)
			{
				std::cerr << "No file extension found: " << m_pimpl->target_file << std::endl;
			}
			else {
				file_extension = file_extension.substr(file_extension.find_last_of("."));
				toLowerCase(file_extension);
				std::cerr << "Invalid file extension: " << m_pimpl->target_file << std::endl;
			}

			std::cerr << "Supported file types: ";
			for (int i = 0; i < m_pimpl->supported_filetypes.size(); i++)
			{
				std::cerr << m_pimpl->supported_filetypes[i] << " ";
			}
			std::cerr << std::endl;
		}
		return false;
	}

	// If relative path, append the root path
	if (m_pimpl->target_file.find(":") == std::string::npos)
	{
		m_pimpl->target_file = m_pimpl->root_path + m_pimpl->target_file;
	}

	return true;
}

bool sp::CLIParser::GetPasswordFromArgs(int index, int argc, const char* argv[], bool print_to_stderr)
{
	// not enough arguments for -s action
	if (argc < index + 1 && m_pimpl->action == SP_CLI_ACTION_HIDE)
	{
		if (print_to_stderr)
		{
			std::cerr << "Password is a second required argument for -s action" << std::endl;
		}
		return false;
	}

	// indicates a hanging argument
	else if (argc == index + 2 && m_pimpl->action == SP_CLI_ACTION_HIDE)
	{
		if (!print_to_stderr) {
			return false;
		}

		// check if the next argument is an -h flag
		if (strcmp(argv[index], "-h") == 0)
		{
			std::cerr << "Password is a second required argument for -s action, but got -h action" << std::endl;
		}
		else {
			std::cerr << "Invalid optional parameter: " << argv[index + 1] << std::endl;
			std::cerr << "Usage: .\\" << argv[0] << ".exe -[action] [path_to_image_file] [optional_parameters...]" << std::endl;
			std::cerr << "Actions: -s (save password) [password], -g (get password)" << std::endl;
			std::cerr << "Optional parameters: -h [256bit hash] (encryption key)" << std::endl;
		}

		return false;
	}

	m_pimpl->password = argv[index];

	return true;
}

bool sp::CLIParser::VerifyFileExtension(const char* filename) const
{
	std::string file_extension = std::string(filename);

	// verify an extension exists
	if (file_extension.find_last_of(".") == std::string::npos)
	{
		return false;
	}

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

bool sp::CLIParser::GetHashFromArgs(int index, int argc, const char* argv[], bool print_to_stderr)
{
	// expected -h flag
	if (strcmp(argv[index], "-h") != 0)
	{
		if (print_to_stderr)
		{
			std::cerr << "Invalid optional parameter: " << argv[index] << std::endl;
			std::cerr << "Usage: .\\" << argv[0] << ".exe -[action] [path_to_image_file] [optional_parameters...]" << std::endl;
			std::cerr << "Actions: -s (save password) [password], -g (get password)" << std::endl;
			std::cerr << "Optional parameters: -h [256bit hash] (encryption key)" << std::endl;
		}
		return false;
	}

	// not enough arguments for -h action
	if (argc < index + 1)
	{
		if (print_to_stderr)
		{
			std::cerr << "Hash is required for -h action" << std::endl;
		}
		return false;
	}

	// verify the hash
	if (!sp::StringToHash(m_pimpl->hash, argv[index + 1]))
	{
		if (print_to_stderr)
		{
			std::cerr << "Invalid hash: " << argv[index + 1] << std::endl;
		}
		return false;
	}

	m_pimpl->has_hash = true;
	return true;
}
