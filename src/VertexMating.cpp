#include "VertexMating.h"

VertexMating::VertexMating(int firstPieceId, int firstPieceVertex, int secondPieceId, int secondPieceVertex)
{
	firstPieceId_ = firstPieceId;
	secondPieceId_ = secondPieceId;
	firstPieceVertex_ = firstPieceVertex;
	secondPieceVertex_ = secondPieceVertex;
}

bool VertexMating::operator==(const VertexMating& other) const {
	return (firstPieceId_ == other.firstPieceId_ &&
		firstPieceVertex_ == other.firstPieceVertex_ &&
		secondPieceId_ == other.secondPieceId_ &&
		secondPieceVertex_ == other.secondPieceVertex_);
}
