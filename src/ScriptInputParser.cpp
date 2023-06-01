#include "ScriptInputParser.h"

void parseInput(bool& oIsSimulationAuto, std::string &oPuzzleDirectory, int argc, char** argv)
{
	for (int i = 0; i < argc; i++)
	{
		std::string token = argv[i];


		if (token=="--puzzleDir")
		{
			oPuzzleDirectory = argv[i + 1];			
			continue;
		}

		if (token == "--ManualSimulation")
		{
			oIsSimulationAuto = false;
			continue;
		}

	}
}