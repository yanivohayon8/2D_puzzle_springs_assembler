#include <DataLoader.h>
#include <Piece.h>
#include <World.h>
#include <VertexMating.h>
#include <ScriptInputParser.h>

int main(int argc, char** argv)
{
	//std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0";
	//std::string puzzleDirectory = "../data/ofir/RePAIR/group_39";
	std::string puzzleDirectory; 
	bool isSimulationAuto = true;
	bool isOfir = true;
	parseInput(isSimulationAuto,puzzleDirectory,argc,argv);

	DataLoader dataLoader(puzzleDirectory);
	std::vector<Piece> pieces;
	dataLoader.loadPieces(pieces, isOfir);
	//dataLoader.loadExtraInfo(pieces);
	std::vector<VertexMating> matings;
	dataLoader.loadVertexMatings(matings);
	World world(pieces,matings);
	world.Init();
	world.Simulation(isSimulationAuto);
	world.saveFinalTransforms(puzzleDirectory + "/final_transforms.csv");
	std::cout << "Finish" << std::endl;

	return 0;
}