#include "pch.h"
#include "core/CLIParser.hpp"

struct CLIParser::Impl
{
	std::vector<std::string> supported_filetypes;
};

CLIParser::CLIParser(int argc, const char** supported_filetypes)
	: m_pimpl(new CLIParser::Impl)
{
	for (int i = 0; i < argc; i++)
	{
		m_pimpl->supported_filetypes.push_back(supported_filetypes[i]);
	}
}

CLIParser::~CLIParser()
{
	delete m_pimpl;
}

const char* CLIParser::GetSupportedFileType(int index) const
{
	if (index < 0 || index >= m_pimpl->supported_filetypes.size())
	{
		return "";
	}

	return m_pimpl->supported_filetypes[index].c_str();
}
