#include "vertexMating.h"

VertexMating::VertexMating(std::string firstPieceId, b2Vec2 &firstPieceLocalCoords,
	std::string secondPieceId, b2Vec2 &secondPieceLocalCoords)
{
	firstPieceId_ = firstPieceId;
	firstPieceLocalCoords_ = firstPieceLocalCoords; 
	secondPieceId_ = secondPieceId; 
	secondPieceLocalCoords_ = secondPieceLocalCoords; 
}

bool VertexMating::operator==(const VertexMating& other) const {
	return (firstPieceId_ == other.firstPieceId_ &&
		firstPieceLocalCoords_ == other.firstPieceLocalCoords_ &&
		secondPieceId_ == other.secondPieceId_ &&
		secondPieceLocalCoords_ == other.secondPieceLocalCoords_);
}

