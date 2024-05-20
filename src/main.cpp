//#include "http_server.h"
#include "HttpServerCD.h"
#include "HttpServerRePAIR.h"
#include <DataLoader.h>
#include <Piece.h>
////#include <World.h>
#include <VertexMating.h>
#include <ScriptInputParser.h>
#include <Puzzle.h>
#include <reconstruction.h>
//#include <TestHandler.h>
//#include <cpp-httplib/httplib.h>

int main(int argc, char** argv)
{
	std::map<std::string, std::string> param2Value;
	parseInput(param2Value,argc,argv);

	if (param2Value.count("server") > 0)
	{
		if (param2Value.at("server") == "ConvexDrawing")
		{

			HttpServerCD httpServer;
			httpServer.run();
		}
		else if (param2Value.at("server") == "RePAIR") {
			HttpServerRePAIR httpServer;
			httpServer.run();
		}
	}
	else
	{
		std::cout << "Please provide the task you want to execute (test\server)" << std::endl;
	}
	

	return 0;
}

