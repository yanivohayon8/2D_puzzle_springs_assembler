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
	//std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0";
	//std::string puzzleDirectory = "../data/ofir/RePAIR/group_39";

	std::map<std::string, std::string> param2Value;
	parseInput(param2Value,argc,argv);

	if (param2Value.count("test") > 0)
	{
		//--test VisualTwoReconstructs --puzzleDir "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0"
		//--test VisualGroundTruth
		RouteTests(param2Value);
	} else {
		/*if (param2Value.count("script") > 0)
		{
			if (param2Value["script"] == "http_server")
			{
		
			}
		}*/

		/*VisualReconstructor vsReconstructor;
		vsReconstructor.init();
		Puzzle puzzle();*/

		HTTPServer httpServer;
		httpServer.run();
	}




	/*DataLoader dataLoader(param2Value["puzzleDir"]);

	std::vector<VertexMating> trueMatings;
	dataLoader.loadVertexMatings(trueMatings, "springs_anchors_correct.csv");
	std::vector<Piece> pieces;
	dataLoader.loadPieces(pieces);
	Puzzle puzzle(pieces,trueMatings);
	
	VisualReconstructor vsReconstructor;
	vsReconstructor.init();
	
	std::vector<VertexMating> matings;
	dataLoader.loadVertexMatings(matings);
	std::vector<Piece> activePieces;
	puzzle.findPiecesToReconstruct(activePieces, matings);
	vsReconstructor.initRun(activePieces, matings);
	vsReconstructor.Run("../data/test1.png");
	vsReconstructor.closeRun();

	puzzle.findPiecesToReconstruct(activePieces, trueMatings);
	vsReconstructor.initRun(activePieces, trueMatings);
	vsReconstructor.Run("../data/test2.png");
	vsReconstructor.closeRun();*/

	/*SilentReconstructor silentReconstructor;
	silentReconstructor.init();
	silentReconstructor.initRun(activePieces, matings);
	silentReconstructor.Run("../data/deleteme.png");
	silentReconstructor.closeRun();*/
	



	//std::vector<Piece> pieces;
	//bool isOfir = true;
	//dataLoader.loadPieces(pieces);
	////dataLoader.loadExtraInfo(pieces);
	//std::vector<VertexMating> matings;
	//dataLoader.loadVertexMatings(matings);
	//std::vector<VertexMating> trueMatings;
	//dataLoader.loadVertexMatings(trueMatings,"ground_truth_rels.csv");

	//World world(pieces,matings,trueMatings);
	//world.Init();
	//world.Simulation(isSimulationAuto);
	//world.saveFinalTransforms(puzzleDirectory + "/final_transforms.csv");
	//std::cout << "Finish" << std::endl;

	return 0;
}

