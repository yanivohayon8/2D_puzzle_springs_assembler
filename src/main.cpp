#include "http_server.h"
#include <DataLoader.h>
#include <Piece.h>
////#include <World.h>
#include <VertexMating.h>
#include <ScriptInputParser.h>
#include <Puzzle.h>
#include <reconstruction.h>
#include <TestHandler.h>
#include <cpp-httplib/httplib.h>

int main(int argc, char** argv)
{
	std::map<std::string, std::string> param2Value;
	parseInput(param2Value,argc,argv);

	if (param2Value.count("test") > 0)
	{
		//--test VisualTwoReconstructs --puzzleDir "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0"
		//--test VisualGroundTruth
		RouteTests(param2Value);
	} else {
		HTTPServer httpServer;
		httpServer.run();
	}

	return 0;
}

