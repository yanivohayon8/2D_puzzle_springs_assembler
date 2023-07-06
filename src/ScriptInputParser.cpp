#include "ScriptInputParser.h"

void parseInput(std::map<std::string, std::string> &oParam2Value, int argc, char** argv)
{

	for (int i = 0; i < argc; i++)
	{
		std::string token = argv[i];

		if (token.rfind("--",0)==0)
		{
			oParam2Value.insert({ token.substr(2,token.size()),argv[i + 1]});
			++i;
			continue;
		}

		//if (token=="--test")
		//{
		//	oParam2Value.insert({ "test",argv[i + 1] });
		//	continue;
		//}

		//if (token=="--puzzleDir")
		//{
		//	//oPuzzleDirectory = argv[i + 1];			
		//	oParam2Value.insert({ "puzzleDir",argv[i + 1] });
		//	continue;
		//}

	}
}