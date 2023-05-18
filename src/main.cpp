#include <DataLoader.h>
#include <Piece.h>
#include <EdgeMating.h>
#include <World.h>
#include <VertexMating.h>
//#include "mapbox/earcut.hpp"

int main(int argc, char** argv)
{
	std::string puzzleDirectory = "../data/ofir/RePAIR/group_45";
	//std::string puzzleDirectory = "../data/ofir/RePAIR/group_39";
	//std::string puzzleDirectory = "../data/ofir/Roman_fresco_Villa_dei_Misteri_Pompeii_009/Puzzle1/0";
	bool isOfir = true;
	DataLoader dataLoader(puzzleDirectory);
	std::vector<Piece> pieces;
	dataLoader.loadPieces(pieces, isOfir);
	//dataLoader.loadExtraInfo(pieces);
	std::vector<VertexMating> matings;
	dataLoader.loadVertexMatings(matings);

	World world(pieces,matings);
	world.Init();
	world.Simulation();
	world.saveFinalTransforms(puzzleDirectory + "/final_transforms.csv");
	std::cout << "Finish" << std::endl;

	return 0;
}