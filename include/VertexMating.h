#pragma once
#include <string>

class VertexMating
{
public:
	std::string firstPieceId_; // The id of the first piece in the mating
	int firstPieceVertex_; // the id of the edge of the first piece
	std::string secondPieceId_; // The id of the second piece in the mating
	int secondPieceVertex_; // the id of the edge of the second piece

	VertexMating(std::string firstPieceId, int firstPieceEdge, std::string secondPieceId, int secondPieceEdge);

	bool operator==(const VertexMating& other) const;
};

