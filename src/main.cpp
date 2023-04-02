#include <DataLoader.h>
#include <Piece.h>
#include <EdgeMating.h>
#include <World.h>

int main(int argc, char** argv)
{
	std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0";
	DataLoader dataLoader(puzzleDirectory);
	std::vector<Piece> pieces;
	dataLoader.loadPieces(pieces);
	//pieces[0].printCoords();
	std::vector<EdgeMating> edgeMatings;
	dataLoader.loadEdgeMatings(edgeMatings);
	World world;
	world.insert_pieces(pieces);
	std::cout << "Finish" << std::endl;
}