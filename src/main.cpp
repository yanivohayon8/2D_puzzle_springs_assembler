#include "httpServer.h"
#include "scriptInputParser.h"

int main(int argc, char** argv)
{
	std::map<std::string, std::string> param2Value;
	
	parseInput(param2Value, argc, argv);
	int port = 8888;

	if (param2Value.count("port"))
	{
		port = std::stoi(param2Value["port"]);
	}

	HTTPServer httpServer(port);
	httpServer.run();

	return 0;
}

