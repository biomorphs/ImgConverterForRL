#include "command_line_parameters.h"
#include <iostream>
#include <algorithm>

CommandLineParameters::CommandLineParameters()
	: m_compressionMethod(CompressionMethod::UseMinMax)
{

}

CommandLineParameters::~CommandLineParameters()
{

}

void CommandLineParameters::DisplayHelp()
{
	std::cout << "Usage: ImgConverter <sourcefile> <destfile>" << std::endl;
}

CommandLineParameters::FileType CommandLineParameters::GetSourceFiletype() const
{
	return GetFiletypeFromPath(m_sourcePath);
}

CommandLineParameters::FileType CommandLineParameters::GetDestinationFiletype() const
{
	return GetFiletypeFromPath(m_destinationPath);
}

CommandLineParameters::CompressionMethod CommandLineParameters::GetCompressorMethod() const
{
	return m_compressionMethod;
}

CommandLineParameters::FileType CommandLineParameters::GetFiletypeFromPath(const std::string& path) const
{
	// This is pretty crude, but it does the job!
	// Note it will only really work with ANSI strings since tolower does not work
	// with utf8, etc.
	// Ideally we would use a cross-platform filesystem library to do this correctly, 
	// but the stdlib one isn't coming until c++17(!) so this will do for now

	// Pull out the file extension by grabbing anything in the string after the last '.'
	std::string foundExtension = "";
	const auto lastSeparator = path.find_last_of('.');	
	if (lastSeparator != std::string::npos)
	{
		foundExtension = path.substr(lastSeparator);
		std::transform(foundExtension.begin(), foundExtension.end(), foundExtension.begin(), tolower);
	}	

	if (foundExtension == ".dds")
	{
		return CommandLineParameters::FileType::DDS;
	}
	else if (foundExtension == ".bmp")
	{
		return CommandLineParameters::FileType::Bitmap;
	}
	else
	{
		return CommandLineParameters::FileType::Unknown;
	}
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