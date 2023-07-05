#include <DataLoader.h>
#include <Piece.h>
#include <World.h>
#include <VertexMating.h>
#include <ScriptInputParser.h>
#include <Puzzle.h>

int main(int argc, char** argv)
{
	//std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0";
	//std::string puzzleDirectory = "../data/ofir/RePAIR/group_39";
	std::string puzzleDirectory; 
	bool isSimulationAuto = true;
	bool isOfir = true;
	parseInput(isSimulationAuto,puzzleDirectory,argc,argv);

	DataLoader dataLoader(puzzleDirectory);
	Puzzle puzzle(dataLoader);
	puzzle.initPuzzle();
	
	std::vector<VertexMating> Matings;
	dataLoader.loadVertexMatings(Matings);
	puzzle.initReconstruct(Matings);


	//std::vector<Piece> pieces;
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