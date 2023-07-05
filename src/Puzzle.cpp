#include "Puzzle.h"

Puzzle::Puzzle(DataLoader& dataLoader)
{
	dataLoader_ = dataLoader;
}

void Puzzle::initPuzzle()
{
	dataLoader_.loadPieces(pieces_);

	for (auto& piece: pieces_)
	{
		id2piece_.insert({ piece.id_,piece });
	}

	dataLoader_.loadVertexMatings(trueMatings_, "ground_truth_rels.csv");
}

void Puzzle::reconstruct(std::vector<VertexMating>& matings)
{
	std::vector<Piece> reconstructPieces;
	std::vector<std::string> reconstructPiecesIds;

	for (auto& mating : matings)
	{

		if (std::find(reconstructPiecesIds.begin(), 
			reconstructPiecesIds.end(), mating.firstPieceId_) == reconstructPiecesIds.end())
		{	
			reconstructPieces.push_back(id2piece_.at(mating.firstPieceId_));
			reconstructPiecesIds.push_back(mating.firstPieceId_);
		}

		if (std::find(reconstructPiecesIds.begin(),
			reconstructPiecesIds.end(), mating.secondPieceId_) == reconstructPiecesIds.end())
		{		
			reconstructPieces.push_back(id2piece_.at(mating.secondPieceId_));
			reconstructPiecesIds.push_back(mating.secondPieceId_);
		}
	}

}


