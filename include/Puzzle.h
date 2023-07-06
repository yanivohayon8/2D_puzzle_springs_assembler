#pragma once
#include <Piece.h>
#include <reconstruction.h>
#include <VisualReconstructor.h>
#include <SilentReconstructor.h>
#include <DataLoader.h>
#include <string>

class Puzzle
{
private:
	std::vector<Piece> pieces_;
	std::map<std::string, Piece> id2piece_;
	std::vector<VertexMating> groundTruthMatings_;
	
public:
	Puzzle(std::vector<Piece>& pieces, std::vector<VertexMating> groundTruthMatings);
	void findPiecesToReconstruct(std::vector<Piece> &oReconstructPieces, std::vector<VertexMating>& matings);
};

