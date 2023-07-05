#include "Puzzle.h"

Puzzle::Puzzle(DataLoader& dataLoader, std::string mode)
{
	dataLoader_ = dataLoader;
	
	if (mode == "Visual")
	{
		reconstructor_ = new VisualReconstructor();
	}
	else {
		reconstructor_ = new SilentReconstructor();
	}
}

void Puzzle::initPuzzle()
{
	dataLoader_.loadPieces(pieces_);

	for (auto& piece: pieces_)
	{
		id2piece_.insert({ piece.id_,piece });
	}

	dataLoader_.loadVertexMatings(trueMatings_, "ground_truth_rels.csv");
	reconstructor_->init();
}


void Puzzle::initReconstruct(std::vector<VertexMating>& matings)
{
	std::vector<Piece> reconstructPieces;
	std::vector<std::string> reconstructPiecesIds;

	for (auto& mating : matings)
	{
		if (!std::count(reconstructPiecesIds.begin(), reconstructPiecesIds.end(), mating.firstPieceId_))
		{
			reconstructPieces.push_back(id2piece_.at(mating.firstPieceId_));
			reconstructPiecesIds.push_back(mating.firstPieceId_);
		}

		if (!std::count(reconstructPiecesIds.begin(), reconstructPiecesIds.end(), mating.secondPieceId_))
		{
			reconstructPieces.push_back(id2piece_.at(mating.secondPieceId_));
			reconstructPiecesIds.push_back(mating.secondPieceId_);
		}
	}

	reconstructor_->initRun(reconstructPieces, matings);
}

void Puzzle::reconstruct(std::vector<VertexMating>& matings)
{
	initReconstruct(matings);
	reconstructor_->Run();
}

