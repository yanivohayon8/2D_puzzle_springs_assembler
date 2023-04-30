#pragma once
class VertexMating
{
public:
	int firstPieceId_; // The id of the first piece in the mating
	int firstPieceVertex_; // the id of the edge of the first piece
	int secondPieceId_; // The id of the second piece in the mating
	int secondPieceVertex_; // the id of the edge of the second piece

	VertexMating(int firstPieceId, int firstPieceEdge, int secondPieceId, int secondPieceEdge);

	bool operator==(const VertexMating& other) const;
};

