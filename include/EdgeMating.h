#pragma once
class EdgeMating
{
public:
	int firstPieceId_; // The id of the first piece in the mating
	int firstPieceEdge_; // the id of the edge of the first piece
	int secondPieceId_; // The id of the second piece in the mating
	int secondPieceEdge_; // the id of the edge of the second piece

	EdgeMating(int firstPieceId, int firstPieceEdge, int secondPieceId, int secondPieceEdge);
};

