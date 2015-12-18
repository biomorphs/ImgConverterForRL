#include "command_line_parameters.h"

int main(int argc, char *argv[])
{
	CommandLineParameters commandLineParams;
	if (commandLineParams.ParseCommandLine(argc, argv) != CommandLineParameters::ParseResult::OK)
	{
		commandLineParams.DisplayHelp();
		return 1;
	}
    return 0;
}

