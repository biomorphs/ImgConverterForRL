#include "command_line_parameters.h"
#include <iostream>

CommandLineParameters::CommandLineParameters()
{

}

CommandLineParameters::~CommandLineParameters()
{

}

void CommandLineParameters::DisplayHelp()
{
	std::cout << "Usage: ImgConverter <sourcefile> <destfile>" << std::endl;
}

const std::string& CommandLineParameters::GetSourcePath() const
{
	return m_sourcePath;
}

const std::string& CommandLineParameters::GetDestinationPath() const
{
	return m_destinationPath;
}

CommandLineParameters::ParseResult CommandLineParameters::ParseCommandLine(int argc, char* argv[])
{
	// First argument is the executable path so 3 params are needed
	if (argc == 3 && argv != nullptr)
	{
		if (strlen(argv[1]) > 0 && strlen(argv[2]) > 0)
		{
			m_sourcePath = argv[1];
			m_destinationPath = argv[2];
			return ParseResult::OK;
		}
	}

	return ParseResult::MissingParameters;
}