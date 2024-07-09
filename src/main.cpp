#include "http_server.h"
//#include "HttpServerCD.h"
//#include "HttpServerRePAIR.h"
#include <DataLoader.h>
#include <Piece.h>
////#include <World.h>
#include <VertexMating.h>
#include <ScriptInputParser.h>
#include <Puzzle.h>
#include <reconstruction.h>

int main(int argc, char** argv)
{
	std::map<std::string, std::string> param2Value;
	parseInput(param2Value,argc,argv);

	/*if (param2Value.count("ConvexDrawing"))
	{
		HttpServerCD httpServer;
		httpServer.run();
	}
	else
	{
		HttpServerRePAIR httpServer;
		httpServer.run();
	}
	*/

	HTTPServer httpServer;
	httpServer.run();

	return 0;
}

