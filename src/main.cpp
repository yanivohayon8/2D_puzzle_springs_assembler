#include <DataLoader.h>
#include <Piece.h>

int main(int argc, char** argv)
{
	std::string puzzleDirectory = "../data/ofir/Pseudo-Sappho_MAN_Napoli_Inv9084/Puzzle1/0";
	DataLoader dataLoader(puzzleDirectory);
	std::vector<Piece> pieces;
	dataLoader.loadPieces(pieces);
	pieces[0].printCoords();
	std::cout << "Finish" << std::endl;
}