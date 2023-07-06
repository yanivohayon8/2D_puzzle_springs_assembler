#include "Puzzle.h"

Puzzle::Puzzle(std::vector<Piece> &pieces, std::vector<VertexMating> groundTruthMatings)
{
	pieces_ = pieces;
	groundTruthMatings_ = groundTruthMatings;

	for (auto& piece : pieces_)
	{
		id2piece_.insert({ piece.id_,piece });
	}
}


void Puzzle::findPiecesToReconstruct(std::vector<Piece> &oReconstructPieces,std::vector<VertexMating>& matings)
{
	oReconstructPieces.clear();
	std::vector<std::string> reconstructPiecesIds;

	for (auto& mating : matings)
	{
		if (!std::count(reconstructPiecesIds.begin(), reconstructPiecesIds.end(), mating.firstPieceId_))
		{
			oReconstructPieces.push_back(id2piece_.at(mating.firstPieceId_));
			reconstructPiecesIds.push_back(mating.firstPieceId_);
		}

		if (!std::count(reconstructPiecesIds.begin(), reconstructPiecesIds.end(), mating.secondPieceId_))
		{
			oReconstructPieces.push_back(id2piece_.at(mating.secondPieceId_));
			reconstructPiecesIds.push_back(mating.secondPieceId_);
		}
	}	
}


