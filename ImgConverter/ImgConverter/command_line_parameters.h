#ifndef IMGCONVERTER_COMMAND_LINE_PARAMS_INCLUDED
#define IMGCONVERTER_COMMAND_LINE_PARAMS_INCLUDED

#include <string>

// Simple helper class to wrap the parsing of command line options
class CommandLineParameters
{
public:
	CommandLineParameters();
	~CommandLineParameters();

	enum class ParseResult
	{
		MissingParameters,
		OK
	};
	ParseResult ParseCommandLine(int argc, char* argv[]);
	void DisplayHelp();

	const std::string& GetSourcePath() const;
	const std::string& GetDestinationPath() const;

private:
	std::string m_sourcePath;
	std::string m_destinationPath;
};

#endif