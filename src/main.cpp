#include <DataLoader.h>
#include <Piece.h>
#include <EdgeMating.h>
#include <World.h>


int main(int argc, char** argv)
{
	std::string puzzleDirectory = "../data/ofir/Roman_fresco_Villa_dei_Misteri_Pompeii_009/Puzzle1/4";
	//std::string puzzleDirectory = "../data/ofir/Roman_fresco_Villa_dei_Misteri_Pompeii_009/Puzzle3/0";
	//std::string puzzleDirectory = "../data/ofir/Roman_fresco_Villa_dei_Misteri_Pompeii_009/Puzzle2/0";
	//std::string puzzleDirectory = "../data/ofir/Roman_fresco_Villa_dei_Misteri_Pompeii_009/Puzzle1/0";
	//std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle3/0";
	//std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle2/0";
	//std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/1";
	//std::string puzzleDirectory = "../data/ofir/springs_nonpictorial_simple";
	DataLoader dataLoader(puzzleDirectory);
	std::vector<Piece> pieces;
	dataLoader.loadPieces(pieces,true);
	//pieces[0].printCoords();
	std::vector<EdgeMating> edgeMatings;
	dataLoader.loadEdgeMatings(edgeMatings);
	World world(pieces,edgeMatings);
	world.Init();
	//world.Simulation();
	world.AutomaticSimulation();
	std::cout << "Finish" << std::endl;

	return 0;
}