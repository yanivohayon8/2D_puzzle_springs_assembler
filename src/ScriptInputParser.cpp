#include "ScriptInputParser.h"

void parseInput(std::map<std::string, std::string> &oParam2Value, int argc, char** argv)
{
	for (int i = 0; i < argc; i++)
	{
		std::string token = argv[i];


		if (token=="--puzzleDir")
		{
			//oPuzzleDirectory = argv[i + 1];			
			oParam2Value.insert({ "puzzleDir",argv[i + 1] });
			continue;
		}

		/*if (token == "--ManualSimulation")
		{
			oIsSimulationAuto = false;
			param2value.insert({ "ManualSimulation","false" });
			continue;
		}*/

	}
}